/*!
** \file Array.h
** \author Steve Sloan <steve@finagle.org>
** \date Fri Jul 23 2004
** Copyright (C) 2004 by Steve Sloan
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

#ifndef FINAGLE_ARRAY_H
#define FINAGLE_ARRAY_H

#include <algorithm>
#include <vector>

namespace Finagle {

//! Wrapper for the (misnamed, IMNSHO) STL \c vector class.
template <typename Type, typename AllocType = std::allocator<Type> >
class Array : public std::vector<Type, AllocType> {
public:
  typedef typename std::vector<Type, AllocType>::iterator Iterator;
  typedef typename std::vector<Type, AllocType>::const_iterator ConstIterator;
  typedef typename std::vector<Type, AllocType>::size_type size_type;

public:
  Array( void ) {}
  Array( size_type n ) : std::vector<Type, AllocType>( n ) {}
  Array( size_type n, Type const &t ) : std::vector<Type, AllocType>( n, t ) {}
  Array( Type const &t ) : std::vector<Type, AllocType>( 1, t ) {}

  template <class InputIterator>
  Array( InputIterator b, InputIterator e ) : std::vector<Type, AllocType>( b, e ) {}

  Type &operator[]( size_type i );
  Type const &operator[]( size_type i ) const;

  bool contains( Type const &El ) const;

  void sort( void );
};

// INLINE IMPLEMENTATION **********************************************************************************************************

template <typename Type, typename AllocType>
inline Type &Array<Type, AllocType>::operator[]( size_type i )
{
  if ( i >= std::vector<Type, AllocType>::size() )
    resize( i + 1 );

  return std::vector<Type, AllocType>::operator[]( i );
}

template <typename Type, typename AllocType>
inline Type const &Array<Type, AllocType>::operator[]( size_type i ) const
{
  return std::vector<Type, AllocType>::operator[]( i );
}

//! Returns \c true iff the array contains an element equal to \a el.
template <typename Type, typename AllocType>
inline bool Array<Type, AllocType>::contains( Type const &el ) const
{
  return std::find( Array::begin(), Array::end(), el ) != Array::end();
}


//! Sorts the array using the std::sort algorithm.
template <typename Type, typename AllocType>
inline void Array<Type, AllocType>::sort( void )
{
  std::sort( Array::begin(), Array::end() );
}


}

#endif
