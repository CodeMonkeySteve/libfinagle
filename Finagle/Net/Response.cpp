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

#include <iostream>
using namespace std;

using namespace Finagle;
using namespace Transfer;

/*!
** \class Finagle::Transfer::Response
** \brief A single response from a cURL request.
**
**
*/

void Response::init( void )
{
  _code = 0;
  _req->recvHeader.connect( this, &Response::onHeader );

  if ( _saveBody )
    _req->recvBodyFrag.connect( this, &Response::onBodyFrag );
}


void Response::onHeader( const char *data, size_t size )
{
  String header( data, size );
  header.trim();
  if ( header.empty() ) {
    _req->recvHeader.disconnect( this );
    recvHeaders( _headers );
    return;
  }

  String::size_type i = header.find_first_of( ':' );
  if ( i == String::npos ) {
    String::size_type i = header.find_first_of( ' ' );
    if ( (i == String::npos) || _code )
      throw Transfer::Exception( "Malformed HTTP header: \"" + header + "\"" );

    String proto( header.substr( 0, i ) );
    _code = header.substr( i + 1, 3 ).as<unsigned>();
    _msg = header.substr( i + 5 ).trim();
    return;
  }

  NoCase name( header.substr( 0, i ).trim() );
  String val( header.substr( i + 1 ).trim() );

  _headers[name] = val;

  if ( name == "content-type" )
    _type = val;
  else
  if ( name == "content-length" ) {
    _size = val.as<size_t>();

    if ( _saveBody )
      _body.reserve( _size );
  }
}

void Response::onBodyFrag( const char *data, size_t size )
{
  _body.append( data, size );

  if ( _body.size() >= _size ) {
    recvBody( _body );
    _req->recvBodyFrag.disconnect( this );
  }
}


/*!
** \class Finagle::Transfer::MultipartResponse
** \brief A MIME multi-part response from a cURL request.
**
**
*/

#if 0
MultipartResponse::MultipartResponse( Request::Ptr req, bool saveBody )
: _req( req ), _saveBody( saveBody )
{
  _req->recvHeader.connect( this, &Response::onHeader );
  _req->recvBodyFrag.connect( this, &Response::onBodyFrag );
}

void Response::onHeader( String const &name, String const &val )
{
  NoCase n( name );
  _headers[n] = val;

  if ( n == "content-type" )
    _type = val;
  else
  if ( n == "content-length" ) {
    _size = val.as<size_t>();

    if ( _saveBody )
      _body.reserve( _size );
  }
}

void Response::onBodyFrag( String const &frag )
{
  if ( !_saveBody ) {
    recvHeaders( _headers );
    _req->disconnect( this );
    return;
  }

  _body.append( frag );
}


#endif
