/*!
** \file SortedQueue.h
** \author Steve Sloan <steve@finagle.org>
** \date Mon Aug 27 2007
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

#ifndef FINAGLE_SORTEDQUEUE_H
#define FINAGLE_SORTEDQUEUE_H

#include <set>

template <typename Type>
class SortedQueue {
public:
  SortedQueue( void );

  void push( Type const &el );
  void unpop( Type const &el );

  Type pop( void );

protected:
  mutable Mutex _guard;
  std::multiset<Type> _queue;

};

// INLINE IMPLEMENTATION ******************************************************

#endif
