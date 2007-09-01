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

public:
  void push_back( Type const &el );
  void push_front( Type const &el );

  Type pop_back( void );
  Type pop_front( void );

  bool pop_back( Type &dest, Time timeout = 0 );
  bool pop_front( Type &dest, Time timeout = 0 );


public:
  template <typename Functor>
  void whenNotFull( Functor &func );

  template <typename Functor>
  bool ifNotFull( Functor &func, Time timeout = 0 );

protected:
  unsigned _capacity;
  WaitCondition _notFull;

protected: // functors
  class BackPusher {
  public:
    BackPusher( Type const &src ) : _src(src) {}
    void operator()( SizedQueue<Type> &queue ) {  ((deque<Type> &) queue).push_back( _src );  }

  protected:
    Type const &_src;
  };
  friend class BackPusher;

  class FrontPusher : protected BackPusher {
  public:
    FrontPusher( Type const &src ) : BackPusher(src) {}
    void operator()( SizedQueue<Type> &queue ) {  ((std::deque<Type> &) queue).push_front( BackPusher::_src );  }
  };
  friend class FrontPusher;

  struct BackPopper : public Queue<Type>::BackPopper {
    BackPopper( Type &dest ) : Queue<Type>::BackPopper(dest) {}
    void operator()( Queue<Type> &queue ) {
      Queue<Type>::BackPopper::operator()( queue );
      SizedQueue<Type> &q( (SizedQueue<Type> &) queue );
      if ( !q.full() )
       q._notFull.signalOne();
    }
  };
  friend class BackPopper;

  struct FrontPopper : public Queue<Type>::FrontPopper {
    FrontPopper( Type &dest ) : Queue<Type>::FrontPopper(dest) {}
    void operator()( Queue<Type> &queue ) {
      Queue<Type>::FrontPopper::operator()( queue );
      SizedQueue<Type> &q( (SizedQueue<Type> &) queue );
      if ( !q.full() )
       q._notFull.signalOne();
    }
  };
  friend class FrontPopper;
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
  return Queue<Type>::size() >= _capacity;
}


//! Adds an item \a el to the tail of the queue.  Will block while the queue is full.
template <typename Type>
inline void SizedQueue<Type>::push_back( Type const &el )
{
  BackPusher p( el );
  whenNotFull( p );
}

//! Adds an item \a el to the head of the queue.  Will block while the queue is full.
template <typename Type>
inline void SizedQueue<Type>::push_front( Type const &el )
{
  FrontPusher p( el );
  whenNotFull( p );
}

//! Overridden to use our functor.
template <typename Type>
inline Type SizedQueue<Type>::pop_back( void )
{
  Type dest;
  BackPopper p( dest );
  whenNotEmpty( p );
  return dest;
}

//! Overridden to use our functor.
template <typename Type>
inline Type SizedQueue<Type>::pop_front( void )
{
  Type dest;
  FrontPopper p( dest );
  whenNotEmpty( p );
  return dest;
}

//! Overridden to use our functor.
template <typename Type>
inline bool SizedQueue<Type>::pop_back( Type &dest, Time timeout )
{
  BackPopper p( dest );
  return ifNotEmpty( p, timeout );
}

//! Overridden to use our functor.
template <typename Type>
inline bool SizedQueue<Type>::pop_front( Type &dest, Time timeout )
{
  FrontPopper p( dest );
  return ifNotEmpty( p, timeout );
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

}

#endif
