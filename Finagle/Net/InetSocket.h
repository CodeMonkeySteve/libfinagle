/*!
** \file InetSocket.h
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

#ifndef FINAGLE_NET_INETSOCKET_H
#define FINAGLE_NET_INETSOCKET_H

#include <arpa/inet.h>
#include <Finagle/Net/Socket.h>

namespace Finagle {

class InetSocket : public Socket {
public:

  class Addr : public Socket::Addr {
  public:
    Addr( void );
    Addr( int sock );
    Addr( String const &spec );
    Addr( IPAddress const &host, unsigned short port );
    Addr( sockaddr const *addr, socklen_t len );

    IPAddress const &host( void ) const;
    void host( IPAddress const &host );

    unsigned short port( void ) const;
    void port( unsigned short port );

    unsigned domainFamily( void ) const;
    sockaddr const &domainAddr( void ) const;
    socklen_t domainAddrLen( void ) const;
    operator String( void ) const;

  protected:
    IPAddress _host;
    unsigned short _port;
    sockaddr_in _domainAddr;
  };

public:
  InetSocket( void );
  InetSocket( Addr const &addr, int sockDesc = -1  );

  Addr const &addr( void ) const;
  void addr( Addr const &addr );

protected:
  Addr _addr;
};

// INLINE IMPLEMENTATION ******************************************************

inline InetSocket::Addr::Addr( void )
{
  memset( &_domainAddr, 0, sizeof(_domainAddr) );
  _domainAddr.sin_family = domainFamily();
}

inline InetSocket::Addr::Addr( IPAddress const &host, unsigned short port )
{
  memset( &_domainAddr, 0, sizeof(_domainAddr) );
  _domainAddr.sin_family = domainFamily();
  InetSocket::Addr::host( host );
  InetSocket::Addr::port( port );
}


//! \note use IPAddress::null to listen on all interfaces.
inline IPAddress const &InetSocket::Addr::host( void ) const
{
  return _host;
}

inline void InetSocket::Addr::host( IPAddress const &host )
{
  _host = host;
  _domainAddr.sin_addr.s_addr = (host == IPAddress::nil) ? INADDR_ANY : htonl(host.ip());
}

inline unsigned short InetSocket::Addr::port( void ) const
{
  return _port;
}

inline void InetSocket::Addr::port( unsigned short port )
{
  _port = port;
  _domainAddr.sin_port = htons(port);
}

inline unsigned InetSocket::Addr::domainFamily( void ) const
{
  return AF_INET;
}

inline sockaddr const &InetSocket::Addr::domainAddr( void ) const
{
  return (sockaddr &) _domainAddr;
}

inline unsigned InetSocket::Addr::domainAddrLen( void ) const
{
  return sizeof(_domainAddr);
}

inline InetSocket::Addr::operator String( void ) const
{
  return _host.name() + ":" + String(_port);
}


inline InetSocket::InetSocket( void )
{}

inline InetSocket::InetSocket( Addr const &addr, int sockDesc )
: Socket( sockDesc ), _addr( addr )
{}

inline InetSocket::Addr const &InetSocket::addr( void ) const
{
  return _addr;
}

inline void InetSocket::addr( Addr const &addr )
{
  disconnect();
  _addr = addr;
}

}

#endif
