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

#include <boost/algorithm/string/classification.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include "URL.h"

using namespace std;
using namespace boost;
using namespace Finagle;

/*!
** \namespace Finagle::URL
** \brief HTTP Uniform Resource Locator
*/

string URL::escape( string const &str )
{
  ostringstream out;
  for ( string::const_iterator ch = str.begin(); ch != str.end(); ++ch ) {
    if ( is_alnum()( *ch ) || (*ch == '_') || (*ch == '.') || (*ch == '-') )
      out << *ch;
    else
    if ( *ch == ' ' )
      out << '+';
    else
      out << '%' << boost::format("%02X") % unsigned(*ch);
  }

  return out.str();
}


std::string const &URL::request( void ) const
{
  if ( !_req.empty() )
    return _req;

  ostringstream req;

  if ( _path[0] != '/' )  req << '/';
  req << _path;

  if ( !_params.empty() ) {
    ParamMap::const_iterator p = _params.begin();
    req << '?' << URL::escape(p->first) << '=' << URL::escape(p->second);

    for ( ++p; p != _params.end(); ++p )
      req << _delim << URL::escape(p->first) << '=' << URL::escape(p->second);
  }
  if ( !_frag.empty() )
    req << "#" << URL::escape(_frag);

  _req = req.str();
  return _req;
}

URL::operator string const &( void ) const
{
  if ( !_url.empty() && !_req.empty() )
    return _url;

  ostringstream url;
  url << "http://";

  if ( !_user.empty() ) {
    url << _user;
    if ( !_pass.empty() )  url << ':' << _pass;
    url << '@';
  }

  url << _host;
  if ( _port )  url << ':' << lexical_cast<unsigned>(_port);
  url << request();

  _url = url.str();
  return _url;
}
