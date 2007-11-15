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

/*! \class Finagle::XML::Element
** \brief Represents an element node.
**
** An Element Node may have, in addition to siblings and a parent (as provided by Node), any number of attributes and child nodes.
** It also provides convenience functions for searching child nodes, and for accessing a child Text node.
*/

/*! \brief A do-nothing element.
**
** \sa Element::operator().
*/
const Element Element::nil( "" );

//! Initializes the element with \a name and (optionally) a Text node containing \a text.
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


/*! \brief Appends the text in \a str to a child Text node.
**
** If the element's last node is a Text node, appends the text to that.  Otherwise, creates a new Text node and inserts
** it into the element as the last child node.
*/
Element &Element::append( String const &str )
{
  Text::Ref textNode( _lastChild );
  if ( textNode )
    textNode->text() += str;
  else
    lastChild( new Text(str) );

  return *this;
}

/*! \brief Appends \a node as a child node.
**
** If \a node is a Text node, and the element's last node is also a Text node, appends the text in \a node to the element's
** Text node.  Otherwise, inserts \a node into the element as the last child node.
*/
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


/*! \internal
** \brief Renders the element's openning tag (including attributes).
*/
void Element::openTag( std::ostream &out ) const
{
  out << "<" << name();

  if ( !attribs().empty() ) {
    for ( Element::AttribMap::ConstIterator e = attribs().begin(); e != attribs().end(); ++e )
      out << " " << e.key() << "='" << XML::escape( e.val() ) << "'";
  }

  out << ">";
}


//! Utility function to escape a string using %XML entities, such that it's suitable for representing an attribute value.
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
