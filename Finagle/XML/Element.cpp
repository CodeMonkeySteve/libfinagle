/*!
** \file Element.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Sun Nov 11 2007
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

#include "Element.h"

using namespace std;
using namespace Finagle;
using namespace XML;

const Element Element::nil( "" );


Element::Element( String const &name, String const &text )
: _name(name)
{
  if ( !text.empty() )
    insert( new Text(text) );
}


//! Removes all attributes and child nodes
void Element::clear( void )
{
  for ( Node::Ref child( _firstChild ), next; child; child = next ) {
    next = child->next();
    Node::remove( child );
  }

  _firstChild = _lastChild = 0;
  Node::clear();
}


Element &Element::append( String const &str )
{
  Text::Ref textNode( _lastChild );
  if ( textNode )
    textNode->text() += str;
  else
    lastChild( new Text(str) );

  return *this;
}

Element &Element::append( Node::Ref node )
{
  Text::Ref a( _lastChild ), b( node );
  if ( a && b )
    a->text() += b->text();
  else
    lastChild( node );

  return *this;
}


void Element::render( std::ostream &out ) const
{
  out << "<" << name();

  if ( !_attribs.empty() ) {
    for ( AttribMap::ConstIterator e = _attribs.begin(); e != _attribs.end(); ++e )
      out << " " << e.key() << "='" << escape( e.val() ) << "'";
  }

  if ( !_lastChild ) {
    out << "/>";
    return;
  }
  out << ">";

  for ( ConstIterator n( _firstChild ); n; ++n )
    n->render( out );

  closeTag( out );
}


void Element::insert( Node::Ref child )
{
  Node::insert( child );

  if ( !child->prev() )
    _firstChild = child;

  if ( !child->next() )
    _lastChild = child;
}

void Element::remove( Node::Ref child )
{
  if ( _firstChild == child )
    _firstChild = child->next();

  if ( _lastChild == child )
    _lastChild = child->prev();

  Node::remove( child );
}


void Element::openTag( std::ostream &out ) const
{
  out << "<" << name();

  if ( !attribs().empty() ) {
    for ( Element::AttribMap::ConstIterator e = attribs().begin(); e != attribs().end(); ++e )
      out << " " << e.key() << "='" << XML::escape( e.val() ) << "'";
  }

  out << ">";
}


String XML::escape( String const &str )
{
  String s;
  for ( String::const_iterator i = str.begin(); i != str.end(); ++i ) {
    unsigned char c = *i;
    if ( (c < 32) && (c != 9) && (c != 10) && (c != 13) )
      continue;

         if ( c == '&' )  s += "&amp;";
    else if ( c == '<' )  s += "&lt;";
    else if ( c == '\'' ) s += "&apos;";
    else if ( (c < 32) || (c > 127) || (c == 39) )
      s += String::format( "&#%i;", c );
    else
      s += c;
  }

  return( s );
}


/*
void XML::Element::prettyRender( std::ostream &out, unsigned indent ) const
{
  if ( _name.empty() && _text.empty() && _elements.empty() )
    return;

  out << String( indent * 2, ' ' );

  if ( _name.empty() ) {
    // Text-only element
    out << escape( _text ) << endl;
    return;
  }

  out << "<" << _name;

  if ( !_attribs.empty() ) {
    for ( AttribMap::ConstIterator e = _attribs.begin(); e != _attribs.end(); ++e )
      out << " " << e.key() << "='" << escape( e.val() ) << "'";
  }

  if ( !_text.empty() ) {
    out << ">" << endl
        << String( (indent + 1) * 2, ' ' ) << escape( _text ) << endl;
    closeTag( out );
    out << endl;
  } else
  if ( !_elements.empty() ) {
    out << ">" << endl;

    for ( List::ConstIterator e = _elements.begin(); e != _elements.end(); ++e )
      e->prettyRender( out, indent + 1 );

    out << String( indent * 2, ' ' );
    closeTag( out );
    out << endl;
  } else
    out << " />" << endl;
}
*/

/*
void Element::dump( unsigned indent ) const
{
  if ( name().empty() && text().empty() && elements().empty() )
    return;

  cerr << String( indent * 2, ' ' );
  if ( !name().empty() ) {
    cerr << name() << ":";

    for ( XML::Element::AttribMap::ConstIterator a = attribs().begin(); a != attribs().end(); ++a )
      cerr << " " << a.key() << "='" << *a << "'";
  } else {
    FINAGLE_ASSERT( elements().empty() );
    cerr << '"' << text() << '"';
  }

  cerr << endl;

  for ( XML::Element::ConstIterator e = elements().begin(); e != elements().end(); ++e )
    e->dump( indent + 1 );
}
*/
