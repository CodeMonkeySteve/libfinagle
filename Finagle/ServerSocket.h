/*!
** \file ServerSocket.h
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

#ifndef FINAGLE_SERVERSOCKET_H
#define FINAGLE_SERVERSOCKET_H

#include <Finagle/Socket.h>

namespace Finagle {

//! Template class to provide a server-side network socket
template <typename SockType>
class ServerSocket : public SockType, public has_slots<> {
public:
  typedef ObjectRef<ServerSocket<SockType> > Ref;
  typedef typename SockType::Addr Addr;

public:
  ServerSocket( void );
  ServerSocket( Addr const &addr, int sockDesc = -1 );
 ~ServerSocket( void ) {}

public:
  Finagle::signal1<Finagle::Socket::Ref> connected;

protected:
  void onReadable( void );
};

// TEMPLATE/INLINE IMPLEMENTATION *************************************************************************************************

template <typename SockType>
inline ServerSocket<SockType>::ServerSocket( void )
: SockType()
{
  SockType::readable.connect( this, &ServerSocket<SockType>::onReadable );
}

template <typename SockType>
inline ServerSocket<SockType>::ServerSocket( Addr const &addr, int sockDesc )
: SockType( addr, sockDesc )
{
  SockType::readable.connect( this, &ServerSocket<SockType>::onReadable );
}


template <typename SockType>
void ServerSocket<SockType>::onReadable( void )
{
  socklen_t len = SockType::addr().domainAddrLen();
  sockaddr *newSockAddr = (sockaddr *) new char[len];
  int sockDesc = ::accept( SockType::fd(), newSockAddr, &len );

  if ( sockDesc == -1 ) {
    SockType::_error = (SystemEx::sysErrCode() == EWOULDBLOCK) ? 0 : SystemEx::sysErrCode();
    return;
  }

  Socket::Ref sock;
  if ( len > sizeof(((sockaddr *) 0)->sa_family) ) {
    sock = new SockType( Addr( newSockAddr, len ), sockDesc );
  } else {
    // Unix-domain sockets (among others?) don't provide an address (just family), so in these cases assume the
    // listening socket's address
    sock = new SockType( SockType::addr(), sockDesc );
  }

  delete [] (char *) newSockAddr;
  connected( sock );
}

}

#endif
