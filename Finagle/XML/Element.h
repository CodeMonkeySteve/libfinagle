/*!
** \file Element.h
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

#ifndef FINAGLE_XML_ELEMENT_H
#define FINAGLE_XML_ELEMENT_H

#include <ext/functional>   // for compose1()
#include <Finagle/Map.h>
#include <Finagle/TextString.h>
#include <Finagle/XML/Node.h>
#include <Finagle/XML/Text.h>
#include <Finagle/XML/Iterator.h>

namespace Finagle {  namespace XML {

class Element : public Node {
public:
  typedef ObjectRef<Element> Ref;
  typedef ObjectRef<const Element> ConstRef;
  typedef Map<String, String> AttribMap;

  typedef XML::Iterator<Node>    Iterator;
  typedef XML::Iterator<Element> ElementIterator;
  typedef XML::ConstIterator<Node>    ConstIterator;
  typedef XML::ConstIterator<Element> ConstElementIterator;

  static const Element nil;

public:
  explicit Element( String const &name, String const &text = String() );
  Element( Element const &that );
  Element &operator =( Element const &that );
 ~Element();

  bool empty() const;
  operator bool( void ) const;
  bool hasChildren( void ) const;
  void render( std::ostream &out ) const;

  Node::ConstRef firstChild( void ) const;
  Node::ConstRef lastChild( void ) const;
  Node::Ref firstChild( void );
  Node::Ref lastChild( void );
  Node *firstChild( Node *child );
  Node *lastChild( Node *child );

  String const &name( void ) const;
  AttribMap const &attribs( void ) const;
  String const &attrib( String const &attrib ) const;
  String const &text( void ) const;
  String  const &operator[]( const char *attrib ) const;
  Element const &operator()( String const &name ) const;

  void name( String const &name );
  AttribMap &attribs( void );
  String &attrib( String const &attrib );
  String  &operator[]( const char *attrib );

  void clear( void );

  Element &append( String const &str );
  Element &append( Node &node );
  Element &append( Node::Ref node );

  template <typename T>  Element &operator <<( T t );
  template <typename T>  Element &operator <<( String const &str );
  template <typename T>  Element &operator <<( Element::Ref el );
  Element &operator +=( Element::Ref el );

protected:
  void insert( Node::Ref child );
  void remove( Node::Ref child );

  void openTag( std::ostream &out ) const;
  void closeTag( std::ostream &out ) const;

protected:
  String _name;
  AttribMap _attribs;
  Node::Ref _firstChild, _lastChild;
};

extern String escape( String const &str );

// INLINE IMPLEMENTATION **********************************************************************************************************

inline Element::Element( Element const &that )
: Node( that ), _name( that._name ), _attribs( that._attribs )
{}

inline Element::~Element( void )
{
  clear();
}

inline Element &Element::operator =( Element const &that )
{
  _name = that._name;
  _attribs = that._attribs;
  Node::operator =( that );
  return *this;
}


//! Returns \c true if the node has no attributes and no child nodes.
inline bool Element::empty() const
{
  return _attribs.empty() && !hasChildren();
}

inline Element::operator bool( void ) const
{
  return !empty();
}

inline bool Element::hasChildren( void ) const
{
  return _firstChild;
}


//! Returns the first child node (or \c 0, if this element has no children).
inline Node::ConstRef Element::firstChild( void ) const
{
  return Node::ConstRef(_firstChild);
}

//! Returns the last child node (or \c 0, if this element has no children).
inline Node::ConstRef Element::lastChild( void ) const
{
  return Node::ConstRef(_lastChild);
}

//! Returns the first child node (or \c 0, if this element has no children).
inline Node::Ref Element::firstChild( void )
{
  return _firstChild;
}

//! Returns the last child node (or \c 0, if this element has no children).
inline Node::Ref Element::lastChild( void )
{
  return _lastChild;
}


//! Inserts \a child as the first child node.
inline Node *Element::firstChild( Node *child )
{
  if ( !hasChildren() )
    insert( child );
  else
    child->insertBefore( _firstChild );

  return child;
}

//! Inserts \a child as the last child node.
inline Node *Element::lastChild( Node *child )
{
  if ( !_lastChild )
    insert( child );
  else
    child->insertAfter( _lastChild );

  return child;
}


inline String const &Element::name( void ) const
{
  return _name;
}

inline Element::AttribMap const &Element::attribs( void ) const
{
  return _attribs;
}

inline String const &Element::attrib( String const &attrib ) const
{
  return attribs()[attrib];
}

inline String const &Element::text( void ) const
{
  XML::Text::ConstRef t( _lastChild );
  return t ? t->text() : String::nil;
}

//! Attribute index
inline String const &Element::operator[]( const char *attrib ) const
{
  return Element::attrib(attrib);
}

//! Child element index
inline Element const &Element::operator()( String const &name ) const
{
  for ( ConstElementIterator i( firstChild() ); i; ++i ) {
    if ( i->name() == name )
      return i;
  }
  return nil;
}


inline void Element::name( String const &name )
{
  _name = name;
}

inline Element::AttribMap &Element::attribs( void )
{
  return _attribs;
}

inline String &Element::attrib( String const &attrib )
{
  return attribs()[attrib];
}

//! Attribute index
inline String &Element::operator[]( const char *attrib )
{
  return Element::attrib(attrib);
}


template <typename T>
inline Element &Element::operator <<( T t )
{
  std::ostringstream s;
  s << t;
  return append( s.str() );
}

inline Element &Element::append( Node &node )
{
  return append( &node );
}

template <>
inline Element &Element::operator <<( String const &str )
{
  return append( str );
}

template <>
inline Element &Element::operator <<( Element::Ref el )
{
  return append( el );
}

inline Element &Element::operator +=( Element::Ref el )
{
  return append( el );
}


inline void Element::closeTag( std::ostream &out ) const
{
  out << "</" << name() << ">";
}

} }

#endif