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

#include <Finagle/Map.h>
#include <Finagle/TextString.h>
#include <Finagle/XML/NodeList.h>
#include <Finagle/XML/Text.h>

namespace Finagle {  namespace XML {

class Element : public Node, public NodeList {
public:
  typedef ObjectRef<Element> Ref;
  typedef ObjectRef<const Element> ConstRef;
  typedef Map<String, String> AttribMap;

  typedef XML::Iterator<Element>      ElementIterator;
  typedef XML::ConstIterator<Element> ConstElementIterator;

  static const Element nil;

public:
  explicit Element( String const &name, String const &text = String() );
  Element( Element const &that );
  virtual Node::Ref dup( void ) const;
 ~Element();
  Element &operator =( Element const &that );

  operator bool( void ) const;
  bool empty() const;
  bool hasChildren( void ) const;
  void render( std::ostream &out ) const;

  String const &name( void ) const;
  AttribMap const &attribs( void ) const;
  String const &attrib( String const &attrib ) const;
  String  const &operator[]( const char *attrib ) const;
  Element const &operator()( String const &name ) const;

  void name( String const &name );
  AttribMap &attribs( void );
  String &attrib( String const &attrib );
  String &operator[]( const char *attrib );

  Element &append( String const &str ) {  return (Element &) NodeList::append(str);  }
  Element &append( Node &node )        {  return (Element &) NodeList::append(node);  }
  Element &append( Node::Ref node )    {  return (Element &) NodeList::append(node);  }

  template <typename T>  Element &operator <<( T t );
  template <typename T>  Element &operator <<( String const &str );
  template <typename T>  Element &operator <<( Element::Ref el );
  Element &operator +=( Element::Ref el );

  void clear( void );

protected:
  virtual void insert( Node::Ref node );
  virtual void remove( Node::Ref node );

  void openTag( std::ostream &out ) const;
  void closeTag( std::ostream &out ) const;

protected:
  String _name;
  AttribMap _attribs;
};

extern String escape( String const &str );

// INLINE IMPLEMENTATION **********************************************************************************************************

//! Copy constructor
inline Element::Element( Element const &that )
: NodeList(that),
  _name( that._name ), _attribs( that._attribs )
{}

inline Element::~Element( void )
{
  clear();
}

//! Assignment operator
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

//! Returns \c true if the element has a name (i.e. is not #nil).
inline Element::operator bool( void ) const
{
  return ! _name.empty();
}

//! Returns \c true if the element contains child nodes.
inline bool Element::hasChildren( void ) const
{
  return ! NodeList::empty();
}


//! Returns the element's tag name.
inline String const &Element::name( void ) const
{
  return _name;
}

//! Returns the element's attributes.
inline Element::AttribMap const &Element::attribs( void ) const
{
  return _attribs;
}

//! Returns the value of the attribute \a attrib.
inline String const &Element::attrib( String const &attrib ) const
{
  return attribs()[attrib];
}

/*! \brief Attribute index
**
** \sa attrib.
*/
inline String const &Element::operator[]( const char *attrib ) const
{
  return Element::attrib(attrib);
}

/*! \brief Child element index
**
** Returns a reference to the first child node with name \a name.  If no such child node exists, returns a reference
** to Element::nil.
*/
inline Element const &Element::operator()( String const &name ) const
{
  for ( ConstElementIterator i( first() ); i; ++i ) {
    if ( i->name() == name )
      return i;
  }
  return nil;
}


//! Sets the element's tag name.
inline void Element::name( String const &name )
{
  _name = name;
}

//! Returns the element's attributes.
inline Element::AttribMap &Element::attribs( void )
{
  return _attribs;
}

//! Returns the value of the attribute \a attrib.
inline String &Element::attrib( String const &attrib )
{
  return attribs()[attrib];
}

/*! \brief Attribute index
**
** \sa attrib.
*/
inline String &Element::operator[]( const char *attrib )
{
  return Element::attrib(attrib);
}

/*! \brief Appends any object \a t (as a string).
**
** \sa append.
*/
template <typename T>
inline Element &Element::operator <<( T t )
{
  std::ostringstream s;
  s << t;
  return (Element &) append( s.str() );
}

/*! \brief Appends the string \a str.
**
** \sa append.
*/
template <>
inline Element &Element::operator <<( String const &str )
{
  return (Element &) append( str );
}

/*! \brief Appends the element \a el.
**
** \sa append.
*/
template <>
inline Element &Element::operator <<( Element::Ref el )
{
  return (Element &) append( Node::Ref(el) );
}

/*! \brief Appends the element \a el.
**
** \sa append.
*/
inline Element &Element::operator +=( Element::Ref el )
{
  return (Element &) append( Node::Ref(el) );
}


/*! \internal
** \brief Renders the node's closing tag.
*/
inline void Element::closeTag( std::ostream &out ) const
{
  out << "</" << name() << ">";
}

} }

#endif
