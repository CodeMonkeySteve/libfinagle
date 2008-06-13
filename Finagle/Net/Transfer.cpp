/*!
** \file Transfer.cpp
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
#include <curl/multi.h>

#include "Transfer.h"
#include "Request.h"

using namespace Finagle;
using namespace Transfer;

inline void CURLM_ASSERT( CURLMcode res )
{
  if ( res != 0 )
    throw Transfer::Exception( curl_multi_strerror( res ) );
}

/*!
** \class Finagle::Transfer::Processor
** \brief Handles cURL startup/shutdown and Request processing.
**
**
*/

Processor::Processor( void )
{
  _refs++;  // kludge to avoid premature deletion

  if ( curl_global_init( CURL_GLOBAL_ALL ) != 0 )
    throw Transfer::Exception( "Unable to initialize cURL" );

  _reqs = curl_multi_init();
  if ( !_reqs )
    throw Transfer::Exception( "Unable to create cURL multi instance" );

  enable();
}

Processor::~Processor( void )
{
  if ( _reqs ) {
    curl_multi_cleanup( _reqs );
    _reqs = 0;
  }

  curl_global_cleanup();
}


Request const &Processor::add( Request const &req )
{
  CURLM_ASSERT( curl_multi_add_handle( _reqs, req._req ) );

  CURLMcode res;
  int n = 0;
  while ( (res = curl_multi_perform( _reqs, &n )) == CURLM_CALL_MULTI_PERFORM )
    ;

  CURLM_ASSERT( res );
  return req;
}

Request const &Processor::remove( Request const &req )
{
  CURLMcode res = curl_multi_remove_handle( _reqs, req._req );
  if ( res != CURLM_BAD_EASY_HANDLE )
    CURLM_ASSERT( res );

  return req;
}


int Processor::fds( fd_set &readFDs, fd_set &writeFDs, fd_set &exceptFDs ) const
{
  int fd = -1;
  CURLM_ASSERT( curl_multi_fdset( _reqs, &readFDs, &writeFDs, &exceptFDs, &fd ) );
  return fd;
}

void Processor::onSelect( fd_set &readFDs, fd_set &writeFDs, fd_set &exceptFDs ) const
{
  CURLMcode res;
  int n = 0;
  while ( (res = curl_multi_perform( _reqs, &n )) == CURLM_CALL_MULTI_PERFORM )
    ;
  CURLM_ASSERT( res );
}
