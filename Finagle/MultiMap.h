/*!
** \file MultiMap.h
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

#ifndef FINAGLE_MULTIMAP_H
#define FINAGLE_MULTIMAP_H

#include <map>
#include <Finagle/MapIterator.h>

namespace Finagle {

//! \brief Wrapper for the STL \c multimap class

template <typename KeyType, typename DataType, typename CompareType = std::less<KeyType>, typename AllocType = std::allocator<std::pair<const KeyType, DataType> > >
class MultiMap : public std::multimap<KeyType, DataType, CompareType, AllocType> {
public:
  typedef MapIterator<typename std::multimap<KeyType, DataType, CompareType, AllocType>::iterator> Iterator;
  typedef MapConstIterator<typename std::multimap<KeyType, DataType, CompareType, AllocType>::const_iterator> ConstIterator;

public:
  MultiMap( void );

  ConstIterator begin( void ) const;
  ConstIterator end( void ) const;

  Iterator begin( void );
  Iterator end( void );

  bool contains( KeyType const &Key ) const;
  DataType const &operator[]( KeyType const &Key ) const;
  DataType &operator[]( KeyType const &Key );
  DataType &insert( KeyType const &Key, DataType const &Data );
  DataType &insert( KeyType const &Key );
};

// INLINE/TEMPLATE IMPLEMENTATION *********************************************

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline MultiMap<KeyType, DataType, CompareType, AllocType>::MultiMap( void )
{
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline typename MultiMap<KeyType, DataType, CompareType, AllocType>::ConstIterator MultiMap<KeyType, DataType, CompareType, AllocType>::begin( void ) const
{
  return( std::multimap<KeyType, DataType, CompareType, AllocType>::begin() );
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline typename MultiMap<KeyType, DataType, CompareType, AllocType>::ConstIterator MultiMap<KeyType, DataType, CompareType, AllocType>::end( void ) const
{
  return( std::multimap<KeyType, DataType, CompareType, AllocType>::end() );
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline typename MultiMap<KeyType, DataType, CompareType, AllocType>::Iterator MultiMap<KeyType, DataType, CompareType, AllocType>::begin( void )
{
  return( std::multimap<KeyType, DataType, CompareType, AllocType>::begin() );
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline typename MultiMap<KeyType, DataType, CompareType, AllocType>::Iterator MultiMap<KeyType, DataType, CompareType, AllocType>::end( void )
{
  return( std::multimap<KeyType, DataType, CompareType, AllocType>::end() );
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline bool MultiMap<KeyType, DataType, CompareType, AllocType>::contains( KeyType const &Key ) const
{
  return( find(Key) != MultiMap::end() );
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline DataType const &MultiMap<KeyType, DataType, CompareType, AllocType>::operator[]( KeyType const &Key ) const
{
  static DataType null;
  ConstIterator i = find( Key );
  return( (i != MultiMap::end()) ? i.val() : null );
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline DataType &MultiMap<KeyType, DataType, CompareType, AllocType>::operator[]( KeyType const &Key )
{
  Iterator i = find( Key );
  return (i != MultiMap::end()) ? i.val() : insert( Key );
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline DataType &MultiMap<KeyType, DataType, CompareType, AllocType>::insert( KeyType const &Key, DataType const &Data )
{
  return( std::multimap<KeyType, DataType, CompareType, AllocType>::insert( std::pair<KeyType, DataType>( Key, Data ) )->second );
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline DataType &MultiMap<KeyType, DataType, CompareType, AllocType>::insert( KeyType const &Key )
{
  return( insert( Key, DataType() ) );
}

}

#endif
