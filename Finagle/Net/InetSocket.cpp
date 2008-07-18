/*!
** \file InetSocket.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Thu Jul 17 2008
** Copyright (C) 2008 by Steve Sloan
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

#include "InetSocket.h"

using namespace Finagle;

/*!
** \class Finagle::InetSocket
** \brief Provides an Internet domain socket
*/

/*!
** \class Finagle::InetSocket::Addr
** \brief Internet domain address (host and port)
*/


InetSocket::Addr::Addr( int sock )
{
  socklen_t len = domainAddrLen();
  if ( getpeername( sock, (sockaddr *) &_domainAddr, &len ) != 0 )
    throw SystemEx( "Unable to read peer name from socket" );
}

InetSocket::Addr::Addr( String const &spec )
{
  memset( &_domainAddr, 0, sizeof(_domainAddr) );
  _domainAddr.sin_family = domainFamily();

  String::Array parts = spec.split(':');
  if ( parts.size() > 2 )
    throw Exception( String("Bad inet address: ") + spec );

  InetSocket::Addr::host( IPAddress( parts.front() ) );
  if ( parts.size() > 1 )
    InetSocket::Addr::port( parts[1].as<unsigned short>() );
}

InetSocket::Addr::Addr( sockaddr const *addr, socklen_t len )
{
  if ( len > sizeof(_domainAddr) )
    throw std::length_error( "Socket address is too large" );

  memset( &_domainAddr, 0, sizeof(_domainAddr) );
  memcpy( &_domainAddr, addr, len );
  _host = ntohl( ((sockaddr_in *) addr)->sin_addr.s_addr );
  _port = ntohs( ((sockaddr_in *) addr)->sin_port );
}
