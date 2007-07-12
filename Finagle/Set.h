/*!
** \file Set.h
** \author Steve Sloan <steve@finagle.org>
** \date Sun Dec 10 2006
** Copyright (C) 2006 by Steve Sloan
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

#ifndef FINAGLE_SET_H
#define FINAGLE_SET_H

#include <set>

namespace Finagle {

//! \brief Wrapper for the STL \c set class.
template <class Type, class Compare = std::less<Type>, class Alloc = std::allocator<Type> >
class Set : public std::set<Type, Compare, Alloc> {
public:
  typedef typename std::set<Type, Compare, Alloc>::iterator Iterator;
  typedef typename std::set<Type, Compare, Alloc>::const_iterator ConstIterator;

public:
  Set( void ) {}

  bool contains( Type const &Key ) const;
};

// INLINE/TEMPLATE IMPLEMENTATION *********************************************

template <typename Type, typename Compare, typename Alloc>
inline bool Set<Type, Compare, Alloc>::contains( Type const &key ) const
{
  return find(key) != Set::end();
}

};

#endif
