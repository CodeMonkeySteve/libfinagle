/*!
** \file List.h
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

#ifndef FINAGLE_LIST_H
#define FINAGLE_LIST_H

#include <list>
#include <algorithm>

namespace Finagle {

//! \brief Wrapper for the STL \c list class.
template <typename Type, typename Alloc = std::allocator<Type> >
class List : public std::list<Type, Alloc> {
public:
  typedef typename std::list<Type, Alloc>::size_type size_type;
  typedef typename std::list<Type, Alloc>::iterator Iterator;
  typedef typename std::list<Type, Alloc>::const_iterator ConstIterator;

public:
  List( void ) {}
  List( size_type n ) : std::list<Type, Alloc>( n ) {}
  List( size_type n, Type const &t ) : std::list<Type, Alloc>( n, t ) {}
  List( Type const &t ) : std::list<Type, Alloc>( 1, t ) {}
  template <typename Iterator>
  List( Iterator begin, Iterator end ) : std::list<Type, Alloc>( begin, end ) {}
  template <typename Container>
  explicit List( Container container ) : std::list<Type, Alloc>( container.begin(), container.end() ) {}

  Type &operator[]( unsigned index );
  Type const &operator[]( unsigned index ) const;

  ConstIterator find( Type const &el ) const;
  Iterator find( Type const &el );
  bool contains( Type const &el ) const;

  // For sorted lists
  Iterator insert( Type const &el );

  // For unsorted-lists (pass-through wrappers)
  Iterator insert( Iterator pos, Type const &el );
  template <class InputIterator>
  void insert( Iterator pos, InputIterator first, InputIterator last );
  void insert( Iterator pos, size_type n, Type const &el );
};

// INLINE/TEMPLATE IMPLEMENTATION *********************************************

/*! \brief Returns a reference to the element at index \a Index
**
** If no element exists at that location, returns a reference to a static
** element (i.e. \c null).
*/
template <typename Type, typename Alloc>
inline Type &List<Type, Alloc>::operator[]( unsigned index )
{
  Type const &r = operator[]( index );
  return const_cast<Type &>( r );
}

template <typename Type, typename Alloc>
inline typename List<Type, Alloc>::ConstIterator List<Type, Alloc>::find( Type const &el ) const
{
  return std::find( List::begin(), List::end(), el );
}

template <typename Type, typename Alloc>
inline typename List<Type, Alloc>::Iterator List<Type, Alloc>::find( Type const &el )
{
  return std::find( List::begin(), List::end(), el );
}

//! Returns \c true iff the list contains an element equal to \a El.
template <typename Type, typename Alloc>
inline bool List<Type, Alloc>::contains( Type const &el ) const
{
  return std::find( List::begin(), List::end(), el ) != List::end();
}

/*! \brief Returns a reference to the element at index \a Index
**
** If no element exists at that location, returns a reference to a static
** element (i.e. \c null).
*/
template <typename Type, typename Alloc>
Type const &List<Type, Alloc>::operator[]( unsigned index ) const
{
  static Type null;

  if ( index >= List::size() )
    return null;

  ConstIterator i = List::begin();
  while ( index-- )
    ++i;

  return *i;
}

template <typename Type, typename Alloc>
typename List<Type, Alloc>::Iterator List<Type, Alloc>::insert( Type const &el )
{
  for ( Iterator i = List::begin(); i != List::end(); ++i ) {
    if ( el < *i )
      return List::insert( i, el );
  }

  push_back( el );
  return --List::end();
}

template <typename Type, typename Alloc>
typename List<Type, Alloc>::Iterator List<Type, Alloc>::insert( Iterator pos, Type const &el )
{
  return List::insert( pos, el );
}

template <typename Type, typename Alloc>
template <class InputIterator>
void List<Type, Alloc>::insert( Iterator pos, InputIterator first, InputIterator last )
{
  List::insert( pos, first, last );
}

template <typename Type, typename Alloc>
void List<Type, Alloc>::insert( Iterator pos, size_type n, Type const &el )
{
  List::insert( pos, n, el );
}

}

#endif
