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

struct Context {
  Node::Ref first, last;
  Element::Ref cur;
};

static void elStart( void *ctxPtr, const char *name, const char **attrs );
static void elEnd( void *ctxPtr, const char *name );
static void elData( void *ctxPtr, const XML_Char *str, int len );


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

  _root->render( out );
}


//! \brief Parses the XML document from an input stream.
void Document::parse( istream &in, String const &srcName )
{
  _root = 0;
  if ( !in )
    return;

  Context ctx;
  XML_Parser parser = XML_ParserCreate(0);
  XML_SetUserData( parser, &ctx );
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

  _root = ctx.first;
}


static void elStart( void *ctxPtr, const char *name, const char **attrs )
{
  Context &ctx( *(Context *) ctxPtr );

  Element::Ref el = new Element(name);
  Element::AttribMap &attribs = el->attribs();
  for ( const char **attr = attrs; *attr; attr += 2 )
    attribs.insert( attr[0], attr[1] );

  if ( !ctx.first )
    ctx.first = ctx.last = Node::Ref(el);
  else
  if ( !ctx.cur ) {
    el->insertAfter( ctx.last );
    ctx.last = Node::Ref(el);
  } else
    ctx.cur->lastChild( Node::Ref(el) );

  ctx.cur = el;
}

static void elData( void *ctxPtr, XML_Char const *str, int len )
{
  Context &ctx( *(Context *) ctxPtr );
  String text( str, len );

  // Ignore whitespace (?)
  if ( text.trim().empty() )
    return;

  if ( !ctx.first )
    ctx.first = ctx.last = new Text(text);
  else
  if ( !ctx.cur ) {
    Text::Ref t( ctx.last );
    if ( t )
      t->text() += text;
    else {
      t = new Text(text);
      t->insertAfter( Node::Ref(ctx.last) );
      ctx.last = Node::Ref(t);
    }
  } else
    ctx.cur->append( text );
}

static void elEnd( void *ctxPtr, const char *name )
{
  Context &ctx( *(Context *) ctxPtr );

  FINAGLE_ASSERT( ctx.cur );
  FINAGLE_ASSERT( ctx.cur->name() == name );
  ctx.cur = Element::Ref(ctx.cur->parent());
}
