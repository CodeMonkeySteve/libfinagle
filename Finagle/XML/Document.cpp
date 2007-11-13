/*!
** \file Document.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Mon Nov 12 2007
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

#include "Document.h"
#include "Finagle/AppLog.h"
#include "Finagle/File.h"
#include "expat.h"

using namespace std;
using namespace Finagle;
using namespace XML;

static Array<Element::Ref> Elements;
static void elStart( void *, const char *name, const char **attrs );
static void elEnd( void *, const char *name );
static void elData( void *, const XML_Char *text, int len );


//! \brief Loads the XML document from #src.
void Document::load( void )
{
  ifstream in;

  in.open( _src.path() );
  if ( !in )
    throw File::OpenEx( _src, ios::in );

  parse( in, _src.path() );
}


//! \brief Saves the XML document to #src.
void Document::save( void ) const
{
  ofstream out;

  out.open( _src.path() );
  if ( !out )
    throw File::OpenEx( _src, ios::out );

  render( out );
}


//! \brief Parses the XML document from a string
void Document::parse( String const &in, String const &srcName )
{
  clear();
  if ( in.empty() )
    return;

  XML_Parser parser = XML_ParserCreate(0);
  XML_SetElementHandler( parser, elStart, elEnd );
  XML_SetCharacterDataHandler( parser, elData );

//  int done = 1;
  if ( XML_Parse( parser, in, in.length(), 1 ) == XML_STATUS_ERROR )
    throw ParseEx( srcName, XML_GetCurrentLineNumber( parser ), XML_ErrorString( XML_GetErrorCode( parser ) ) );

  XML_ParserFree( parser );

  Element::operator=( Elements.front() );
  Elements.clear();
}


//! \brief Parses the XML document from an input stream
void Document::parse( std::istream &in, String const &srcName )
{
  if ( !in )
    return;

  XML_Parser parser = XML_ParserCreate(0);
  XML_SetElementHandler( parser, elStart, elEnd );
  XML_SetCharacterDataHandler( parser, elData );

  int done = 0;
  while ( !done ) {
    char buff[512];
    in.read( buff, sizeof( buff ) );
    size_t bytesRead = in.gcount();

    done = bytesRead < sizeof( buff );
    if ( XML_Parse( parser, buff, bytesRead, done ) == XML_STATUS_ERROR )
      throw ParseEx( srcName, XML_GetCurrentLineNumber( parser ), XML_ErrorString( XML_GetErrorCode( parser ) ) );
  }

  XML_ParserFree( parser );

  Element::operator=( Elements.front() );
  Elements.clear();
}


static void elStart( void *, const char *name, const char **attrs )
{
  Element::Ref xml = new Element(name);

  if ( !Elements.empty() )
    *(Elements.back()) += xml;

  Elements.push_back( xml );

  Element::AttribMap &attribs = xml->attribs();
  for ( const char **attr = attrs; *attr; attr += 2 )
    attribs.insert( attr[0], attr[1] );
}

static void elData( void *, XML_Char const *text, int len )
{
  Element::Ref xml;
  String t( text, len );

  // Ignore whitespace
  if ( t.trim().empty() )
    return;

  xml = Elements.empty() ? Element::Ref(new Element) : Elements.back();
  xml->append( t );
}

static void elEnd( void *, const char *name )
{
  FINAGLE_ASSERT( !Elements.empty() );
  FINAGLE_ASSERT( Elements.back()->name() == name );

  if ( Elements.size() > 1 )
    Elements.pop_back();
}
