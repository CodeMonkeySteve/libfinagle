/*!
** \file MapIterator.h
** \author Steve Sloan <steve@finagle.org>
** \date Tue Nov. 30 2004
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

#ifndef FINAGLE_MAPITERATOR_H
#define FINAGLE_MAPITERATOR_H

namespace Finagle {

template <typename Base>
class MapConstIterator : public Base {
public:
  typedef Base IteratorType;
  typedef typename Base::value_type::first_type KeyType;
  typedef typename Base::value_type::second_type ValType;

  typedef typename Base::value_type::second_type value_type;

public:
  MapConstIterator( void ) {}
  MapConstIterator( Base const &Init ) : Base( Init ) {}

  KeyType const &key( void ) const;
  ValType const &val( void ) const;

  ValType const &operator *( void ) const;
  ValType const *operator ->( void ) const;

  operator ValType const &( void ) const;
  operator ValType const *( void ) const;

  MapConstIterator &operator ++( void );
  MapConstIterator &operator --( void );

  MapConstIterator operator ++( int );
  MapConstIterator operator --( int );
};


template <typename Base>
class MapIterator : public MapConstIterator<Base> {
public:
  typedef Base IteratorType;
  typedef typename MapConstIterator<Base>::KeyType KeyType;
  typedef typename MapConstIterator<Base>::ValType ValType;

  typedef typename MapConstIterator<Base>::ValType value_type;

public:
  MapIterator( void ) {}
  MapIterator( Base const &Init ) : MapConstIterator<Base>( Init ) {}

  KeyType &key( void );
  ValType &val( void );

  ValType &operator *( void );
  ValType *operator ->( void );

  operator ValType &( void );
  operator ValType *( void );
};

// INLINE IMPLEMENTATION ******************************************************

template <typename Base>
inline typename MapConstIterator<Base>::KeyType const &MapConstIterator<Base>::key( void ) const
{
  return (Base::operator *()).first;
}

template <typename Base>
inline typename MapConstIterator<Base>::ValType const &MapConstIterator<Base>::val( void ) const
{
  return (Base::operator *()).second;
}

template <typename Base>
inline typename MapConstIterator<Base>::ValType const &MapConstIterator<Base>::operator *( void ) const
{
  return (Base::operator *()).second;
}

template <typename Base>
inline typename MapConstIterator<Base>::ValType const *MapConstIterator<Base>::operator ->( void ) const
{
  return &(Base::operator *()).second;
}

template <typename Base>
inline MapConstIterator<Base>::operator typename MapConstIterator<Base>::ValType const &( void ) const
{
  return (Base::operator *()).second;
}

template <typename Base>
inline MapConstIterator<Base>::operator typename MapConstIterator<Base>::ValType const *( void ) const
{
  return (Base::operator *()).second;
}

template <typename Base>
inline MapConstIterator<Base> &MapConstIterator<Base>::operator ++( void )
{
  Base::operator++();
  return *this;
}

template <typename Base>
inline MapConstIterator<Base> &MapConstIterator<Base>::operator --( void )
{
  Base::operator--();
  return *this;
}

template <typename Base>
inline MapConstIterator<Base> MapConstIterator<Base>::operator ++( int )
{
  return MapConstIterator<Base>( Base::operator++( 0 ) );
}

template <typename Base>
inline MapConstIterator<Base> MapConstIterator<Base>::operator --( int )
{
  return MapConstIterator<Base>( Base::operator--( 0 ) );
}


template <typename Base>
inline typename MapIterator<Base>::KeyType &MapIterator<Base>::key( void )
{
  return (Base::operator *()).first;
}

template <typename Base>
inline typename MapIterator<Base>::ValType &MapIterator<Base>::val( void )
{
  return (Base::operator *()).second;
}

template <typename Base>
inline typename MapIterator<Base>::ValType &MapIterator<Base>::operator *( void )
{
  return (Base::operator *()).second;
}

template <typename Base>
inline typename MapIterator<Base>::ValType *MapIterator<Base>::operator ->( void )
{
  return &(Base::operator *()).second;
}

template <typename Base>
inline MapIterator<Base>::operator typename MapIterator<Base>::ValType &( void )
{
  return (Base::operator *()).second;
}

template <typename Base>
inline MapIterator<Base>::operator typename MapIterator<Base>::ValType *( void )
{
  return (Base::operator *()).second;
}


};


#endif
