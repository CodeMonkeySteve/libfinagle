/*!
** \file Response.h
** \author Steve Sloan <steve@finagle.org>
** \date Wed May 7 2008
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

#ifndef FINAGLE_NET_RESPONSE_H
#define FINAGLE_NET_RESPONSE_H

#include <sigslot/sigslot.h>
#include <Finagle/Map.h>
#include <Finagle/Net/Request.h>

namespace Finagle {
namespace Transfer {

class Response : public sigslot::has_slots<> {
public:
  typedef Map<NoCase, String> HeaderMap;

public:
  Response( Request::Ptr req, bool saveBody = true );
  Response( URI const &uri );
  virtual ~Response( void ) {}

  unsigned statCode( void ) const;
  String const &statMsg( void ) const;
  bool succeeded( void ) const;
  bool failed( void ) const;

  HeaderMap &headers( void );
  HeaderMap const &headers( void ) const;

  String const &type( void ) const;
  size_t size( void ) const;

  void perform( void );

public:
  sigslot::signal1<Response::HeaderMap const &> recvHeaders;
  sigslot::signal1<String const &> recvBody;

protected:
  void init( void );

  virtual void onHeader( const char *data, size_t size );
  virtual void onBodyFrag( const char *data, size_t size );

protected:
  Request::Ptr _req;
  bool         _saveBody;

  unsigned _code;
  String   _msg;

  HeaderMap _headers;
  String    _type;
  size_t    _size;
  String    _body;
};

// INLINE IMPLEMENTATION ******************************************************

inline Response::Response( Request::Ptr req, bool saveBody )
: _req( req ), _saveBody( saveBody )
{
  init();
}

inline Response::Response( URI const &uri )
: _req( new Request( uri ) ), _saveBody( true )
{
  init();
}


inline unsigned Response::statCode( void ) const
{
  return _code;
}

inline String const &Response::statMsg( void ) const
{
  return _msg;
}

inline bool Response::succeeded( void ) const
{
  return (_code >= 200) && (_code < 300);
}

inline bool Response::failed( void ) const
{
  return (_code >= 400) && (_code < 600);
}


inline String const &Response::type( void ) const
{
  return _type;
}

inline size_t Response::size( void ) const
{
  return _size;
}


inline Response::HeaderMap &Response::headers( void )
{
  return _headers;
}

inline Response::HeaderMap const &Response::headers( void ) const
{
  return _headers;
}


inline void Response::perform( void )
{
  _req->perform();
}

} }

#endif
