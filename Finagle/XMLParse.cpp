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

static Array<Element::Ref> elements;
static void elStart( void *, const char *tag, const char **attrs );
static void elEnd( void *, const char *tag );
static void elData( void *, const XML_Char *text, int len );

/*! \brief Parses an XML document from a file
** \return the document root element
*/
Element::Ref XML::parse( FilePath const &file )
{
  ifstream in;

  in.open( file.path() );
  if ( !in )
    throw File::OpenEx( file, ios::in );

  return parse( in, file.path() );
}

ParseEx::ParseEx( String const &src, unsigned lineNum, String const &err )
: Exception( "XML (" + src + ": " + String( lineNum ) + "): " + err )
{
  attribs()["src"] = src;
  attribs()["line"] = String( lineNum );
}

/*! \brief Parses an XML document from an input stream
** \a srcName is used only for error messages, and is optional.
** \return the document root element
*/
Element::Ref XML::parse( std::istream &in, String srcName )
{
  if ( !in )
    return( 0 );

  XML_Parser parser = XML_ParserCreate( 0 );
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
  Element::Ref root( elements.front() );
  elements.clear();

  return root;
}


static void elStart( void *, const char *tag, const char **attrs )
{
  Element::Ref xml = new Element(tag);

  if ( !elements.empty() )
    *(elements.back()) += xml;

  elements.push_back( xml );

  Element::AttribMap &attribs = xml->attribs();
  for ( const char **attr = attrs; *attr; attr += 2 )
    // Ignore case on attribute names
    attribs.insert( String::toLower( attr[0] ), attr[1] );
}

static void elData( void *, XML_Char const *text, int len )
{
  Element::Ref xml;
  String t( text, len );

  // Ignore whitespace
  if ( t.trim().empty() )
    return;

  xml = elements.empty() ? Element::Ref(new Element) : elements.back();
  xml->append( t );
}

static void elEnd( void *, const char *tag )
{
  FINAGLE_ASSERT( !elements.empty() );
  FINAGLE_ASSERT( elements.back()->tag() == tag );

  if ( elements.size() > 1 )
    elements.pop_back();
}
