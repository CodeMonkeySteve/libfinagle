/*!
** \file Document.h
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

#ifndef FINAGLE_XML_DOCUMENT_H
#define FINAGLE_XML_DOCUMENT_H

#include <Finagle/Exception.h>
#include <Finagle/FilePath.h>
#include <Finagle/XML/Element.h>

namespace Finagle {  namespace XML {

class Document {
public:
  typedef ObjectRef<Document> Ref;
  typedef ObjectRef<const Document> ConstRef;

public:
  Document( void );
  Document( FilePath const &src );
  explicit Document( String const &xml );

  FilePath const &src( void ) const;

  Node::ConstRef root( void ) const;
  Node::Ref root( void );

  void load( void );
  void save( void ) const;

  void parse( String const &in, String const &srcName = String() );
  void parse( std::istream &in, String const &srcName = String() );

protected:
  FilePath _src;
  Node::Ref _root;
};

//! %Exception thrown when %XML parsing fails.
struct ParseEx : public Exception {
  ParseEx( String const &src, unsigned line, String const &err );
};

extern std::istream &operator >>( std::istream &in,  Document &doc );
extern std::ostream &operator <<( std::ostream &out, Document const &doc );

// INLINE IMPLEMENTATION **********************************************************************************************************

inline Document::Document( void )
{}

//! Constructs a document and attempts to load it from the file \a src.
inline Document::Document( FilePath const &src )
: _src(src)
{
  load();
}

//! Constructs a document and attempts to load it from the string \a xml.
inline Document::Document( String const &xml )
{
  parse( xml );
}

//! Returns the document's source file path.
inline FilePath const &Document::src( void ) const
{
  return _src;
}

//! Returns the document's root node (may be \c 0).
inline Node::ConstRef Document::root( void ) const
{
  return Node::ConstRef(_root);
}

//! Returns the document's root node (may be \c 0).
inline Node::Ref Document::root( void )
{
  return _root;
}


//! \brief Parses the %XML document from a string.
inline void Document::parse( String const &in, String const &srcName )
{
  std::istringstream strm( in );
  parse( strm, srcName );
}


//! \brief Parses the %XML document from an input stream
inline std::istream &operator >>( std::istream &in, Document &doc )
{
  doc.parse( in );
  return in;
}

//! \brief Writes the %XML document to an output stream
inline std::ostream &operator <<( std::ostream &out, Document const &doc )
{
  doc.root()->render( out );
  return out;
}

inline ParseEx::ParseEx( String const &src, unsigned lineNum, String const &err )
: Exception( "XML (" + src + ": " + String( lineNum ) + "): " + err )
{
  attrib("src") = src;
  attrib("line") = String( lineNum );
}

} }

#endif
