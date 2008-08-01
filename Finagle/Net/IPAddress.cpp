/*!
** \file IPAddress.cpp
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

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#include <sstream>
#include "Finagle/MemTrace.h"
#include "IPAddress.h"

using namespace std;
using namespace Finagle;

/*! \class Finagle::IPAddress
** \brief Represents an IPv4 network address.
*/

const IPAddress IPAddress::nil;
const IPAddress IPAddress::local( 127, 0, 0, 1 );

/*! \brief Returns the IP address.
**
** If a host name was specified, this will attempt a reverse-DNS lookup of the
** IP.  Returns \c 0 if the host name doesn't resolve.
** \note This will effect the results of a later call to host(), as the DNS
** forward-reverse lookup may not give the same result as the host name specified.
*/
unsigned IPAddress::ip( void ) const
{
  if ( _ip || _name.empty() )
    return _ip;

  // No IP.  Convert IP from host IP string.
  unsigned ip = inet_addr( _name );
  if ( ip == INADDR_NONE ) {
    // Host was a name not IP.  Do DNS lookup.
    ip = 0;
    hostent *HostEntry = gethostbyname( _name );
    if ( HostEntry )
      ip = *((unsigned long *) HostEntry->h_addr_list[0]);
  }

  // Clear host IP, so that we can do a reverse lookup later.
  if ( ip ) {
    _ip = ntohl(ip);
    _name.clear();
  }

  return _ip;
}


//! Returns the IP address, as a string, in dotted-decimal form.
String IPAddress::ipStr( void ) const
{
  in_addr Addr;
  Addr.s_addr = htonl( ip() );
  return String( inet_ntoa( Addr ) );
}


/*! \brief Returns the host name that corresponds to this address.
**
** If an IP address was specified, this will attempt a DNS lookup of the
** hostname.  If the host name doesn't resolve, returns the IP address in
** dotted-decimal string form.
*/
String IPAddress::name( void ) const
{
  if ( !_name.empty() )
    return _name;

  // No hostname.  Do reverse-DNS lookup.
  unsigned ip = htonl( _ip );
  hostent *hostEntry = gethostbyaddr( (const char *) &ip, sizeof(ip), AF_INET );
  if ( hostEntry ) {
    _name = hostEntry->h_name;
    return _name;
  }

  // Couldn't find hostname.  Return formatted IP address
  return ipStr();
}

