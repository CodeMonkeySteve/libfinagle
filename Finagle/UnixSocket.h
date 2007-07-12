/*!
** \file UnixSocket.h
** \author Steve Sloan <steve@finagle.org>
** \date Sat Dec 16 2006
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

#ifndef FINAGLE_UNIXSOXKET_H
#define FINAGLE_UNIXSOXKET_H

#include <sys/un.h>
#include <Finagle/Socket.h>

namespace Finagle {

class UnixSocket : public Socket {
public:
  class Addr : public Socket::Addr {
  public:
    Addr( void );
    Addr( FilePath const &path );
    Addr( sockaddr const *addr, socklen_t len );

    FilePath const &path( void ) const;
    void path( FilePath const &path );

    unsigned domainFamily( void ) const;
    sockaddr const &domainAddr( void ) const;
    socklen_t domainAddrLen( void ) const;
    operator String( void ) const;

  protected:
    FilePath _path;
    sockaddr_un _domainAddr;
  };

public:
  UnixSocket( void ) {}

  UnixSocket( Addr const &addr, int sockDesc = -1, Socket const *src = 0  )
  : Socket( sockDesc, src ), _addr( addr ) {}

  Addr const &addr( void ) const;
  void addr( Addr const &addr );

protected:
  Addr _addr;
};

// INLINE IMPLEMENTATION ******************************************************

inline UnixSocket::Addr::Addr( void )
{
  memset( &_domainAddr, 0, sizeof(_domainAddr) );
  _domainAddr.sun_family = domainFamily();
}

inline UnixSocket::Addr::Addr( FilePath const &path )
{
  memset( &_domainAddr, 0, sizeof(_domainAddr) );
  _domainAddr.sun_family = domainFamily();
  UnixSocket::Addr::path( path );
}

inline UnixSocket::Addr::Addr( sockaddr const *addr, socklen_t len )
{
  if ( len > sizeof(_domainAddr) )
    throw std::length_error( "Socket address is too large" );

  memset( &_domainAddr, 0, sizeof(_domainAddr) );
  memcpy( &_domainAddr, addr, len );

  if ( len > (sizeof(_domainAddr) - sizeof(_domainAddr.sun_path)) )
    _path = _domainAddr.sun_path;
}

inline FilePath const &UnixSocket::Addr::path( void ) const
{
  return _path;
}


inline unsigned UnixSocket::Addr::domainFamily( void ) const
{
  return AF_UNIX;
}

inline void UnixSocket::Addr::path( FilePath const &path )
{
  _path = path;
  if ( _path.path().size() >= sizeof(_domainAddr.sun_path) )
    throw std::length_error( "Path is too large" );

  memset( &_domainAddr.sun_path, 0, sizeof(_domainAddr.sun_path) );
  strcpy( _domainAddr.sun_path, path.path() );
}

inline sockaddr const &UnixSocket::Addr::domainAddr( void ) const
{
  return (sockaddr &) _domainAddr;
}

inline unsigned UnixSocket::Addr::domainAddrLen( void ) const
{
  return sizeof(_domainAddr);
}

inline UnixSocket::Addr::operator String( void ) const
{
  return _path;
}


inline UnixSocket::Addr const &UnixSocket::addr( void ) const
{
  return _addr;
}

inline void UnixSocket::addr( Addr const &addr )
{
  disconnect();
  _addr = addr;
}

}

// SERVERSOCKET IMPLEMENTATION ****************************************************************************************************

#include <Finagle/File.h>
#include <Finagle/ServerSocket.h>

namespace Finagle {

//! Partial template instantiation to cleanup after UnixSocket
template <>
inline ServerSocket<UnixSocket>::~ServerSocket( void )
{
  File( addr().path() ).erase();
}

}

#endif
