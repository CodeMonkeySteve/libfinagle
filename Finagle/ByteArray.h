/*!
** \file ByteArray.h
** \author Steve Sloan <steve@finagle.org>
** \date Thu Aug 19 2004
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

#ifndef FINAGLE_BYTEARRAY_H
#define FINAGLE_BYTEARRAY_H

#include <cstdlib>
#include <iostream>

namespace Finagle {

/*! \brief Statically-sized array of bytes
**
** This template class provides an array of bytes of length \a N.
*/

template <unsigned N>
class ByteArray {
public:
  typedef unsigned char Byte;

public:
  ByteArray( void );
  ByteArray( ByteArray const &That );

  ByteArray &operator =( ByteArray const &That );

  bool operator ==( ByteArray const &That );
  bool operator !=( ByteArray const &That );

  unsigned size( void ) const;

  operator Byte const *( void ) const {  return( Bytes );  }
  operator Byte *( void ) {  return( Bytes );  }

  Byte operator[]( unsigned Index ) const;
  Byte &operator[]( unsigned Index );

protected:
  Byte Bytes[N];
};

// INLINE/TEMPLATE IMPLEMENTATINO *********************************************

template <unsigned N>
inline ByteArray<N>::ByteArray( void )
{
}

template <unsigned N>
inline ByteArray<N>::ByteArray( ByteArray const &That )
{
  *this = That;
}

template <unsigned N>
inline ByteArray<N> &ByteArray<N>::operator =( ByteArray<N> const &That )
{
  memcpy( Bytes, That.Bytes, N );
  return *this;
}

template <unsigned N>
inline bool ByteArray<N>::operator ==( ByteArray const &That )
{
  return( memcmp( Bytes, That.Bytes, N ) == 0 );
}

template <unsigned N>
inline bool ByteArray<N>::operator !=( ByteArray const &That )
{
  return( !ByteArray::operator ==( That ) );
}

template <unsigned N>
inline unsigned ByteArray<N>::size( void ) const
{
  return( N );
}

template <unsigned N>
inline typename ByteArray<N>::Byte ByteArray<N>::operator[]( unsigned Index ) const
{
  return( (Index < N) ? Bytes[Index] : 0 );
}

template <unsigned N>
inline typename ByteArray<N>::Byte &ByteArray<N>::operator[]( unsigned Index )
{
  static Byte null;
  return( (Index < N) ? Bytes[Index] : null );
}

//! Extracts a byte array stored in hex form
template <unsigned N>
std::istream &operator >>( std::istream &in, ByteArray<N> &Array )
{
  char Buff[N * 2];

  if ( !in.read( Buff, sizeof( Buff ) ) )
    return in;

  char *b = Buff;
  for ( unsigned i = 0; i < N; ++i ) {
    typename ByteArray<N>::Byte v;

    if ( (*b >= 'a') && (*b <= 'f') )
      v = *b - 'a' + 10;
    else
    if ( (*b >= 'A') && (*b <= 'F') )
      v = *b - 'A' + 10;
    else
    if ( (*b >= '0') && (*b <= '9') )
      v = *b - '0';
    else {
      in.setstate( in.rdstate() | std::ios_base::failbit );
      return in;
    }
    ++b;
    v <<= 4;

    if ( (*b >= 'a') && (*b <= 'f') )
      v |= *b - 'a' + 10;
    else
    if ( (*b >= 'A') && (*b <= 'F') )
      v |= *b - 'A' + 10;
    else
    if ( (*b >= '0') && (*b <= '9') )
      v |= *b - '0';
    else {
      in.setstate( in.rdstate() | std::ios_base::failbit );
      return in;
    }
    ++b;

    Array[i] = v;
  }

  return in;
}

//! Inserts a byte array stored in hex form
template <unsigned N>
std::ostream &operator <<( std::ostream &out, ByteArray<N> const &Array )
{
  for ( unsigned i = 0; i < N; ++i ) {
    typename ByteArray<N>::Byte b = Array[i];

    char c;
    c = ((b >> 4) & 0x0F);
    out << char( (c < 10) ? ('0' + c) : ('a' + c - 10) );

    c = (b & 0x0F);
    out << char( (c < 10) ? ('0' + c) : ('a' + c - 10) );
  }

  return out;
}

}

#endif
