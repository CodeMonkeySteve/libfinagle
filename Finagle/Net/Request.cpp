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

#include "Request.h"

using namespace std;
using namespace Finagle;
using namespace Transfer;

#define CURL_ASSERT( e ) {                 \
  CURLcode __curl_result = (e);                 \
  if ( __curl_result != 0 ) {              \
    throw Transfer::Exception( curl_easy_strerror( __curl_result ) );  \
  }                                         \
}


/*!
** \class Finagle::Transfer::Request
** \brief A single cURL request.
**
**
*/

Request::Request( URI const &uri )
: _uri( uri ), _req( curl_easy_init() )
{
  if ( !_req )
    throw Transfer::Exception( "Unable to create cURL easy instance" );

  CURL_ASSERT( curl_easy_setopt( _req, CURLOPT_URL, _uri.c_str() ) );

  CURL_ASSERT( curl_easy_setopt( _req, CURLOPT_HEADERFUNCTION, onHeader ) );
  CURL_ASSERT( curl_easy_setopt( _req, CURLOPT_HEADERDATA, (void *) this ) );

  CURL_ASSERT( curl_easy_setopt( _req, CURLOPT_WRITEFUNCTION,  onBodyFrag ) );
  CURL_ASSERT( curl_easy_setopt( _req, CURLOPT_WRITEDATA, (void *) this ) );

//  curl_easy_setopt( curl, CURLOPT_USERAGENT, "vigilos-IPCamD/1.0" );
}

Request::~Request( void )
{
  if ( _req )
    curl_easy_cleanup( _req );
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
  return req->recvHeader.connected() ? size : 0;
}

size_t Request::onBodyFrag( const char *data, size_t membSize, size_t membNum, Request *req )
{
  size_t size = membSize * membNum;
  req->recvBodyFrag( data, size );
  return req->recvBodyFrag.connected() ? size : 0;
}
