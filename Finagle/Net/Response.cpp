/*!
** \file Response.cpp
** \date Wed May 7 2008
** \author Steve Sloan <steve@finagle.org>
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

#include <curl/curl.h>

#include "Response.h"

using namespace Finagle;
using namespace Transfer;


/*!
** \class Finagle::Transfer::Response
** \brief A single response from a cURL request.
**
**
*/

Response::Response( Request::Ptr req, bool saveBody )
: _req( req ), _saveBody( saveBody ), _size( 0 )
{
  init();
}

Response::Response( URL const &uri )
: _req( new Request( uri ) ), _saveBody( true ), _size( 0 )
{
  init();
}

void Response::request( Request::Ptr const &req )
{
  if ( _req ) {
    _req->recvBodyStart.disconnect( this );
    _req->recvBodyFrag .disconnect( this );
    _req->recvBodyDone .disconnect( this );
  }
  _req = req;
  init();
}

void Response::init( void )
{
  if ( !_req )  return;
  _req->recvBodyStart.connect( boost::bind( &Response::onBodyStart, this, _1, _2 ) );
  _req->recvBodyDone .connect( boost::bind( &Response::onBodyDone, this ) );
  if ( _saveBody )  _req->recvBodyFrag.connect( boost::bind( &Response::onBodyFrag, this, _1, _2 ) );
}


void Response::onBodyStart( String const &type, size_t size )
{
  _type = type;
  _size = size;

  if ( _size && _saveBody )
    _body.reserve( _size );
}

void Response::onBodyFrag( const char *data, size_t size )
{
  if ( !data || !size )  return;
  _body.append( data, size );
}

void Response::onBodyDone( void )
{
  if ( _req )  _req->recvBodyFrag.disconnect( this );
  recvBody( *this );
}
