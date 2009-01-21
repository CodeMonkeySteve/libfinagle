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
  _boundary.clear();
  _head.clear();
  _resp = 0;

  if ( _req )  {
    _req->recvBodyStart.connect( boost::bind( &MultipartResponse::onBodyStart, this, _1, _2 ) );
    _req->recvBodyFrag. connect( boost::bind( &MultipartResponse::onBodyFrag,  this, _1, _2 ) );
  }

  return _req;
}


void MultipartResponse::onBodyStart( String const &type, size_t size )
{
  // parse multipart boundary
  String::Array parts( type.split(";") );
  for ( String::Array::ConstIterator field = parts.begin(); field != parts.end(); ++field ) {
    String::Array fieldParts( field->split("=") );
    if ( fieldParts[0].trim() == "boundary" ) {
      _boundary = fieldParts[1].trim();
      break;
    }
  }

  if ( _boundary.empty() )
    throw Transfer::Exception( "Malformed multipart type (missing boundary): \"" + type + "\"" );
}

void MultipartResponse::onBodyFrag( const char *data, size_t size )
{
  #define NEWLINE "\r\n"

  // anybody listening?
  if ( recvPart.empty() ) {
    // nope -- abort
    _req->recvBodyFrag.disconnect( this );
    return;
  }

  // response in progress -- append to part
  if ( _resp ) {
    size_t want = 0;

    if ( _resp->_size )
      want = _resp->_size - _resp->_body.size();
    else {
      // non-standard hack for unsized text parts
      String body( _resp->_body );
      body.append( data, size );

      size_t end = body.find( NEWLINE + _boundary + NEWLINE );
      if ( end == String::npos ) {
        // hack to prevent response from completing prematurely
        _resp->_size = _resp->_body.size() + size + 1;
        _resp->onBodyFrag( data, size );
        _resp->_size = 0;
        return;
      }

      want = end - _resp->_body.size();
      _resp->_size = end;
    }

    if ( want > size ) {
      _resp->onBodyFrag( data, size );
      return;
    }

    // last fragment for this part
    _resp->onBodyFrag( data, want );
    recvPart( *_resp );
    _resp = 0;

    data += want;
    size -= want;

    if ( !size )
      return;
  }

  // no current response -- read headers for next part
  String partType;
  size_t partSize = 0;

  // only continue if we have the whole header
  _head.append( data, size );
  size_t headEnd = _head.find( NEWLINE NEWLINE );
  if ( headEnd == String::npos )
    return;
  String headers = _head.substr( 0, headEnd + sizeof(NEWLINE)-1 );
  _head.erase( 0, headEnd + sizeof(NEWLINE)-1 + sizeof(NEWLINE)-1 );

  // drop terminating CRLF from last part
  if ( headers.find( NEWLINE ) == 0 )
    headers.erase( 0, sizeof(NEWLINE)-1 );

  // drop boundary
  if ( headers.find( _boundary + NEWLINE ) == 0 )
    headers.erase( 0, _boundary.size() + sizeof(NEWLINE)-1 );

  while ( !headers.empty() ) {
    size_t end = headers.find( NEWLINE );
    String header = headers.substr( 0, end );
    headers.erase( 0, end + sizeof(NEWLINE)-1 );

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

  if ( partType.empty() )
    throw Transfer::Exception( "Malformed response: missing content-type" );

  // non-standard hack for unsized text parts
  if ( !partSize && (partType != "text/plain") )
    throw Transfer::Exception( "Malformed response: missing content-length" );

  _resp = new Response( 0, true );
  _resp->onBodyStart( partType, partSize );

  // handle first fragment
  if ( _head.size() ) {
    String frag( _head );
    _head.clear();
    onBodyFrag( (const char *) frag, frag.size() );
  }
}
