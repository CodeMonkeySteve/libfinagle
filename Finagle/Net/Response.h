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
  Response( URL const &uri );

  Request::Ptr req( void );

  String const &type( void ) const;
  size_t size( void ) const;
  String const &body( void ) const;

public:
  sigslot::signal1<Response const &> recvBody;

protected:
  void init( void );

  void onBodyStart( String const &type, size_t size );
  void onBodyFrag( const char *data, size_t size );

  friend class MultipartResponse;

protected:
  Request::Ptr _req;
  bool         _saveBody;

  String _type;
  size_t _size;
  String _body;
};

// INLINE IMPLEMENTATION **********************************************************************************************************

inline Request::Ptr Response::req( void )
{
  return _req;
}

inline String const &Response::type( void ) const
{
  return _type;
}

inline size_t Response::size( void ) const
{
  return _size;
}

inline String const &Response::body( void ) const
{
  return _body;
}

} }

#endif
