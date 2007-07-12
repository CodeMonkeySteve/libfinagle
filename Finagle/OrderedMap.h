/*!
** \file OrderedMap.h
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

#ifndef FINAGLE_ORDEREDMAP_H
#define FINAGLE_ORDEREDMAP_H

#include <Finagle/List.h>
#include <Finagle/MapIterator.h>

namespace Finagle {

/*! \brief Provides an ordered map
**
** This class behaves similarly to the Map class, except the elements are stored
** in order of insertion.  Normal maps hash the elements, and the order is lost.
*/
template <typename KeyType, typename DataType>
class OrderedMap : public List<std::pair<KeyType, DataType> > {
public:
  typedef MapIterator<typename List<std::pair<KeyType, DataType> >::Iterator> Iterator;
  typedef MapConstIterator<typename List<std::pair<KeyType, DataType> >::ConstIterator> ConstIterator;

public:
  OrderedMap( void );

  bool contains( KeyType const &Key ) const;
  DataType const &operator[]( KeyType const &Key ) const;
  DataType &operator[]( KeyType const &Key );
  DataType &insert( KeyType const &Key, DataType const &Data );
  DataType &insert( KeyType const &Key );

  DataType const &front( void ) const;
  DataType const &back( void ) const;
  DataType &front( void );
  DataType &back( void );

  ConstIterator find( KeyType const &Key, ConstIterator Start ) const;
  Iterator find( KeyType const &Key, Iterator Start );

  ConstIterator find( KeyType const &Key ) const;
  Iterator find( KeyType const &Key );

  Iterator begin( void );
  ConstIterator begin( void ) const;
};


/*! \brief Provides an ordered multi-map
**
** This class behaves similarly to the STL \c multimap class, except the elements
** are stored in order of insertion.  Normal multimaps hash the elements, and the
** order is lost.
**
** This class differs from OrderedMap in that a single key can have multiple
** values.
*/

template <typename KeyType, typename DataType>
class OrderedMultiMap : public OrderedMap<KeyType, DataType> {
public:
  typedef typename OrderedMap<KeyType, DataType>::Iterator Iterator;
  typedef typename OrderedMap<KeyType, DataType>::ConstIterator ConstIterator;

public:
  OrderedMultiMap( void );

  unsigned count( KeyType const &Key ) const;
  DataType &insert( KeyType const &Key, DataType const &Data );
  DataType &insert( KeyType const &Key );
};


// INLINE/TEMPLATE IMPLEMENTATION *********************************************

//! Constructor
template <typename KeyType, typename DataType>
inline OrderedMap<KeyType, DataType>::OrderedMap( void )
{
}

//! Returns \c true if an element with key \a Key exists in the map.
template <typename KeyType, typename DataType>
inline bool OrderedMap<KeyType, DataType>::contains( KeyType const &Key ) const
{
  return( find( Key ) != OrderedMap<KeyType, DataType>::end() );
}

//! Returns the first element with key \a Key.
template <typename KeyType, typename DataType>
inline DataType const &OrderedMap<KeyType, DataType>::operator[]( KeyType const &Key ) const
{
  static DataType null;
  ConstIterator i = find( Key );
  return( (i != OrderedMap<KeyType, DataType>::end()) ? i : null );
}

//! Returns the data for the first element with key \a Key.
template <typename KeyType, typename DataType>
inline DataType &OrderedMap<KeyType, DataType>::operator[]( KeyType const &Key )
{
  Iterator i = find( Key );
  if ( i != OrderedMap<KeyType, DataType>::end() )
    return( i );

  push_back( std::pair<KeyType, DataType>( Key, DataType() ) );
  return( back() );
}

/*! \brief Inserts an element \a Data with key \a Key at then end of the map.
**
** If an element with \a Key already exists in the map, replaces it with \a Data.
** Returns a reference to the element data.
*/
template <typename KeyType, typename DataType>
inline DataType &OrderedMap<KeyType, DataType>::insert( KeyType const &Key, DataType const &Data )
{
  Iterator i = find( Key );
  if ( i != OrderedMap<KeyType, DataType>::end() )
    return( *i = Data );

  push_back( std::pair<KeyType, DataType>( Key, Data ) );
  return( back() );
}

/*! \brief Inserts an empty element with key \a Key at then end of the map.
**
** If an element with \a Key already exists in the map, replaces it with \a Data.
** Returns a reference to the element data.
*/
template <typename KeyType, typename DataType>
inline DataType &OrderedMap<KeyType, DataType>::insert( KeyType const &Key )
{
  return( operator[]( Key ) );
}

//! Returns the first element in the map.
template <typename KeyType, typename DataType>
DataType const &OrderedMap<KeyType, DataType>::front( void ) const
{
  return( List<std::pair<KeyType, DataType> >::front().second );
}

