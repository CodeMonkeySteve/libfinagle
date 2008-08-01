/*!
** \file URL.cpp
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

#include "URL.h"

using namespace Finagle;

/*!
** \namespace Finagle::URL
** \brief Uniform Resource Locator
*/

URL URL::HTTP( String const &userInfo, IPAddress const &host, unsigned port, String const &path, Map<String, String> const &query, String const &fragment )
{
  URL url( "http", "//" );

  if ( userInfo ) {
    url.append( userInfo );
    url.append( 1, '@' );
  }

  url.append( host.name() );

  if ( port ) {
    url.append( 1, ':' );
    url.append( String(port) );
  }

  if ( path[0] != '/' )
    url.append( 1, '/' );

  url.append( path );

  if ( !query.empty() ) {
    Map<String, String>::ConstIterator q = query.begin();
    url.append( '?' + q.key() + '=' + q.val() );

    for ( ++q; q != query.end(); ++q )
      url.append( ';' + q.key() + '=' + q.val() );
  }

  if ( fragment ) {
    url.append( '#' );
    url.append( fragment );
  }
}
