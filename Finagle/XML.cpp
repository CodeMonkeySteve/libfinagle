/*!
** \file XML.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Mon Nov 1 2004
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

#include <sstream>
#include "XML.h"

using namespace std;
using namespace Finagle;
using namespace XML;

const Element Element::nil;


static String escape( String const &str )
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


//! Returns the first child element with \a tag.
Element &Element::element( String const &tag )
{
  String const &(XML::Element::*f)(void) const = &XML::Element::tag;

  Iterator i = std::find_if( _elements.begin(), _elements.end(),
    compose1( bind2nd( std::equal_to<String>(), tag ), mem_fun_ref(f) )
  );

  if ( i != _elements.end() )
    return *i;

  Element::Ref el = new Element( tag );
  append( el );
  return el;
}


void Element::openTag( ostream &out ) const
{
  out << "<" << tag();

  if ( !attribs().empty() ) {
    for ( Element::AttribMap::ConstIterator e = attribs().begin(); e != attribs().end(); ++e )
      out << " " << e.key() << "='" << escape( e.val() ) << "'";
  }

  out << ">";
}


void Element::closeTag( ostream &out ) const
{
  out << "</" << tag() << ">";
}


void Element::render( ostream &out ) const
{
  if ( tag().empty() ) {
    // Text-only element
    out << escape( text() );
    return;
  }

  out << "<" << tag();

  if ( !_attribs.empty() ) {
    for ( AttribMap::ConstIterator e = _attribs.begin(); e != _attribs.end(); ++e )
      out << " " << e.key() << "='" << escape( e.val() ) << "'";
  }

  if ( !_text.empty() ) {
    out << ">" << escape( _text );
    closeTag( out );
  } else
  if ( !_elements.empty() ) {
    out << ">";
    for ( List::ConstIterator e = _elements.begin(); e != _elements.end(); ++e ) {
      e->render( out );
    }
    closeTag( out );
  } else
    out << " />";
}


Element &Element::append( String const &str )
{
  if ( !_elements.empty() ) {
    if ( _elements.back()->_tag.empty() )
      _elements.back()->_text += str;
    else
      _elements.push_back( new Element( String(), str ) );
  } else
    _text += str;

  return *this;
}


Element &Element::append( Element const &el )
{
  if ( el._tag.empty() && !el._text.empty() )
    append( el._text );
  else {
    if ( _elements.empty() && !_text.empty() ) {
      _elements.push_back( new Element( String(), _text ) );
      _text.clear();
    }

    if ( el._tag.empty() )
      _elements.insert( _elements.end(), el._elements.begin(), el._elements.end() );
    else
      _elements.push_back( new Element( el ) );
  }

  return *this;
}


Element &Element::append( Element::Ref el )
{
  if ( el->_tag.empty() && !el->_text.empty() )
    append( el->_text );
  else {
    if ( _elements.empty() && !_text.empty() ) {
      _elements.push_back( new Element( String(), _text ) );
      _text.clear();
    }

    if ( el->_tag.empty() )
      _elements.insert( _elements.end(), el->_elements.begin(), el->_elements.end() );
    else
      _elements.push_back( el );
  }

  return *this;
}


void XML::Element::prettyRender( ostream &out, unsigned indent ) const
{
  if ( _tag.empty() && _text.empty() && _elements.empty() )
    return;

  out << String( indent * 2, ' ' );

  if ( _tag.empty() ) {
    // Text-only element
    out << escape( _text ) << endl;
    return;
  }

  out << "<" << _tag;

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

#include <iostream>
#include "AppLog.h"

void XML::Element::dump( unsigned indent ) const
{
  if ( tag().empty() && text().empty() && elements().empty() )
    return;

  cerr << String( indent * 2, ' ' );
  if ( !tag().empty() ) {
    cerr << tag() << ":";

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
