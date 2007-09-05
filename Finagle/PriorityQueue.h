/*!
** \file PriorityQueue.h
** \author Steve Sloan <steve@finagle.org>
** \date Tue Sep 4 2007
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

#ifndef FINAGLE_PRIORITYQUEUE_H
#define FINAGLE_PRIORITYQUEUE_H

#include <Finagle/DateTime.h>
#include <Finagle/WaitCondition.h>
#include <Finagle/MultiMap.h>

namespace Finagle {

template <typename Type, typename PriType = double>
class PriorityQueue : protected MultiMap<PriType, Type, std::greater<PriType> > {
public:
  PriorityQueue( void ) {}

  bool empty( void ) const;
  unsigned size( void ) const;

  void push( Type const &el, PriType const &pri = 0 );
  Type pop( void );
  bool pop( Type &dest, Time timeout = 0 );

public:
  template <typename Functor>
  void whenNotEmpty( Functor &func );

  template <typename Functor>
  bool ifNotEmpty( Functor &func, Time timeout = 0 );

protected:  // functors
  typedef MultiMap<PriType, Type, std::greater<PriType> > Map;

  class FrontPopper {
  public:
    FrontPopper( Type &dest ) : _dest(dest) {}
    void operator()( PriorityQueue<Type, PriType> &queue ) {
      typename Map::Iterator i( queue.begin() );
      _dest = *i;
      queue.erase( i );
    }

  protected:
    Type &_dest;
  };
  friend class FrontPopper;

protected:
  mutable Mutex _guard;
  WaitCondition _notEmpty;
};

// INLINE IMPLEMENTATION ******************************************************

//! Returns \c true iff the queue is empty.
template <typename Type, typename PriType>
inline bool PriorityQueue<Type, PriType>::empty( void ) const
{
  Lock _( _guard );
  return Map::empty();
}

//! Returns the number of items in the queue.
template <typename Type, typename PriType>
inline unsigned PriorityQueue<Type, PriType>::size( void ) const
{
  Lock _( _guard );
  return Map::size();
}


//! Adds an item to the queue
template <typename Type, typename PriType>
inline void PriorityQueue<Type, PriType>::push( Type const &el, PriType const &pri )
{
  Lock _( _guard );
  Map::insert( pri, el );
  _notEmpty.signalOne();
}

//! Returns the item at the tail of the queue.  If the queue is empty, blocks until an item has been added.
template <typename Type, typename PriType>
inline Type PriorityQueue<Type, PriType>::pop( void )
{
  Type dest;
  FrontPopper p( dest );
  whenNotEmpty( p );
  return dest;
}

//! Attempts to pop the tail of the queue, waiting up to \a timeout for an entry to appear.  If the queue is empty, returns \c false.
//! Otherwise, stores the item in \a dest and returns \c true.
template <typename Type, typename PriType>
inline bool PriorityQueue<Type, PriType>::pop( Type &dest, Time timeout )
{
  FrontPopper p( dest );
  return ifNotEmpty( p, timeout );
}

//! Calls functor \a func (passing the queue reference).  If the queue is empty, blocks until an item has been added.
template <typename Type, typename PriType>
template <typename Functor>
void PriorityQueue<Type, PriType>::whenNotEmpty( Functor &func )
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
template <typename Type, typename PriType>
template <typename Functor>
bool PriorityQueue<Type, PriType>::ifNotEmpty( Functor &func, Time timeout )
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
