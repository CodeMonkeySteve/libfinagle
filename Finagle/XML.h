/*!
** \file XML.h
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

#ifndef FINAGLE_XML_H
#define FINAGLE_XML_H

#include <ostream>
#include <sstream>
#include <ext/functional>   // for compose1()
#include <Finagle/Map.h>
#include <Finagle/ReferencedObject.h>

#include <iostream>

namespace Finagle {  namespace XML {

//#include <tinyxml/tinyxml.h>

class Element : public ReferenceCount {
public:
  typedef ObjectRef<Element> Ref;
  typedef ObjectRef<const Element> ConstRef;

public:
  typedef Map<String, String> AttribMap;

  class List : public Finagle::List<Element::Ref> {
  public:
    typedef ObjectRefIterator<List::Iterator> Iterator;
    typedef ObjectRefConstIterator<List::ConstIterator> ConstIterator;

  public:
    List( void ) {}
    bool contains( String const &tag ) const;
  };
  typedef List::Iterator Iterator;
  typedef List::ConstIterator ConstIterator;

public:
  explicit Element( String const &tag = String(), String const &text = String() );
  Element( Element const &that );
  bool empty() const;
  operator bool( void ) const;

  String const &tag( void ) const;
  AttribMap const &attribs( void ) const;
  String const &text( void ) const;
  List const &elements( void ) const;
  String const &attrib( String const &attribKey ) const;
  Element const &element( String const &tag ) const;

  void tag( String const &tag );
  AttribMap &attribs( void );
  String &text( void ) ;
  List &elements( void );
  String &attrib( String const &attribKey );
  Element &element( String const &tag );

  Element &append( String const &str );
  Element &append( Element const &el );
  Element &append( Element::Ref el );

  template <typename T>  Element &operator << ( T t );
  template <typename T>  Element &operator << ( String const &str );
  template <typename T>  Element &operator << ( Element::Ref el );
  Element &operator +=( Element::Ref El );

  void openTag( std::ostream &out ) const;
  void closeTag( std::ostream &out ) const;

public:
  void render( std::ostream &out ) const;
  void prettyRender( std::ostream &out, unsigned indent = 0 ) const;
  void dump( unsigned indent = 0 ) const;

public:
  static const Element nil;

protected:
  String _tag;
  AttribMap _attribs;
  String _text;
  List _elements;
};

std::ostream &operator <<( std::ostream &out, Element const &el );

// INLINE IMPLEMENTATION **********************************************************************************************************

inline bool Element::List::contains( String const &tag ) const
{
  String const &(XML::Element::*f)(void) const = &XML::Element::tag;
  return
    std::find_if( List::begin(), List::end(),
      compose1( bind2nd( std::equal_to<String>(), tag ), mem_fun_ref(f) )
    ) != List::end();
}


inline Element::Element( String const &tag, String const &text )
: _tag(tag), _attribs(), _text(text), _elements()
{}

inline Element::Element( Element const &that )
: ReferenceCount(),
  _tag( that._tag ), _attribs( that._attribs ), _text( that._text ), _elements( that._elements )
{}

inline bool Element::empty() const
{
  return _tag.empty() && _text.empty() && _attribs.empty() && _elements.empty();
}

inline Element::operator bool( void ) const
{
  return !empty();
}


inline String const &Element::tag( void ) const
{
  return _tag;
}

inline Element::AttribMap const &Element::attribs( void ) const
{
  return _attribs;
}

inline String const &Element::text( void ) const
{
  return _text;
}

inline Element::List const &Element::elements( void ) const
{
  return _elements;
}

inline String const &Element::attrib( String const &attribKey ) const
{
  return const_cast<Element*>(this)->attrib(attribKey);
}

inline Element const &Element::element( String const &tag ) const
{
  return const_cast<Element*>(this)->element(tag);
}


inline void Element::tag( String const &tag )
{
  _tag = tag;
}

inline Element::AttribMap &Element::attribs( void )
{
  return _attribs;
}

inline String &Element::text( void )
{
  return _text;
}

inline Element::List &Element::elements( void )
{
  return _elements;
}

inline String &Element::attrib( String const &attribKey )
{
  return _attribs[attribKey];
}


template <typename T>
inline Element &Element::operator <<( T t )
{
  std::ostringstream s;
  s << t;
  return append( s.str() );
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


inline std::ostream &operator <<( std::ostream &out, Element const &el )
{
  el.render( out );
  return out << std::flush;
}

} }

#endif
