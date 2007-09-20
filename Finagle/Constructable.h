/*!
** \file Constructable.h
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

#ifndef FINAGLE_CONSTRUCTABLE_H
#define FINAGLE_CONSTRUCTABLE_H

#include <Finagle/Configure.h>
#include <Finagle/Singleton.h>

namespace Finagle {  namespace XML {

class Constructable {
public:
  Constructable( String const &tag );
  virtual ~Constructable( void ) {}

  String const &tag( void ) const;

  virtual Configurable::Ref construct( XML::Element const &el ) = 0;

protected:
  String _tag;
};

template <typename Class>
class ObjectFactory : public Constructable {
public:
  ObjectFactory( String const &tag );
  Configurable::Ref construct( XML::Element const &el );
};

static Singleton<Map<String, Constructable *> > Factories;

// INLINE IMPLEMENTATION ******************************************************

inline Constructable::Constructable( String const &tag )
: _tag(tag)
{
  Factories()[tag] = this;
}

inline String const &Constructable::tag( void ) const
{
  return _tag;
}


template <typename Class>
inline ObjectFactory<Class>::ObjectFactory( String const &tag )
: Constructable( tag )
{}

template <typename Class>
inline Configurable::Ref ObjectFactory<Class>::construct( XML::Element const &el )
{
  Configurable::Ref obj = new Class;
  obj->configure( el );
  return obj;
}

} }

#endif
