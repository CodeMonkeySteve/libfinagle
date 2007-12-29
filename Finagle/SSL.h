/*!
** \file SSL.h
** \author Steve Sloan <steve@finagle.org>
** \date Tue Dec 26 2006
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

#ifndef FINAGLE_SSL_H
#define FINAGLE_SSL_H

#include <Finagle/Socket.h>
#include <Finagle/FilePath.h>
#include <AppLog.h>

namespace Finagle {

namespace SSL {

#include <openssl/ssl.h>
#include <openssl/err.h>

class BadKeyFileEx : public Exception {
public:
  BadKeyFileEx( FilePath const &keyFile )
  : Exception( "Bad SSL key file: " + keyFile.path() ) {}
};


class Context {
public:
  Context( FilePath const &keyFile );
 ~Context( void );

  operator SSL_CTX *( void );

protected:
  SSL_CTX &_ctx;
};

template <class SockType>
class Socket : public SockType {
public:
  typedef Finagle::ObjectPtr<Socket> Ref;

public:
  Socket( Context &ctx )
  : SockType(), _ctx(*ctx), _ssl(0), _sslError( SSL_ERROR_NONE ) {}

  Socket( typename SockType::Addr const &addr, int sockDesc = -1, Finagle::Socket const *src = 0 );

  Context *context( void ) const;
  void context( Context &ctx );

  bool connect( void );
  bool listen( void );
  void disconnect( void );
  bool isConnected( void ) const;

  int error( void ) const;
  String errorStr( void ) const;

  int send( const char *data, unsigned len );
  int receive( char *data, unsigned len );

protected:
  Context *_ctx;
  SSL *_ssl;
  int _sslError;
};

// TEMPLATE/INLINE IMPLEMENTATION *********************************************

inline Context::operator SSL_CTX *( void )
{
  return &_ctx;
}

template <class SockType>
Socket<SockType>::Socket( typename SockType::Addr const &addr, int sockDesc, Finagle::Socket const *src )
: SockType( addr, sockDesc, src ), _ctx(0), _ssl(0), _sslError( SSL_ERROR_NONE )
{
  if ( !src )
    return;

  _ctx = dynamic_cast<Socket<SockType> const *>(src)->context();
  _ssl = SSL_new(*_ctx);

  BIO *io = BIO_new_socket( SockType::fd(), BIO_NOCLOSE );
  SSL_set_bio( _ssl, io, io );

  int res  = SSL_accept(_ssl);
  if ( res < 1 ) {
    LOG_ERROR << "SSL accept error (" << (String) SockType::addr() << "): " << ERR_error_string( SSL_get_error( _ssl, res ), 0 );
    disconnect();
  }
}


template <class SockType>
Context *Socket<SockType>::context( void ) const
{
  return _ctx;
}

template <class SockType>
void Socket<SockType>::context( Context &ctx )
{
  _ctx = &ctx;
}

template <class SockType>
int Socket<SockType>::error( void ) const
{
  return (_sslError != SSL_ERROR_NONE) ? _sslError : SockType::error();
}

template <class SockType>
String Socket<SockType>::errorStr( void ) const
{
  return (_sslError != SSL_ERROR_NONE) ? String( ERR_error_string( _sslError, 0 ) ) : SockType::errorStr();
}

template <class SockType>
bool Socket<SockType>::connect( void )
{
  _sslError = SSL_ERROR_NONE;
  disconnect();

// FIX ME
//  SSL_connect();
return false;
}

template <class SockType>
bool Socket<SockType>::listen( void )
{
  return _ctx && SockType::listen();
}

template <class SockType>
void Socket<SockType>::disconnect( void )
{
  if ( _ssl ) {
    _sslError = SSL_ERROR_NONE;
    int res = SSL_shutdown( _ssl );
    if ( res == -1 ) {
      _sslError = SSL_get_error( _ssl, res );
      LOG_ERROR << "SSL Socket disconnect error (" << (String) SockType::addr() << "): " << errorStr();
      return;
    }

    SSL_free(_ssl);
    _ssl = 0;
  }
  SockType::disconnect();
}

template <class SockType>
inline bool Socket<SockType>::isConnected( void ) const
{
  return _ssl && SockType::isConnected();
}


template <class SockType>
int Socket<SockType>::send( const char *data, unsigned len )
{
  _sslError = SSL_ERROR_NONE;
  if ( !SockType::isConnected() )
    return -1;

  int res = SSL_write( _ssl, data, len );
  if ( res == (int) len )
    return res;

  disconnect();

//  if ( (SystemEx::sysErrCode() == ENOTCONN) || (SystemEx::sysErrCode() == ECONNRESET) ) {
//    _error = 0;
//  } else {
//    _error = SystemEx::sysErrCode();

  _sslError = SSL_get_error( _ssl, res );
  LOG_ERROR << "SSL Socket write error (" << (String) SockType::addr() << "): " << errorStr();
  return -1;
}


template <class SockType>
int Socket<SockType>::receive( char *data, unsigned len )
{
  _sslError = SSL_ERROR_NONE;
  if ( !isConnected() || !SSL_pending(_ssl) )
    return 0;

  int res = SSL_read( _ssl, data, len );
  if ( res > 0 )
    return res;

  _sslError = SSL_get_error( _ssl, res );
  if ( res == 0 ) {
    disconnect();
    return 0;
  }

  disconnect();
  LOG_ERROR << "SSL Socket read error (" << (String) SockType::addr() << "): " << errorStr();
  return -1;
}

} }

#endif
