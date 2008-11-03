/*!
** \file Map.h
** \author Steve Sloan <steve@finagle.org>
** \date Tue Apr. 5 2004
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

#ifndef FINAGLE_MAP_H
#define FINAGLE_MAP_H

#include <map>
#include <Finagle/MapIterator.h>
#include <Finagle/ObjectPtr.h>

namespace Finagle {

//! \brief Wrapper for the STL \c map class

template <typename KeyType, typename DataType, typename CompareType = std::less<KeyType>, typename AllocType = std::allocator<std::pair<const KeyType, DataType> > >
class Map : public std::map<KeyType, DataType, CompareType, AllocType> {
public:
  typedef MapIterator<typename std::map<KeyType, DataType, CompareType, AllocType>::iterator> Iterator;
  typedef MapConstIterator<typename std::map<KeyType, DataType, CompareType, AllocType>::const_iterator> ConstIterator;

public:
  Map( void );

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

//! \brief Wrapper for the STL \c map class, using ObjectPtr values

template <typename KeyType, typename DataType, typename CompareType = std::less<KeyType>, typename AllocType = std::allocator<std::pair<const KeyType, DataType> > >
class ObjectMap : public Map<KeyType, ObjectPtr<DataType>, CompareType, AllocType> {
public:
  typedef ObjectPtrConstIterator<typename Map<KeyType, ObjectPtr<DataType>, CompareType, AllocType>::ConstIterator> ConstIterator;
  typedef ObjectPtrIterator<typename Map<KeyType, ObjectPtr<DataType>, CompareType, AllocType>::Iterator> Iterator;

  ObjectMap( void );

  ObjectPtr<DataType> insert( KeyType const &Key, DataType const &Data );
  ObjectPtr<DataType> insert( KeyType const &Key );
};

// INLINE/TEMPLATE IMPLEMENTATION *********************************************

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline Map<KeyType, DataType, CompareType, AllocType>::Map( void )
{}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline typename Map<KeyType, DataType, CompareType, AllocType>::ConstIterator Map<KeyType, DataType, CompareType, AllocType>::begin( void ) const
{
  return std::map<KeyType, DataType, CompareType, AllocType>::begin();
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline typename Map<KeyType, DataType, CompareType, AllocType>::ConstIterator Map<KeyType, DataType, CompareType, AllocType>::end( void ) const
{
  return std::map<KeyType, DataType, CompareType, AllocType>::end();
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline typename Map<KeyType, DataType, CompareType, AllocType>::Iterator Map<KeyType, DataType, CompareType, AllocType>::begin( void )
{
  return std::map<KeyType, DataType, CompareType, AllocType>::begin();
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline typename Map<KeyType, DataType, CompareType, AllocType>::Iterator Map<KeyType, DataType, CompareType, AllocType>::end( void )
{
  return std::map<KeyType, DataType, CompareType, AllocType>::end();
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline bool Map<KeyType, DataType, CompareType, AllocType>::contains( KeyType const &Key ) const
{
  return find(Key) != Map::end();
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline DataType const &Map<KeyType, DataType, CompareType, AllocType>::operator[]( KeyType const &Key ) const
{
  static DataType null;
  ConstIterator i = find( Key );
  return (i != std::map<KeyType, DataType, CompareType, AllocType>::end()) ? i.val() : null;
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline DataType &Map<KeyType, DataType, CompareType, AllocType>::operator[]( KeyType const &Key )
{
  return std::map<KeyType, DataType, CompareType, AllocType>::operator[]( Key );
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline DataType &Map<KeyType, DataType, CompareType, AllocType>::insert( KeyType const &Key, DataType const &Data )
{
  return std::map<KeyType, DataType, CompareType, AllocType>::insert( std::pair<KeyType, DataType>( Key, Data ) ).first->second;
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline DataType &Map<KeyType, DataType, CompareType, AllocType>::insert( KeyType const &Key )
{
  return insert( Key, DataType() );
}


template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline ObjectMap<KeyType, DataType, CompareType, AllocType>::ObjectMap( void )
{}


template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline ObjectPtr<DataType> ObjectMap<KeyType, DataType, CompareType, AllocType>::insert( KeyType const &Key, DataType const &Data )
{
  return Map<KeyType, DataType, CompareType, AllocType>::insert( std::pair<KeyType, DataType>( Key, Data ) ).first->second;
}

template <typename KeyType, typename DataType, typename CompareType, typename AllocType>
inline ObjectPtr<DataType> ObjectMap<KeyType, DataType, CompareType, AllocType>::insert( KeyType const &Key )
{
  return insert( Key, new DataType() );
}

}

#endif
