/*!
** \file Configure.h
** \author Steve Sloan <steve@finagle.org>
** \date Fri Dec 31 2004
** Copyright (C) 2004 by Steve Sloan <steve@inagle.org>
**
** This program is free software; you can redistribute it and/or modify it
** under the terms of the GNU General Public License as published by the
** Free Software Foundation; either version 2 of the License, or (at your
** option) any later version.
**
** This program is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this library; if not, you may access it via the web at
** http://www.gnu.org/copyleft/gpl.html
*/

#ifndef FINAGLE_CONFIGURE_H
#define FINAGLE_CONFIGURE_H

#include <Finagle/XML.h>
#include <Finagle/Exception.h>

namespace Finagle {  namespace XML {

class Configurable : virtual public ReferenceCount {
public:
  typedef ObjectRef<Configurable> Ref;

public:
  typedef Finagle::Exception Exception;
  struct InvalidTagEx : public Exception {
    InvalidTagEx( const char *tag, const char *wrongTag );
  };
  struct InvalidAttribEx : public Exception {
    InvalidAttribEx( const char *tag, const char *attrib, const char *wrongVal, const char *rightVal );
  };

public:
  Configurable( String const &tag );
  virtual ~Configurable( void ) {}

  String const &tag( void ) const;

  virtual void configure( XML::Element const &el );
  virtual XML::Element::Ref configuration( void ) const;

public:
  static Configurable::Ref deserialize( XML::Element const &el );

protected:
  String _tag;
};

// INLINE IMPLEMENTATION ******************************************************

inline Configurable::Configurable( String const &tag )
: _tag( tag )
{}

inline String const &Configurable::tag( void ) const
{
  return _tag;
}

inline Configurable::InvalidTagEx::InvalidTagEx( const char *tag, const char *wrongTag )
: Exception( String( "Wrong tag for type " ) + tag + ": \"" + wrongTag + "\"" )
{}

inline Configurable::InvalidAttribEx::InvalidAttribEx( const char *tag, const char *attrib, const char *wrongVal, const char *rightVal )
: Exception( String( tag ) + " has invalid attribute \"" + attrib + "\" -- \"" + wrongVal + "\" should be " + rightVal )
{}

} }

#endif
