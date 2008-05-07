/*!
** \file Socket.cpp
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
#include <sys/ioctl.h>
#include <unistd.h>

#include "Finagle/AppLog.h"
#include "Finagle/MemTrace.h"

#include "UnixSocket.h"
#include "InetSocket.h"
#include "Socket.h"


using namespace std;
using namespace Finagle;

/*! \class Finagle::Socket
** \brief Base class for a network socket
*/

/*! \class Finagle::Socket::Addr
** \brief Abstract address class
**
** \sa UnixSocket::Addr and INetSocket::Addr.
*/


/*! \brief Creates a socket from an existing file descriptor
** If \a sockDesc is non-0, binds to an existing socket.
*/
Socket::Socket( int sockDesc )
: FileDescWatcher(sockDesc), _error(0)
{
  if ( sockDesc == -1 )
    return;

  setBlocking( true );
  setReceiveBuff( 256 );
  setSendBuff( 256 );
}

Socket::~Socket( void )
{
  disconnect();

  delete [] eback();
  delete [] pbase();
  setg( 0, 0, 0 );
  setp( 0, 0 );
}

/*! \brief Creates a socket from an address specification
** If \a spec names an existing file, a UnixSocket is created.  Otherwise, \a spec is assumed to be an Internet
** Address (i.e. host:port), and an INetSocket is created.
*/
Socket::Ptr Socket::fromSpec( String const &spec )
{
  File sockAddr( spec );
  if ( sockAddr.exists() )
    return new UnixSocket( UnixSocket::Addr( sockAddr ) );

  return new InetSocket( InetSocket::Addr( spec ) );
}

//! Returns the socket's file descriptor.
int Socket::fd( void )
{
  // if we haven't already, create the client socket here
  if ( FileDescWatcher::fd() == -1 ) {
    int res = socket( addr().domainFamily(), SOCK_STREAM, 0 );
    if ( res == -1 ) {
      _error = SystemEx::sysErrCode();
      return -1;
    }
    FileDescWatcher::fd(res);
  }

  return FileDescWatcher::fd();
}


/*! \brief Attempts to connect to the remote host.
** Returns \c true if the connection was successful, \c false otherwise.
*/
bool Socket::connect( void )
{
  disconnect();

  // Start to connect to the server
  if ( ::connect( fd(), &addr().domainAddr(), addr().domainAddrLen() ) == -1 ) {
    _error = SystemEx::sysErrCode();
    disconnect();
    return false;
  }

  setReceiveBuff( 256 );
  setSendBuff( 256 );
  _error = 0;

  return setBlocking( true );
}


//! Disconnects the socket
void Socket::disconnect( void )
{
  if ( FileDescWatcher::fd() != -1 ) {
    close( fd() );
    FileDescWatcher::fd( -1 );
  }
}


//! Attempts to send \a len bytes of \a data to the socket.
int Socket::send( const char *data, unsigned len )
{
  if ( !isConnected() )
    return -1;

  int res = ::send( fd(), data, len, 0 );
  if ( res != -1 )
    return res;

  if ( SystemEx::sysErrCode() == EWOULDBLOCK )
    return 0;

  disconnect();
  if ( (SystemEx::sysErrCode() == ENOTCONN) || (SystemEx::sysErrCode() == ECONNRESET) || (SystemEx::sysErrCode() == EPIPE) ) {
    _error = 0;
  } else {
    _error = SystemEx::sysErrCode();
    LOG_ERROR << "Socket write error (" << (String) addr() << "): " << SystemEx::sysErrStr();
  }
  return -1;
}


//! Sends \a len bytes of \a data to the socket.
int Socket::receive( char *data, unsigned len )
{
  if ( !isConnected() )
    return 0;

  int res = ::recv( fd(), data, len, 0 );
  if ( res == 0 ) {
    disconnect();
    return -1;
  }

  if ( res != -1 )
    return res;

  if ( SystemEx::sysErrCode() == EWOULDBLOCK )
    return 0;

  disconnect();
  if ( (SystemEx::sysErrCode() == ENOTCONN) || (SystemEx::sysErrCode() == ECONNRESET) || (SystemEx::sysErrCode() == EPIPE) ) {
    _error = 0;
  } else {
    _error = SystemEx::sysErrCode();
    LOG_ERROR << "Socket read error (" << (String) addr() << "): " << SystemEx::sysErrStr() << " (" << _error << ")";
  }
  return -1;
}


//! Enables or disables blocking I/O.
bool Socket::setBlocking( bool enabled )
{
  // Make socket non-blocking
  unsigned long val = enabled ? 0 : 1;

  if ( ioctl( fd(), FIONBIO, &val ) != -1 )
    return true;

  disconnect();
  _error = SystemEx::sysErrCode();
  return false;
}


//! Sets the size of the incoming stream buffer to \a size.
void Socket::setReceiveBuff( unsigned size )
{
  delete [] eback();
  char_type *buff = new char_type[size];
  setg( buff, buff + size, buff + size );
}


//! Sets the size of the outgoing stream buffer to \a size.
void Socket::setSendBuff( unsigned size )
{
  // Allocate an internal stream buffer
  delete [] pbase();
  char_type *buff = new char_type[size];
  setp( buff, buff + size );
}


Socket::int_type Socket::sync( void )
{
  if ( !isConnected() )
    return -1;

  streamsize bytesOut = pptr() - pbase();
  while ( bytesOut ) {
    const streamsize bytesSent = send( pbase(), bytesOut );
    if ( !bytesSent || (bytesSent == -1) )
      return -1;

    if ( bytesSent < bytesOut )
      memmove( pbase(), pbase() + bytesSent, bytesOut - bytesSent );

    pbump( -bytesSent );
    bytesOut -= bytesSent;
  }

  return 0;
}

Socket::int_type Socket::overflow( int_type ch )
{
  if ( !isConnected() )
    return traits::eof();

  streamsize n = pptr() - pbase();

  if ( n && (sync() == -1) )
    return traits::eof();

  if ( (epptr() - pptr()) == 0 ) {
    char_type c = traits::to_char_type( ch );
    return (send( &c, sizeof( c ) ) == sizeof( c )) ? 0 : traits::eof();
  }

  *pptr() = traits::to_char_type( ch );
  pbump( 1 );
  return 0;
}

Socket::int_type Socket::underflow( void )
{
  if ( !isConnected() )
    return traits::eof();

  if ( gptr() < egptr() )
    return traits::to_int_type( *gptr() );

  streamsize n = gptr() - eback();

  if ( !n ) {
    char_type c;
    if ( receive( &c, sizeof( c ) ) != sizeof( c ) )
      return traits::eof();

    return traits::to_int_type( c );
  }

  streamsize BytesIn = receive( eback(), n );
  if ( BytesIn < 1 )
    return traits::eof();

  if ( BytesIn < n )
    memmove( eback() + n - BytesIn, eback(), BytesIn );

  gbump( -BytesIn );
  return traits::to_int_type( *gptr() );
}


Socket::IOEx::IOEx( std::ios::openmode mode )
{
  stringstream err;

  err << "_error ";

  if ( mode & ios::in )
    err << "reading from";
  else
  if ( mode & ios::out )
    err << "writing to";
  else
    err << "accessing";

  err << " socket ";
  *this << err.str();
}