//! Returns the last element in the map.
template <typename KeyType, typename DataType>
DataType const &OrderedMap<KeyType, DataType>::back( void ) const
{
  return( List<std::pair<KeyType, DataType> >::back().second );
}

//! Returns the first element in the map.
template <typename KeyType, typename DataType>
DataType &OrderedMap<KeyType, DataType>::front( void )
{
  return( List<std::pair<KeyType, DataType> >::front().second );
}

//! Returns the last element in the map.
template <typename KeyType, typename DataType>
DataType &OrderedMap<KeyType, DataType>::back( void )
{
  return( List<std::pair<KeyType, DataType> >::back().second );
}

/*! \brief Searches for the first element with key \a Key, starting at position \a Start.
**
** If no element matches, returns \a Start.
*/
template <typename KeyType, typename DataType>
typename OrderedMap<KeyType, DataType>::ConstIterator
           OrderedMap<KeyType, DataType>::find( KeyType const &Key, ConstIterator Start ) const
{
  for ( ; Start != OrderedMap<KeyType, DataType>::end(); ++Start )
    if ( Start.key() == Key )
      break;

  return( Start );
}

/*! \brief Searches for the first element with key \a Key, starting at position \a Start.
**
** Returns an iterator to the element, or \a Start, if no matching element was found.
*/
template <typename KeyType, typename DataType>
typename OrderedMap<KeyType, DataType>::Iterator
           OrderedMap<KeyType, DataType>::find( KeyType const &Key, Iterator Start )
{
  for ( ; Start != OrderedMap<KeyType, DataType>::end(); ++Start )
    if ( Start.key() == Key )
      break;

  return( Start );
}

/*! \brief Searches for the first element with key \a Key.
**
** Returns an iterator to the element, or \a Start, if no matching element was found.
*/
template <typename KeyType, typename DataType>
inline typename OrderedMap<KeyType, DataType>::ConstIterator
                  OrderedMap<KeyType, DataType>::find( KeyType const &Key ) const
{
  return( find( Key, OrderedMap<KeyType, DataType>::begin() ) );
}

/*! \brief Searches for an element with key \a Key.
**
** Returns an iterator to the element, or \a Start, if no matching element was found.
*/
template <typename KeyType, typename DataType>
inline typename OrderedMap<KeyType, DataType>::Iterator
                  OrderedMap<KeyType, DataType>::find( KeyType const &Key )
{
  return( find( Key, OrderedMap<KeyType, DataType>::begin() ) );
}


template <typename KeyType, typename DataType>
inline typename OrderedMap<KeyType, DataType>::Iterator OrderedMap<KeyType, DataType>::begin( void )
{
  return( Iterator( List<std::pair<KeyType, DataType> >::begin() ) );
}

template <typename KeyType, typename DataType>
inline typename OrderedMap<KeyType, DataType>::ConstIterator OrderedMap<KeyType, DataType>::begin( void ) const
{
  return( ConstIterator( List<std::pair<KeyType, DataType> >::begin() ) );
}

//! Constructor
template <typename KeyType, typename DataType>
inline OrderedMultiMap<KeyType, DataType>::OrderedMultiMap( void )
{
}

//! Returns the number of elements with key \a Key.
template <typename KeyType, typename DataType>
unsigned OrderedMultiMap<KeyType, DataType>::count( KeyType const &Key ) const
{
  unsigned Count = 0;
  for ( ConstIterator i = OrderedMap<KeyType, DataType>::begin(); i != OrderedMap<KeyType, DataType>::end(); ++i )
    if ( i.key() == Key )
      Count++;

  return( Count );
}

/*
template <typename KeyType, typename DataType>
DataType const &OrderedMultiMap<KeyType, DataType>::operator[]( KeyType const &Key ) const
{
  return( Map<KeyType, DataType>::operator[]( Key ) );
}

template <typename KeyType, typename DataType>
DataType &OrderedMultiMap<KeyType, DataType>::operator[]( KeyType const &Key )
{
  return( Map<KeyType, DataType>::operator[]( Key ) );
}
*/

//! Adds an element to the end of the map.
template <typename KeyType, typename DataType>
DataType &OrderedMultiMap<KeyType, DataType>::insert( KeyType const &Key, DataType const &Data )
{
  push_back( std::pair<KeyType, DataType>( Key, Data ) );
  return( OrderedMap<KeyType, DataType>::back() );
}

//! Adds an element, with empty data, to the end of the map.
template <typename KeyType, typename DataType>
DataType &OrderedMultiMap<KeyType, DataType>::insert( KeyType const &Key )
{
  push_back( std::pair<KeyType, DataType>( Key, DataType() ) );
  return( OrderedMap<KeyType, DataType>::back() );
}

}

#endif
