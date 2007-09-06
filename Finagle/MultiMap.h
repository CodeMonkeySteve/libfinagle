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
  typedef MapIterator<typename std::multimap<KeyType, DataType, CompareType, AllocType>::reverse_iterator> ReverseIterator;
  typedef MapConstIterator<typename std::multimap<KeyType, DataType, CompareType, AllocType>::const_reverse_iterator> ConstReverseIterator;

public:
  MultiMap( void );

  ConstIterator begin( void ) const;
  ConstIterator end( void ) const;
  ConstReverseIterator rbegin( void ) const;
  ConstReverseIterator rend( void ) const;

  Iterator begin( void );
  Iterator end( void );
  ReverseIterator rbegin( void );
  ReverseIterator rend( void );

  bool contains( KeyType const &key ) const;
  DataType const &operator[]( KeyType const &key ) const;
  DataType &operator[]( KeyType const &key );
  DataType &insert( KeyType const &key, DataType const &val );
  DataType &insert( KeyType const &key );
};

// INLINE/TEMPLATE IMPLEMENTATION *********************************************

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline MultiMap<KeyType, DataType, CompareType, AllocType>::MultiMap( void )
{}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline typename MultiMap<KeyType, DataType, CompareType, AllocType>::ConstIterator MultiMap<KeyType, DataType, CompareType, AllocType>::begin( void ) const
{
  return std::multimap<KeyType, DataType, CompareType, AllocType>::begin();
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline typename MultiMap<KeyType, DataType, CompareType, AllocType>::ConstIterator MultiMap<KeyType, DataType, CompareType, AllocType>::end( void ) const
{
  return std::multimap<KeyType, DataType, CompareType, AllocType>::end();
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline typename MultiMap<KeyType, DataType, CompareType, AllocType>::ConstReverseIterator MultiMap<KeyType, DataType, CompareType, AllocType>::rbegin( void ) const
{
  return std::multimap<KeyType, DataType, CompareType, AllocType>::rbegin();
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline typename MultiMap<KeyType, DataType, CompareType, AllocType>::ConstReverseIterator MultiMap<KeyType, DataType, CompareType, AllocType>::rend( void ) const
{
  return std::multimap<KeyType, DataType, CompareType, AllocType>::rend();
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline typename MultiMap<KeyType, DataType, CompareType, AllocType>::Iterator MultiMap<KeyType, DataType, CompareType, AllocType>::begin( void )
{
  return std::multimap<KeyType, DataType, CompareType, AllocType>::begin();
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline typename MultiMap<KeyType, DataType, CompareType, AllocType>::Iterator MultiMap<KeyType, DataType, CompareType, AllocType>::end( void )
{
  return std::multimap<KeyType, DataType, CompareType, AllocType>::end();
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline typename MultiMap<KeyType, DataType, CompareType, AllocType>::ReverseIterator MultiMap<KeyType, DataType, CompareType, AllocType>::rbegin( void )
{
  return std::multimap<KeyType, DataType, CompareType, AllocType>::rbegin();
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline typename MultiMap<KeyType, DataType, CompareType, AllocType>::ReverseIterator MultiMap<KeyType, DataType, CompareType, AllocType>::rend( void )
{
  return std::multimap<KeyType, DataType, CompareType, AllocType>::rend();
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline bool MultiMap<KeyType, DataType, CompareType, AllocType>::contains( KeyType const &key ) const
{
  return find(key) != MultiMap::end();
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline DataType const &MultiMap<KeyType, DataType, CompareType, AllocType>::operator[]( KeyType const &key ) const
{
  static DataType null;
  ConstIterator i = find( key );
  return (i != MultiMap::end()) ? i.val() : null;
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline DataType &MultiMap<KeyType, DataType, CompareType, AllocType>::operator[]( KeyType const &key )
{
  Iterator i = find( key );
  return (i != MultiMap::end()) ? i.val() : insert( key );
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline DataType &MultiMap<KeyType, DataType, CompareType, AllocType>::insert( KeyType const &key, DataType const &val )
{
  // To preserve ordering
  return std::multimap<KeyType, DataType, CompareType, AllocType>::insert(
           std::multimap<KeyType, DataType, CompareType, AllocType>::upper_bound(key),
           std::pair<KeyType, DataType>( key, val )
         )->second;
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline DataType &MultiMap<KeyType, DataType, CompareType, AllocType>::insert( KeyType const &key )
{
  return insert( key, DataType() );
}

}

#endif
