/*!
** \file RegEx.h
** \date Wed Dec 15 2004
** \author Steve Sloan <steve@finagle.org>
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

#ifndef FINAGLE_REGEX_H
#define FINAGLE_REGEX_H

#include <Finagle/Exception.h>
#include <Finagle/TextString.h>
#include <Finagle/Array.h>

#include <pcre.h>

namespace Finagle {

class RegEx {
public:
  class Exception : public Finagle::Exception {
  public:
    Exception( const char *regExp, const char *errStr )
      : Finagle::Exception( String( "Regular expression \"" ) + regExp + "\" " + errStr ) {}
  };

public:
  RegEx( const char *regExp, bool ignoreCase = false, unsigned optFlags = 0 );
 ~RegEx( void );

  bool operator()( const char *text = 0 );
  String operator[]( unsigned subMatch ) const;

  String const &text( void ) const;
  Array<int> const &offsets( void ) const;

  bool search( const char *text = 0, unsigned optFlags = 0 );

protected:
  void clearMatchList( void );

protected:
  pcre *_exp;
  pcre_extra *_extra;

  // match data
  String _text;
  Array<int> _offsets;
};

// INLINE IMPLEMENTATION ******************************************************

inline RegEx::~RegEx( void )
{
  if ( _extra )  pcre_free( _extra );
  if ( _exp )  pcre_free( _exp );
}

inline bool RegEx::operator()( const char *text )
{
  return search( text );
}

inline String const &RegEx::text( void ) const
{
  return _text;
}

inline Array<int> const &RegEx::offsets( void ) const
{
  return _offsets;
}

}

#endif
