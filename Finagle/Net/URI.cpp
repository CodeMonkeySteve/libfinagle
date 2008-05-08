/*!
** \file URI.cpp
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

#include "URI.h"

using namespace Finagle;

/*!
** \class Finagle::URI
** \brief Uniform Resource Identifier
**
**
*/

URI::URI( String const &scheme, String const &userInfo, String const &host, unsigned port, String const &path )
: String( scheme + "://" )
{
  if ( userInfo ) {
    append( userInfo );
    append( 1, '@' );
  }

  append( host );

  if ( port ) {
    append( 1, ':' );
    append( String(port) );
  }

  if ( path[0] != '/' )
    append( 1, '/' );

  append( path );
}
