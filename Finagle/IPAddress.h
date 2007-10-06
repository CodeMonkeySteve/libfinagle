/*!
** \file IPAddress.h
** \author Steve Sloan <steve@finagle.org>
** \date Sat Nov 1 2003
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

#ifndef FINAGLE_IPADDRESS_H
#define FINAGLE_IPADDRESS_H

#include <ostream>
#include <Finagle/TextString.h>

namespace Finagle {

class IPAddress {
public:
  typedef unsigned char Byte;

public:
  IPAddress( void );
  explicit IPAddress( String const &name );
  IPAddress( unsigned ip );
  IPAddress( Byte IPA, Byte IPB, Byte IPC, Byte IPD );

  bool operator ==( const IPAddress &that ) const;
  bool operator !=( const IPAddress &that ) const;

  String name( void ) const;
  unsigned ip( void ) const;

  String ipStr( void ) const;

protected:
  mutable String _name;
  mutable unsigned _ip;

  friend std::ostream &operator <<( std::ostream &out, IPAddress const &host );

public:
  static const IPAddress nil, local;
};

// INLINE IMPLEMENTATION ******************************************************

//! Initializes with an invalid address (i.e. \c 0).
inline IPAddress::IPAddress( void )
: _ip(0)
{}

//! Initializes with a \a Host name or address in string form.
inline IPAddress::IPAddress( String const &name )
: _name(name), _ip(0)
{}

//! Initializes with an IP address, in the form \a IPA.IPB.IPC.IPD.
inline IPAddress::IPAddress( Byte IPA, Byte IPB, Byte IPC, Byte IPD )
: _ip( ((unsigned) IPA << 24) | ((unsigned) IPB << 16) | ((unsigned) IPC << 8) | ((unsigned) IPD) )
{}

//! Initializes with a packed IP address in host byte-order.
inline IPAddress::IPAddress( unsigned ip )
: _ip(ip)
{}

//! Returns \c true if the address is the same as \a that.
inline bool IPAddress::operator ==( const IPAddress &that ) const
{
  return ip() == that.ip();
}

//! Returns \c true if the address is different from \a that.
inline bool IPAddress::operator !=( const IPAddress &that ) const
{
  return !(operator ==( that ));
}

//! Writes the \a host name to the \a out stream.
inline std::ostream &operator <<( std::ostream &out, IPAddress const &host )
{
  return out << host.name();
}

}

#endif
