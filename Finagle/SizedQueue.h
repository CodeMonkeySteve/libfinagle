/*!
** \file SizedQueue.h
** \author Steve Sloan <steve@finagle.org>
** \date Mon Aug 20 2007
** Copyright (C) 2007 by Steve Sloan
**
** This library is free software; you can redistribute it and/or modify it
** under the terms of the GNU Lesser General Public License as published
** by the Free Software Foundation; either version 2.1 of the License, or
** (at your option) any later version.
**
** This library is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, you may access it via the web
** at http://www.gnu.org/copyleft/lesser.html .
*/

#ifndef FINAGLE_SIZEDQUEUE_H
#define FINAGLE_SIZEDQUEUE_H

#include <Finagle/Queue.h>

namespace Finagle {

template <typename Type>
class SizedQueue : public Queue<Type> {
public:
  SizedQueue( unsigned capacity );

  unsigned capacity( void ) const;
  unsigned capacity( unsigned capacity );
  bool full( void ) const;

  template <typename Functor>
  bool ifNotFull( Functor &func, Time timeout = 0 );

  template <typename Functor>
  void whenNotFull( Functor &func );

  void push( Type const &el );

public:
  Type pop( void );
  bool tug( Type &dest, Time timeout = 0 );

protected:
  unsigned _capacity;
  WaitCondition _notFull;

protected: // functors
  class Pusher {
  public:
    Pusher( Type const &src ) : _src(src) {}
    void operator()( SizedQueue<Type> &queue ) {  queue._queue.push_back( _src );  }

  protected:
    Type const &_src;
  };
  friend class Pusher;

  struct Popper : public Queue<Type>::Popper {
    Popper( Type &dest ) : Queue<Type>::Popper(dest) {}
    void operator()( Queue<Type> &queue )
    {
      Queue<Type>::Popper::operator()( queue );
      SizedQueue<Type> &q( (SizedQueue<Type> &) queue );
      if ( !q.full() )
       q._notFull.signalOne();
    }
  };
  friend class Popper;
};

// INLINE IMPLEMENTATION ******************************************************

//! Initialized the queue with the given \a capacity.
template <typename Type>
inline SizedQueue<Type>::SizedQueue( unsigned capacity )
: _capacity( capacity )
{}

template <typename Type>
inline unsigned SizedQueue<Type>::capacity( void ) const
{
  return _capacity;
}

template <typename Type>
inline unsigned SizedQueue<Type>::capacity( unsigned capacity )
{
  return _capacity = capacity;
}

template <typename Type>
inline bool SizedQueue<Type>::full( void ) const
{
  Lock _( Queue<Type>::_guard );
  return Queue<Type>::_queue.size() >= _capacity;
}


//! Calls functor \a func (passing the queue reference) if the queue is not empty, or becomes non-empty before \a timeout, and returns \c true.
//! If the queue is still empty, \a func is not called and \c false is returned.
template <typename Type>
template <typename Functor>
bool SizedQueue<Type>::ifNotFull( Functor &func, Time timeout )
{
  Queue<Type>::_guard.lock();
  if ( full() ) {
    _notFull.lock();
    Queue<Type>::_guard.unlock();

    if ( ((timeout != 0.0) && !_notFull.wait( timeout )) || full() ) {
      _notFull.unlock();
      return false;
    }

    Queue<Type>::_guard.lock();
    _notFull.unlock();
  }

  func( *this );
  Queue<Type>::_guard.unlock();
  return true;
}

//! If the queue is full, blocks until an item has been removed.  Then calls functor \a func (passing the queue reference).
template <typename Type>
template <typename Functor>
void SizedQueue<Type>::whenNotFull( Functor &func )
{
  Queue<Type>::_guard.lock();
  while ( full() ) {
    _notFull.lock();
    Queue<Type>::_guard.unlock();
    _notFull.wait();
    Queue<Type>::_guard.lock();
    _notFull.unlock();
  }

  func( *this );
  Queue<Type>::_guard.unlock();
}

//! Adds an item \a el to the tail of the queue.  Will block while the queue is full.
template <typename Type>
void SizedQueue<Type>::push( Type const &el )
{
  Pusher p( el );
  whenNotFull( p );
}


//! Overridden to use our Popper functor.
template <typename Type>
Type SizedQueue<Type>::pop( void )
{
  Type dest;
  Popper p( dest );
  whenNotEmpty( p );
  return dest;
}

//! Overridden to use our Popper functor.
template <typename Type>
bool SizedQueue<Type>::tug( Type &dest, Time timeout )
{
  Popper f( dest );
  return ifNotEmpty( f, timeout );
}

}

#endif
