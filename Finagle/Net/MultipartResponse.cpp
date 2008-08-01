/*!
** \file MultipartResponse.cpp
** \date Fri May 9 2008
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

#include <algorithm>
#include <curl/curl.h>
#include <Finagle/AppLog.h>
#include <Finagle/Util.h>

#include "MultipartResponse.h"

using namespace std;
using namespace Finagle;
using namespace Transfer;

/*!
** \class Finagle::Transfer::MultipartResponse
** \brief A MIME multi-part response from a cURL request.
**
**
*/

Request::Ptr MultipartResponse::request( Request::Ptr req )
{
  if ( _req == req )
    return _req;

  if ( _req )  _req->recvBodyFrag.disconnect( this );
  _req = req;
  if ( _req )  _req->recvBodyFrag.connect( this, &MultipartResponse::onBodyFrag );

  return _req;
}


void MultipartResponse::onBodyFrag( const char *data, size_t size )
{
  static const char delim[] = "\r\n";

  // anybody listening?
  if ( !recvPart.connected() ) {
    // nope -- abort
    _req->recvBodyFrag.disconnect( this );
    return;
  }

  // response in progress -- append to part
  if ( _resp ) {
    size_t remain = _resp->_size - _resp->_body.size();

    if ( size < remain ) {
      _resp->onBodyFrag( data, size );
      return;
    }

    // last fragment for this part
    _resp->onBodyFrag( data, remain );
    recvPart( *_resp );

    delete _resp;
    _resp = 0;

    data += remain;
    size -= remain;

    if ( !size )
      return;
  }

  // no current response -- read headers for next part
  String partType;
  size_t partSize = 0;

  // only continue if we have the whole header
  _head.append( data, size );
  size_t headEnd = _head.find( "\r\n\r\n" );
  if ( headEnd == String::npos )
    return;
  String headers = _head.substr( 0, headEnd + 2 );
  _head.erase( 0, headEnd + 4 );

  // drop terminating CRLF from last part
  if ( headers.find( delim ) == 0 )
    headers.erase( 0, 2 );

  // ignore boundary delimeter
  if ( (headers[0] == '-') && (headers[1] == '-') ) {
    size_t end = headers.find( delim );
    if ( end > 2 )
      headers.erase( 0, end + 2 );
  }

  while ( !headers.empty() ) {
    size_t end = headers.find( delim );
    String header = headers.substr( 0, end );
    headers.erase( 0, end + 2 );

    String::size_type i = header.find_first_of( ':' );
    if ( i == String::npos )
      throw Transfer::Exception( "Malformed HTTP header: \"" + header + "\"" );

    NoCase name( header.substr( 0, i ).trim() );
    String val( header.substr( i + 1 ).trim() );

    if ( name == "content-type" )
      partType = val;
    else
    if ( name == "content-length" )
      partSize = val.as<size_t>();
  }

  if ( partType.empty() || !partSize )
    throw Transfer::Exception( "Malformed response: missing content-type or -size" );

  _resp = new Response( 0, true );
  _resp->onBodyStart( partType, partSize );

  // handle first fragment
  if ( _head.size() ) {
    String frag( _head );
    _head.clear();
    onBodyFrag( (const char *) frag, frag.size() );
  }
}
