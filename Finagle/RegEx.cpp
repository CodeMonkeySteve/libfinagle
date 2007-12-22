/*!
** \file RegEx.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Wed Dec 15 2004
** Copyright (C) 2007 by Steve Sloan
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

#include "RegEx.h"

using namespace std;
using namespace Finagle;

/*!
** \class Finagle::RegEx
** \brief Holds a regular expression (via \c pcre)
*/

RegEx::RegEx( const char *regExp, bool ignoreCase, unsigned optFlags )
: _exp(0), _extra(0)
{
  if ( ignoreCase )
    optFlags |= PCRE_CASELESS;

  const char *errStr;
  int errOff;
  _exp = pcre_compile( regExp, optFlags | PCRE_UNGREEDY, &errStr, &errOff, 0 );
  if ( !_exp )
    throw Exception( regExp, errStr );

  _extra = pcre_study( _exp, 0, &errStr );
  int c;
  pcre_fullinfo( _exp, _extra, PCRE_INFO_CAPTURECOUNT, &c );
  _offsets.resize( (c + 1) * 3 );
}

bool RegEx::search( const char *text, unsigned optFlags )
{
  if ( text )
    _text = String(text);

  int res = pcre_exec( _exp, _extra, _text, _text.size(), 0, optFlags | PCRE_NOTEMPTY, &_offsets.front(), _offsets.size() );
  if ( res < -1 )
    throw Exception( "", "" );

  return res > 0;
}

String RegEx::operator[]( unsigned subMatch ) const
{
  if ( subMatch >= (_offsets.size() / 3) )
    return String();

  unsigned offset = subMatch * 2;
  int start = _offsets[offset], end = _offsets[offset + 1];
  if ( (start < 0) || (end <= start) )
    return String();

  return _text.substr( start, end - start );
}
