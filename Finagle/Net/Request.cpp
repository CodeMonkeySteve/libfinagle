/*!
** \file Request.cpp
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

#include <string>
#include <curl/curl.h>

#include "Transfer.h"
#include "Request.h"

#include <iostream>

using namespace std;
using namespace Finagle;
using namespace Transfer;

inline void CURL_ASSERT( CURLcode res )
{
  if ( res != 0 )  throw Finagle::Transfer::Exception( curl_easy_strerror( res ) );
}


/*!
** \class Finagle::Transfer::Request
** \brief A single cURL request.
**
**
*/

Request::Request( URI const &uri )
: _req( curl_easy_init() )
{
  if ( !_req )
    throw Transfer::Exception( "Unable to create cURL easy instance" );

  CURL_ASSERT( curl_easy_setopt( _req, CURLOPT_URL, uri.c_str() ) );

  CURL_ASSERT( curl_easy_setopt( _req, CURLOPT_HEADERFUNCTION, onHeader ) );
  CURL_ASSERT( curl_easy_setopt( _req, CURLOPT_HEADERDATA, (void *) this ) );

  CURL_ASSERT( curl_easy_setopt( _req, CURLOPT_WRITEFUNCTION,  onBodyFrag ) );
  CURL_ASSERT( curl_easy_setopt( _req, CURLOPT_WRITEDATA, (void *) this ) );
}

Request::~Request( void )
{
  if ( _req )
    curl_easy_cleanup( _req );
}


unsigned Request::result( void ) const
{
  if ( !_res ) {
    long code;
    CURL_ASSERT( curl_easy_getinfo( _req, CURLINFO_RESPONSE_CODE, &code ) );
    _res = (unsigned) code;
  }
  return _res;
}


void Request::perform( void )
{
  if ( _req )
    curl_easy_perform( _req );
}


size_t Request::onHeader( const char *data, size_t membSize, size_t membNum, Request *req )
{
  size_t size = membSize * membNum;
  req->recvHeader( data, size );

  if ( (size <= 2) && req->recvBodyStart.connected() ) {
    char *type;
    double size;

    CURL_ASSERT( curl_easy_getinfo( req->_req, CURLINFO_CONTENT_TYPE, &type ) );
    CURL_ASSERT( curl_easy_getinfo( req->_req, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &size ) );
    req->recvBodyStart( (String) type, (size_t) size );
  }

  return size;
}

size_t Request::onBodyFrag( const char *data, size_t membSize, size_t membNum, Request *req )
{
  size_t size = membSize * membNum;
  if ( !size )  return 0;

  req->recvBodyFrag( data, size );
  return req->recvBodyFrag.connected() ? size : 0;
}
