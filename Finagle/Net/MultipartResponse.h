/*!
** \file MultipartResponse.h
** \author Steve Sloan <steve@finagle.org>
** \date Fri May 9 2008
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

#ifndef FINAGLE_NET_MULTIPARTRESPONSE_H
#define FINAGLE_NET_MULTIPARTRESPONSE_H

#include <Finagle/Net/Response.h>

namespace Finagle {
namespace Transfer {

class MultipartResponse : public sigslot::has_slots<> {
public:
  MultipartResponse( Request::Ptr req = 0 );
//  MultipartResponse( URL const &uri );
 ~MultipartResponse( void );

  Request::Ptr request( void ) const;
  Request::Ptr request( Request::Ptr req );

public:
  sigslot::signal1<Response const &> recvPart;

protected:
  void onBodyFrag( const char *data, size_t size );

protected:
  Request::Ptr _req;
  String _head;
  Response *_resp;
};

// INLINE IMPLEMENTATION **********************************************************************************************************

inline MultipartResponse::MultipartResponse( Request::Ptr req )
: _req( 0 ), _resp( 0 )
{
  request( req );
}

inline MultipartResponse::~MultipartResponse( void )
{
  if ( _resp )
    delete _resp;
}


inline Request::Ptr MultipartResponse::request( void ) const
{
  return _req;
}

} }

#endif
