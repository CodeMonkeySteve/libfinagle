/*!
** \file URL.h
** \author Steve Sloan <steve@finagle.org>
** \date Wed May 7 2008
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

#ifndef FINAGLE_NET_URL_H
#define FINAGLE_NET_URL_H

#include <Finagle/TextString.h>
#include <Finagle/Net/IPAddress.h>
#include <Finagle/Map.h>

namespace Finagle {

class URL : public String {
public:
  URL( String const &url );
  URL( String const &scheme, String const &location );

  static String escape( const String &str );

  typedef Map<String, String> ParamMap;
  static URL HTTP( String const &userInfo, IPAddress const &host, unsigned port, String const &path, ParamMap const &query = ParamMap(), String const &fragment = String(), char delim = ';' );
};

// INLINE IMPLEMENTATION ******************************************************

inline URL::URL( String const &url )
: String( url )
{}

inline URL::URL( String const &scheme, String const &location )
: String( scheme + ':' + location )
{}

}

#endif
