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

  Type pop( void );

  bool tug( Type &dest );
  bool tug( Type &dest, Time timeout );

protected:
  mutable Mutex _guard;
  std::deque<Type> _queue;
  WaitCondition _notEmpty;
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

//! Returns the item at the head of the queue.  If the queue is empty, blocks until an item has been added.
template <typename Type>
Type Queue<Type>::pop( void )
{
  _guard.lock();

  if ( empty() ) {
    _notEmpty.lock();
    _guard.unlock();
    _notEmpty.wait();
    _guard.lock();
    _notEmpty.unlock();
  }

  Type t( _queue.front() );
  _queue.pop_front();
  _guard.unlock();
  return t;
}


//! Attempts to pop the head of the queue.  If the queue is empty, returns \c false immediately.
//! Otherwise, stores the item in \a dest and returns \c true.
template <typename Type>
bool Queue<Type>::tug( Type &dest )
{
  Lock _( _guard );
  if ( _queue.empty() )
    return false;

  dest = _queue.front();
  _queue.pop_front();
  return true;
}

//! Attempts to pop the head of the queue, waiting up to \a timeout for an entry to appear.  If the queue is empty, returns \c false.
//! Otherwise, stores the item in \a dest and returns \c true.
template <typename Type>
bool Queue<Type>::tug( Type &dest, Time timeout )
{
  _guard.lock();

  if ( empty() ) {
    _notEmpty.lock();
    _guard.unlock();

    if ( !_notEmpty.wait( timeout ) ) {
      _notEmpty.unlock();
      return false;
    }

    _guard.lock();
    _notEmpty.unlock();
  }

  dest = _queue.front();
  _queue.pop_front();

  _guard.unlock();
  return true;
}


}

#endif
