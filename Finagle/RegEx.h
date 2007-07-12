/*!
** \file RegEx.h
** \author Steve Sloan <steve@finagle.org>
** \date Wed Dec 15 2004
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
    Exception( const char *Exp, const char *ErrStr )
      : Finagle::Exception( String( "Regular expression \"" ) + Exp + "\" " + ErrStr ) {}
  };

public:
  RegEx( const char *RegExp, bool IgnoreCase = false, unsigned Opts = 0 );
 ~RegEx( void );

  bool operator()( const char *Text = 0 );
  String operator[]( unsigned SubMatch );

  bool search( const char *Text = 0, unsigned Opts = 0 );

protected:
  void clearMatchList( void );

protected:
  pcre *RExp;
  pcre_extra *RExtra;

public:
  String Text;
  Array<int> Offsets;
};

// INLINE IMPLEMENTATION ******************************************************

inline RegEx::RegEx( const char *Exp, bool IgnoreCase, unsigned Opts )
{
  const char *ErrStr;
  int         ErrOff;

  if ( IgnoreCase )
    Opts |= PCRE_CASELESS;

  RExp = pcre_compile( Exp, Opts | PCRE_UNGREEDY, &ErrStr, &ErrOff, 0 );
  if ( !RExp )
    throw Exception( Exp, ErrStr );

  RExtra = pcre_study( RExp, 0, &ErrStr );
  int c;
  pcre_fullinfo( RExp, RExtra, PCRE_INFO_CAPTURECOUNT, &c );
  Offsets.resize( (c + 1) * 3 );
}

inline RegEx::~RegEx( void )
{
  if ( RExtra )
    pcre_free( RExtra );

  pcre_free( RExp );
}

inline bool RegEx::operator()( const char *Text )
{
  return( search( Text ) );
}

inline bool RegEx::search( const char *Text, unsigned Opts )
{
  if ( Text )
    RegEx::Text = Text;

  int Res = pcre_exec( RExp, RExtra, RegEx::Text, RegEx::Text.size(), 0, Opts | PCRE_NOTEMPTY, &Offsets.front(), Offsets.size() );
  if ( Res > 0 )
    return true;
  else
  if ( Res < -1 )
    throw Exception( "", "" );

  return false;
}

inline String RegEx::operator[]( unsigned SubMatch )
{
  if ( SubMatch >= (Offsets.size() / 3) )
    return( String() );

  unsigned Offset = SubMatch * 2;
  int Start = Offsets[Offset], End = Offsets[Offset + 1];
  if ( (Start < 0) || (End <= Start) )
    return( String() );

  return( Text.substr( Start, End - Start ) );
}

}

#endif
