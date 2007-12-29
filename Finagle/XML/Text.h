/*!
** \file Text.h
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

#ifndef FINAGLE_XML_TEXT_H
#define FINAGLE_XML_TEXT_H

#include <Finagle/XML/Node.h>

namespace Finagle {  namespace XML {

//! \brief Represents a text node
class Text : public Node {
public:
  typedef ObjectPtr<Text> Ptr;
  typedef ObjectPtr<Text const> ConstPtr;

public:
  Text( String const &text = String() );
  virtual Node::Ptr dup( void ) const;

  String const &text( void ) const;
  String &text( void );

  void clear( void );

  void render( std::ostream &out ) const;
  operator String( void ) const;

protected:
  String _text;
};

// INLINE IMPLEMENTATION **********************************************************************************************************

//! Construct a text node with the given \a text content.
inline Text::Text( String const &text )
: _text(text)
{}

inline Node::Ptr Text::dup( void ) const
{
  return new Text( *this );
}

//! Returns the node's content.
inline String const &Text::text( void ) const
{
  return _text;
}

//! Returns the node's content.
inline String &Text::text( void )
{
  return _text;
}

inline void Text::clear( void )
{
  _text.clear();
}

inline void Text::render( std::ostream &out ) const
{
  if ( !_text.empty() )
    out << _text;
}

inline Text::operator String( void ) const
{
  return _text;
}


} }

#endif
