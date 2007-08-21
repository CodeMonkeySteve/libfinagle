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
  virtual ~SizedQueue( void ) {}

  unsigned capacity( void ) const;
  unsigned capacity( unsigned capacity );
  bool full( void ) const;

  virtual void push( Type const &el );

protected:
  virtual void pop_front( Type & );

protected:
  unsigned _capacity;
  WaitCondition _notFull;
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


//! Adds an item \a el to the tail of the queue.  Will block while the queue is full.
template <typename Type>
void SizedQueue<Type>::push( Type const &el )
{
  Queue<Type>::_guard.lock();
  if ( Queue<Type>::_queue.size() >= _capacity ) {
    _notFull.lock();
    Queue<Type>::_guard.unlock();
    _notFull.wait();
    Queue<Type>::_guard.lock();
    _notFull.unlock();
  }

  Queue<Type>::_queue.push_back( el );
  Queue<Type>::_notEmpty.signalOne();
  Queue<Type>::_guard.unlock();
}

template <typename Type>
void SizedQueue<Type>::pop_front( Type &dest )
{
  Queue<Type>::pop_front( dest );
  if ( Queue<Type>::_queue.size() < _capacity )
    _notFull.signalOne();
}

}

#endif
