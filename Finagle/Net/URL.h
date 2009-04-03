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

#include <string>
#include <map>
#include <ostream>

namespace Finagle {

class URL {
public:
  typedef std::multimap<std::string, std::string> ParamMap;
  static const char ParamDelim = ';';

public:
  static std::string escape( const std::string &str );

  URL( void );
//  URL( std::string const &url );

  URL &username( std::string const &user );
  URL &password( std::string const &pass );
  URL &host( std::string const &host );
  URL &port( unsigned short port );
  URL &path( std::string const &path );
  URL &fragment( std::string const &frag );
  URL &query( ParamMap const &params );
  URL &delim( char delim );
  ParamMap &query( void );
  URL &param( std::string const &name, std::string const &val );

  std::string const &username( void ) const;
  std::string const &password( void ) const;
  std::string const &host( void ) const;
  unsigned short port( void ) const;
  std::string const &path( void ) const;
  std::string const &fragment( void ) const;
  ParamMap const &query( void ) const;
  char delim( void ) const;

  std::string const &request( void ) const;
  operator std::string const &( void ) const;

protected:
  mutable std::string _url, _req;
  std::string _user, _pass;
  std::string _host, _path, _frag;
  unsigned short _port;
  ParamMap _params;
  char _delim;
};

// INLINE IMPLEMENTATION ******************************************************


inline std::ostream &operator <<( std::ostream &out, URL const &url )
{
  return out << (std::string const &) url;
}


inline URL::URL( void )
: _port(0), _delim( ParamDelim )
{}

// inline URL::URL( std::string const &url )
// : _url( url )
// {}


inline URL &URL::username( std::string const &user )
{
  if ( _user != user ) {
    _user = user;
    _url.clear();
  }
  return *this;
}

inline URL &URL::password( std::string const &pass )
{
  if ( _pass != pass ) {
    _pass = pass;
    _url.clear();
  }
  return *this;
}

inline URL &URL::host( std::string const &host )
{
  if ( _host != host ) {
    _host = host;
    _url.clear();
  }
  return *this;
}

inline URL &URL::port( unsigned short port )
{
  if ( _port != port ) {
    _port = port;
    _url.clear();
  }
  return *this;
}

inline URL &URL::path( std::string const &path )
{
  if ( _path != path ) {
    _path = path;
    _req.clear();
  }
  return *this;
}

inline URL &URL::fragment( std::string const &frag )
{
  if ( _frag != frag ) {
    _frag = frag;
    _req.clear();
  }
  return *this;
}

inline URL &URL::query( ParamMap const &params )
{
  if ( _params != params ) {
    _params = params;
    _req.clear();
  }
  return *this;
}

inline URL &URL::delim( char delim )
{
  if ( _delim != delim ) {
    _delim = delim;
    _req.clear();
  }
  return *this;
}

inline URL &URL::param( std::string const &name, std::string const &val )
{
  _params.insert( ParamMap::value_type( name, val ) );
  return *this;
}


inline std::string const &URL::username( void ) const
{
  return _user;
}

inline std::string const &URL::password( void ) const
{
  return _pass;
}

inline std::string const &URL::host( void ) const
{
  return _host;
}

inline unsigned short URL::port( void ) const
{
  return _port;
}

inline std::string const &URL::path( void ) const
{
  return _path;
}

inline std::string const &URL::fragment( void ) const
{
  return _frag;
}

inline URL::ParamMap const &URL::query( void ) const
{
  return _params;
}

inline char URL::delim( void ) const
{
  return _delim;
}

}

#endif
