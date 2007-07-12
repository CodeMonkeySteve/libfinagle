/*!
** \file XMLParse.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Fri Dec 16 2004
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

#include "AppLog.h"
#include "File.h"
#include "XMLParse.h"
#include "expat.h"

using namespace std;
using namespace Finagle;
using namespace XML;

Element::Ref XML::parse( FilePath const &file )
{
  ifstream in;

  in.open( file.path() );
  if ( !in )
    throw File::OpenEx( file, ios::in );

  return parse( in, file.path() );
}


static Array<Element::Ref> Elements;
static void XMLCALL elStart( void *, const char *Tag, const char **Attrs );
static void XMLCALL   elEnd( void *, const char *Tag );
static void XMLCALL  elData( void *, const XML_Char *Text, int Len );

Element::Ref XML::parse( istream &Stream, String SourceName )
{
  if ( !Stream )
    return( 0 );

  XML_Parser Parser = XML_ParserCreate( 0 );
  XML_SetElementHandler( Parser, elStart, elEnd );
  XML_SetCharacterDataHandler( Parser, elData );

  int Done = 0;
  while ( !Done ) {
    char Buff[512];
    Stream.read( Buff, sizeof( Buff ) );
    size_t BytesRead = Stream.gcount();

    Done = BytesRead < sizeof( Buff );
    if ( XML_Parse( Parser, Buff, BytesRead, Done ) == XML_STATUS_ERROR )
      throw ParseEx( SourceName, XML_GetCurrentLineNumber( Parser ), XML_ErrorString( XML_GetErrorCode( Parser ) ) );
  }

  XML_ParserFree( Parser );
  Element::Ref Root = Elements.front();
  Elements.clear();

  return( Root );
}


static void XMLCALL elStart( void *, const char *Tag, const char **Attrs )
{
  Element::Ref El = new Element( Tag );

  if ( !Elements.empty() )
    *(Elements.back()) += El;

  Elements.push_back( El );

  Element::AttribMap &Attribs = El->attribs();
  for ( const char **Attr = Attrs; *Attr; Attr += 2 )
    // Ignore case on attribute names
    Attribs.insert( String::toLower( Attr[0] ), Attr[1] );
}


static void XMLCALL elData( void *, const XML_Char *Text, int Len )
{
  Element::Ref El;
  String Str( Text, Len );

  // Ignore whitespace
  if ( Str.trim().empty() )
    return;

  El = Elements.empty() ? Element::Ref(new Element) : Elements.back();
  El->append( Str );
}


static void XMLCALL elEnd( void *, const char *Tag )
{
  FINAGLE_ASSERT( !Elements.empty() );
  FINAGLE_ASSERT( Elements.back()->tag() == Tag );

  if ( Elements.size() > 1 )
    Elements.pop_back();
}


ParseEx::ParseEx( String const &src, unsigned lineNum, String const &err )
: Exception( "XML (" + src + ": " + String( lineNum ) + "): " + err )
{
  attribs()["src"] = src;
  attribs()["line"] = String( lineNum );
}
