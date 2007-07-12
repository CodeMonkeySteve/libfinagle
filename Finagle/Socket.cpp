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

#include "Socket.h"
#include "AppLog.h"

#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <UnixSocket.h>
#include <InetSocket.h>

#include "MemTrace.h"

using namespace std;
using namespace Finagle;

/*! \class Finagle::Socket
** Base class for network socket.
*/

/*! Address constructor
** If \a sockDesc is non-0, binds to an existing socket.
*/
Socket::Socket( int sockDesc, Socket const * )
: FileDescWatcher(sockDesc), _error(0)
{
  if ( sockDesc != -1 ) {
    setBlocking( true );
    setReceiveBuff( 256 );
    setSendBuff( 256 );
  }
}


Socket::~Socket( void )
{
  disconnect();

  delete [] eback();
  delete [] pbase();
  setg( 0, 0, 0 );
  setp( 0, 0 );
}


Socket::Ref Socket::fromSpec( String const &spec )
{
  File sockAddr( spec );
  if ( sockAddr.exists() )
    return new UnixSocket( UnixSocket::Addr( sockAddr ) );

  return new InetSocket( InetSocket::Addr( spec ) );
}


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


/*!
** Attempts to connect to the remote host.
**
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


//! Attempts to send \a Len bytes of \a Data to the socket.
int Socket::send( const char *Data, unsigned Len )
{
  if ( !isConnected() )
    return -1;

  int Res = ::send( fd(), Data, Len, 0 );
  if ( Res != -1 )
    return Res;

  if ( SystemEx::sysErrCode() == EWOULDBLOCK )
    return 0;

  disconnect();

  if ( (SystemEx::sysErrCode() == ENOTCONN) || (SystemEx::sysErrCode() == ECONNRESET) || (SystemEx::sysErrCode() == EPIPE) ) {
    _error = 0;
    disconnected();
  } else {
    _error = SystemEx::sysErrCode();
    LOG_ERR << "Socket write error (" << (String) addr() << "): " << SystemEx::sysErrStr();
  }
  return -1;
}


//! Sends \a Len bytes of \a Data to the socket.
int Socket::receive( char *Data, unsigned Len )
{
  if ( !isConnected() )
    return 0;

  int Res = ::recv( fd(), Data, Len, 0 );
  if ( Res == 0 ) {
    disconnect();
    disconnected();
    return -1;
  }

  if ( Res != -1 )
    return Res;

  if ( SystemEx::sysErrCode() == EWOULDBLOCK )
    return 0;

  disconnect();

  if ( (SystemEx::sysErrCode() == ENOTCONN) || (SystemEx::sysErrCode() == ECONNRESET) || (SystemEx::sysErrCode() == EPIPE) ) {
    _error = 0;
    disconnected();
  } else {
    _error = SystemEx::sysErrCode();
    LOG_ERR << "Socket read error (" << (String) addr() << "): " << SystemEx::sysErrStr() << " (" << _error << ")";
  }
  return -1;
}


//! Enables or disables blocking I/O.
bool Socket::setBlocking( bool Enabled )
{
  // Make socket non-blocking
  unsigned long Val = Enabled ? 0 : 1;

  if ( ioctl( fd(), FIONBIO, &Val ) != -1 )
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

  streamsize BytesOut = pptr() - pbase();
  while ( BytesOut ) {
    const streamsize BytesSent = send( pbase(), BytesOut );
    if ( !BytesSent || (BytesSent == -1) )
      return -1;

    if ( BytesSent < BytesOut )
      memmove( pbase(), pbase() + BytesSent, BytesOut - BytesSent );

    pbump( -BytesSent );
    BytesOut -= BytesSent;
  }

  return 0;
}


Socket::int_type Socket::overflow( int_type Ch )
{
  if ( !isConnected() )
    return traits::eof();

  streamsize n = pptr() - pbase();

  if ( n && (sync() == -1) )
    return traits::eof();

  if ( (epptr() - pptr()) == 0 ) {
    char_type c = traits::to_char_type( Ch );
    return( (send( &c, sizeof( c ) ) == sizeof( c )) ? 0 : -1 );
  }

  *pptr() = traits::to_char_type( Ch );
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


Socket::IOEx::IOEx( ios::openmode mode )
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
  append( err.str() );
}
