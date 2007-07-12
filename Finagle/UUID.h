/*!
** \file UUID.h
** \author Steve Sloan <steve@finagle.org>
** \date Mon Nov 1 2004
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

#ifndef FINAGLE_UUID_H
#define FINAGLE_UUID_H

#include <ostream>
#include <Finagle/TextString.h>

namespace Finagle {

class UUID {
  typedef unsigned long  L;
  typedef unsigned short W;
  typedef unsigned char  B;

public:
  UUID( void );
  UUID( L l, W w1, W w2, B b1, B b2, B b3, B b4, B b5, B b6, B b7, B b8 );

  bool isNull( void ) const;
  operator String( void ) const;

protected:
  unsigned long  Data1;
  unsigned short Data2;
  unsigned short Data3;
  unsigned char  Data4[8];
};

std::ostream &operator <<( std::ostream &out, UUID const &ID );

// INLINE IMPLEMENTATION ******************************************************

inline UUID::UUID( L l, W w1, W w2, B b1, B b2, B b3, B b4, B b5, B b6, B b7, B b8 )
: Data1( l ), Data2( w1 ), Data3( w2 )
{
  Data4[0] = b1;  Data4[1] = b2;  Data4[2] = b3;  Data4[3] = b4;
  Data4[4] = b5;  Data4[5] = b6;  Data4[6] = b7;  Data4[7] = b8;
}

inline UUID::UUID( void )
: Data1( 0 ), Data2( 0 ), Data3( 0 )
{
  Data4[0] = Data4[1] = Data4[2] = Data4[3] = Data4[4] = Data4[5] = Data4[6] = Data4[7] = 0;
}

inline bool UUID::isNull( void ) const
{
  return( !Data1 && !Data2 && !Data3 && !Data4[0] && !Data4[1] && !Data4[2] &&
          !Data4[3] && !Data4[4] && !Data4[5] && !Data4[6] && !Data4[7] );
}


inline UUID::operator String( void ) const
{
  return( String::format( "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
          Data1, Data2, Data3, Data4[0], Data4[1], Data4[2], Data4[3], Data4[4], Data4[5], Data4[6], Data4[7] ) );
}

inline std::ostream &operator <<( std::ostream &out, UUID const &ID )
{
  return out << (String) ID;
}

};

#endif
