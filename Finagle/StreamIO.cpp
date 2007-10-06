/*!
** \file StreamIO.cpp
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

#include "StreamIO.h"
#include "MemTrace.h"

using namespace Finagle;
using namespace std;

/*! \class Finagle::expect
** \brief Function object to skip static string during input stream extraction.
**
** Reads the static string \a str from the stream \a in.  If the next input from the stream isn't \a str, sets the fail bit.  If
** EOF is encountered, sets the fail and eof bits.  In both cases, all input is un-read before return.
*/
istream &Finagle::StreamIO::operator >>( istream &in, StreamIO::expect const &str )
{
  typedef char_traits<char> traits;
  istream::sentry guard( in, true );

  if ( !in )
    return in;

  unsigned charsRead = 0;
  for ( string::const_iterator it = str.begin(); it != str.end(); ++it ) {
    streambuf::int_type c = in.rdbuf()->sgetc();

    if ( c == traits::to_int_type( *it ) ) {
      in.rdbuf()->sbumpc();
      charsRead++;
      continue;
    }

    while ( charsRead-- )
      in.rdbuf()->sungetc();

    in.setstate( in.rdstate() | ios_base::failbit );

    if ( c == traits::eof() )
      in.setstate( in.rdstate() | ios_base::eofbit );

    break;
  }

  return in;
}


/*! \class Finagle::find
** \brief Function object to search for static string during input stream extraction.
**
** Reads from the input stream until it encounters the static string \a str.
*/
istream &Finagle::StreamIO::operator >>( istream &in, StreamIO::find const &str )
{
  typedef char_traits<char> traits;

  istream::sentry Guard( in, true );

  if ( !in )
    return in;

  string::const_iterator i = str.begin();

  // find beginning of search string
  streambuf::int_type ch;
  do {
    ch = in.rdbuf()->sgetc();
    if ( ch == traits::eof() ) {
      in.setstate( in.rdstate() | ios_base::failbit | ios_base::eofbit );
      return in;
    }
    in.rdbuf()->sbumpc();
  } while ( ch != traits::to_int_type( *i ) );

  unsigned CharsRead = 0;
  for ( ++i; i != str.end(); ++i ) {
    streambuf::int_type ch = in.rdbuf()->sgetc();
    if ( ch == traits::eof() ) {
      while ( CharsRead-- )
        in.rdbuf()->sungetc();

      in.setstate( in.rdstate() | ios_base::failbit | ios_base::eofbit );
      break;
    }
    CharsRead++;

    if ( ch != traits::to_int_type( *i ) ) {
      in.setstate( in.rdstate() | ios_base::failbit );
      break;
    }

    in.rdbuf()->sbumpc();
  }

  return in;
}


/*! \class Finagle::QuotedString
** \brief Function object to insert/extract quoted strings to/from streams.
**
** A quoted string:
** * Begins and ends with double-quote marks
** * Has internal double-quotes escaped with a single backslash
** * Has internal backslashes escaped with a backslash
**
** Example: \code
** "This is a \"Hello World\" kind of a example"
** \endcode
*/

std::istream &Finagle::operator >>( std::istream &in, QuotedString &qStr )
{
  char ch;
  in.get( ch );
  if ( ch != '"' ) {
    in.setstate( in.rdstate() | ios::failbit );
    return in;
  }

  qStr.str.clear();

  while ( in.good() ) {
    in.get( ch );
    if ( ch == '"' )
      break;

    if ( ch != '\\' ) {
      qStr.str.push_back( ch );
      continue;
    }

    in.get( ch );

    if ( (ch != '\\') && (ch != '"') )
      qStr.str.push_back( '\\' );

    qStr.str.push_back( ch );
  }

  qStr.str.reserve( qStr.str.size() );

  return in;
}

std::ostream &Finagle::operator <<( std::ostream &out, QuotedString const &qStr )
{
  out << '"';

  for ( string::const_iterator i = qStr.str.begin(); (i != qStr.str.end()) && out; ++i ) {
    if ( *i == '"' )
      out << "\\\"";
    else
    if ( *i == '\\' )
      out << "\\\\";
    else
      out << *i;
  }

  out << '"';
  return out;
}
