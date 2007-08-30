/*!
** \file Queue.h
** \author Steve Sloan <ssloan@vigilos.com>
** \date Wed Mar 28 2007
** Copyright (C) 2007 by Vigilos, Inc., All Rights Reserved
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose can only be granted in writing by Vigilos, Inc.
*/

#ifndef FINAGLE_QUEUE_H
#define FINAGLE_QUEUE_H

#include <deque>
#include <Finagle/WaitCondition.h>
#include <Finagle/Mutex.h>

namespace Finagle {

template <typename Type>
class Queue {
public:
  Queue( void ) {}

  bool empty( void ) const;
  unsigned size( void ) const;

  void push( Type const &el );
  void unpop( Type const &el );

public:
  template <typename Functor>
  bool ifNotEmpty( Functor &func, Time timeout = 0 );

  template <typename Functor>
  void whenNotEmpty( Functor &func );

  bool tug( Type &dest, Time timeout = 0 );
  Type pop( void );

protected:
  mutable Mutex _guard;
  std::deque<Type> _queue;
  WaitCondition _notEmpty;

protected:
  class Popper {
  public:
    Popper( Type &dest ) : _dest(dest) {}
    void operator()( Queue<Type> &queue )
    {
      _dest = queue._queue.front();
      queue._queue.pop_front();
    }

  protected:
    Type &_dest;
  };
  friend class Popper;
};

// INLINE/TEMPLATE IMPLEMENTATION *************************************************************************************************

//! Returns \c true iff the queue is empty.
template <typename Type>
inline bool Queue<Type>::empty( void ) const
{
  Lock _( _guard );
  return _queue.empty();
}

//! Returns the number of items in the queue.
template <typename Type>
inline unsigned Queue<Type>::size( void ) const
{
  Lock _( _guard );
  return _queue.size();
}


//! Adds an item to the tail of the queue
template <typename Type>
void Queue<Type>::push( Type const &el )
{
  Lock _( _guard );
  _queue.push_back( el );
  _notEmpty.signalOne();
}

//! Adds an item to the head of the queue
template <typename Type>
void Queue<Type>::unpop( Type const &el )
{
  Lock _( _guard );
  _queue.push_front( el );
  _notEmpty.signalOne();
}


//! Calls functor \a func (passing the queue reference) if the queue is not empty, or becomes non-empty before \a timeout, and returns \c true.
//! If the queue is still empty, \a func is not called and \c false is returned.
template <typename Type>
template <typename Functor>
bool Queue<Type>::ifNotEmpty( Functor &func, Time timeout )
{
  _guard.lock();

  if ( empty() ) {
    _notEmpty.lock();
    _guard.unlock();

    if ( !_notEmpty.wait( timeout ) || empty() ) {
      _notEmpty.unlock();
      return false;
    }

    _guard.lock();
    _notEmpty.unlock();
  }

  func( *this );
  _guard.unlock();
  return true;
}

//! Calls functor \a func (passing the queue reference).  If the queue is empty, blocks until an item has been added.
template <typename Type>
template <typename Functor>
void Queue<Type>::whenNotEmpty( Functor &func )
{
  _guard.lock();

  while ( empty() ) {
    _notEmpty.lock();
    _guard.unlock();
    _notEmpty.wait();
    _guard.lock();
    _notEmpty.unlock();
  }

  func( *this );
  _guard.unlock();
}

//! Attempts to pop the head of the queue, waiting up to \a timeout for an entry to appear.  If the queue is empty, returns \c false.
//! Otherwise, stores the item in \a dest and returns \c true.
template <typename Type>
bool Queue<Type>::tug( Type &dest, Time timeout )
{
  Popper f( dest );
  return ifNotEmpty( f, timeout );
}

//! Returns the item at the head of the queue.  If the queue is empty, blocks until an item has been added.
template <typename Type>
Type Queue<Type>::pop( void )
{
  Type dest;
  Popper f( dest );
  whenNotEmpty( f );
  return dest;
}

}

#endif
