/*!
** \file Object.h
** \author Steve Sloan <steve@finagle.org>
** \date Sun Dec 9 2007
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

#ifndef FINAGLE_XML_OBJECT_H
#define FINAGLE_XML_OBJECT_H

#include <Finagle/XML/Configurable.h>

namespace Finagle {  namespace XML {

class Object : public virtual Configurable {
public:
  typedef ObjectRef<Object> Ref;

public:
  Object( void ) {}
  Object( String const &oid );  //!< "Virtual" constructor
  Object( XML::Element const &config );
  virtual ~Object( void ) {}

  virtual XML::Element::Ref configuration( void ) const;
  virtual bool configure( XML::Element const &config );

  //! Returns the object's unique ID as a string.
  virtual String const &id( void ) const = 0;

  virtual String const &objName( void ) const = 0;
  virtual String const &objIDAttrib( void ) const = 0;
};

#define WITH_OBJECT_ELEMENT( TAG, ATTRIB )                                                          \
public:                                                                                             \
   virtual String const &objName( void ) const     {  static const String s(TAG);      return s;  } \
   virtual String const &objIDAttrib( void ) const {  static const String s(#ATTRIB);  return s;  } \
   virtual String const &id( void ) const          {  return (String const &) ATTRIB();  }


} }

#endif
