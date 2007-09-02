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

using std::deque;

template <typename Type>
class Queue : protected deque<Type> {
public:
  Queue( void ) {}

  bool empty( void ) const;
  unsigned size( void ) const;

  void push_back( Type const &el );
  void push_front( Type const &el );

  Type pop_back( void );
  Type pop_front( void );

  bool pop_back( Type &dest, Time timeout = 0 );
  bool pop_front( Type &dest, Time timeout = 0 );

public:
  template <typename Functor>
  void whenNotEmpty( Functor &func );

  template <typename Functor>
  bool ifNotEmpty( Functor &func, Time timeout = 0 );

protected:  // functors
  class BackPopper {
  public:
    BackPopper( Type &dest ) : _dest(dest) {}
    void operator()( Queue<Type> &queue ) {
      deque<Type> &q( (deque<Type> &) queue );
      _dest = q.back();
      q.pop_back();
    }

  protected:
    Type &_dest;
  };
  friend class BackPopper;

  class FrontPopper : protected BackPopper {
  public:
    FrontPopper( Type &dest ) : BackPopper(dest) {}
    void operator()( Queue<Type> &queue ) {
      deque<Type> &q( (deque<Type> &) queue );
      BackPopper::_dest = q.front();
      q.pop_front();
    }
  };
  friend class FrontPopper;

protected:
  mutable Mutex _guard;
  WaitCondition _notEmpty;
};

// INLINE/TEMPLATE IMPLEMENTATION *************************************************************************************************

//! Returns \c true iff the queue is empty.
template <typename Type>
inline bool Queue<Type>::empty( void ) const
{
  Lock _( _guard );
  return deque<Type>::empty();
}

//! Returns the number of items in the queue.
template <typename Type>
inline unsigned Queue<Type>::size( void ) const
{
  Lock _( _guard );
  return deque<Type>::size();
}


//! Adds an item to the tail of the queue
template <typename Type>
inline void Queue<Type>::push_back( Type const &el )
{
  Lock _( _guard );
  deque<Type>::push_back( el );
  _notEmpty.signalOne();
}

//! Adds an item to the head of the queue
template <typename Type>
inline void Queue<Type>::push_front( Type const &el )
{
  Lock _( _guard );
  deque<Type>::push_front( el );
  _notEmpty.signalOne();
}

//! Returns the item at the tail of the queue.  If the queue is empty, blocks until an item has been added.
template <typename Type>
inline Type Queue<Type>::pop_back( void )
{
  Type dest;
  BackPopper p( dest );
  whenNotEmpty( p );
  return dest;
}

//! Returns the item at the head of the queue.  If the queue is empty, blocks until an item has been added.
template <typename Type>
inline Type Queue<Type>::pop_front( void )
{
  Type dest;
  FrontPopper p( dest );
  whenNotEmpty( p );
  return dest;
}

//! Attempts to pop the tail of the queue, waiting up to \a timeout for an entry to appear.  If the queue is empty, returns \c false.
//! Otherwise, stores the item in \a dest and returns \c true.
template <typename Type>
inline bool Queue<Type>::pop_back( Type &dest, Time timeout )
{
  BackPopper p( dest );
  return ifNotEmpty( p, timeout );
}


//! Attempts to pop the head of the queue, waiting up to \a timeout for an entry to appear.  If the queue is empty, returns \c false.
//! Otherwise, stores the item in \a dest and returns \c true.
template <typename Type>
inline bool Queue<Type>::pop_front( Type &dest, Time timeout )
{
  FrontPopper p( dest );
  return ifNotEmpty( p, timeout );
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

}

#endif
