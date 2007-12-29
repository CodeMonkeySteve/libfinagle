/*!
** \file Socket.h
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

#ifndef FINAGLE_SOCKET_H
#define FINAGLE_SOCKET_H

#include <iostream>
#include <sys/socket.h>

#include <Finagle/Exception.h>
#include <Finagle/FileDescWatcher.h>
#include <Finagle/IPAddress.h>

namespace Finagle {

class Socket : public FileDescWatcher, public std::streambuf {
public:
  class Addr {
  public:
    Addr( void ) {}
    virtual ~Addr( void ) {}

    virtual unsigned domainFamily( void ) const = 0;
    virtual sockaddr const &domainAddr( void ) const = 0;
    virtual socklen_t domainAddrLen( void ) const = 0;
    virtual operator String( void ) const = 0;
  };

public:
  typedef Finagle::ObjectPtr<Socket> Ptr;
  typedef std::streambuf::traits_type traits;
  typedef std::streambuf::char_type   char_type;
  typedef std::streambuf::int_type    int_type;

public:
  Socket( int sockDesc = -1 );
  virtual ~Socket( void );
  static Socket::Ptr fromSpec( String const &spec );

  virtual Socket::Addr const &addr( void ) const = 0;

  int fd( void );
  virtual bool connect( void );
  virtual bool listen( void );
  virtual void disconnect( void );
  virtual bool isConnected( void ) const;

  virtual int send( const char *data, unsigned len );
  virtual int receive( char *data, unsigned len );

  virtual int error( void ) const;
  virtual String errorStr( void ) const;

  bool setBlocking( bool enabled );
  void setReceiveBuff( unsigned size );
  void setSendBuff( unsigned size );

  int_type overflow( int_type Ch = std::streambuf::traits_type::eof() );
  int_type underflow( void );
  int_type sync( void );

public:
  class IOEx : public SystemEx {
  public:
    IOEx( std::ios::openmode Mode );
   ~IOEx( void ) throw() {}
  };

protected:
  int _error;
};

//! Writes the Socket::Addr-derived class to \a out as a string (e.g. hostname/port).
std::ostream &operator <<( std::ostream &out, Socket::Addr const &addr );

// INLINE IMPLEMENTATION ******************************************************

//! Returns \c true if the socket is conencted (i.e. \c connect() returned \c true).
inline bool Socket::isConnected( void ) const
{
  return FileDescWatcher::fd() != -1;
}

//! Starts listening on the socket.
inline bool Socket::listen( void )
{
  disconnect();

  if ( (::bind( fd(), &addr().domainAddr(), addr().domainAddrLen() ) == -1) || (::listen( fd(), SOMAXCONN ) == -1) ) {
    _error = SystemEx::sysErrCode();
    return false;
  }

  return true;
}

inline int Socket::error( void ) const
{
  return _error;
}

inline String Socket::errorStr( void ) const
{
  return SystemEx::sysErrStr( _error );
}

inline std::ostream &operator <<( std::ostream &out, Socket::Addr const &addr )
{
  return out << (String) addr;
}

}

#endif
