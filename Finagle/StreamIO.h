/*!
** \file StreamIO.h
** \author Steve Sloan <steve@finagle.org>
** \date Sat Jan 10 2004
** Copyright (C) 2004 by Steve Sloan
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

#ifndef FINAGLE_STREAMIO_H
#define FINAGLE_STREAMIO_H

#include <string>
#include <istream>

namespace Finagle {

namespace StreamIO {

  struct expect : protected std::string {
    explicit expect( const std::string &str ): std::string( str ) {}
    friend std::istream &operator >>( std::istream &in, expect const &str );
  };

  struct find : protected std::string {
    explicit find( const std::string &str ): std::string( str ) {}
    friend std::istream &operator >>( std::istream &in, find const &str );
  };

  std::istream &operator >>( std::istream &in, expect const &str );
  std::istream &operator >>( std::istream &in, find const &str );

}

struct QuotedString {
  explicit QuotedString( std::string &str ) : str( str ) {}
  friend std::istream &operator >>( std::istream &in, QuotedString &qStr );
  friend std::ostream &operator <<( std::ostream &out, QuotedString const &qStr );

protected:
  std::string &str;
};

std::istream &operator >>( std::istream &in, QuotedString &qStr );
std::ostream &operator <<( std::ostream &out, QuotedString const &qStr );

}

#endif
