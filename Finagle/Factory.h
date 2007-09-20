/*!
** \file Factory.h
** \author Steve Sloan <steve@finagle.org>
** \date Thu Sep 20 2007
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

#ifndef FINAGLE_FACTORY_H
#define FINAGLE_FACTORY_H

#include <Finagle/Map.h>
#include <Finagle/ReferencedObject.h>

namespace Finagle {

template <typename Base>
class Factory {
public:
  Factory( void ) {}
  virtual ~Factory( void ) {}

  virtual ObjectRef<Base> create( void ) const = 0;
};

template <typename Class, typename Base = Class>
class ClassFactory : public Factory<Base> {
public:
  ClassFactory( void ) {}

  ObjectRef<Base> create( void ) const;
};

template <typename Name, typename Base>
class FactoryMap : public Map<Name, Factory<Base> *> {
public:
  FactoryMap( void ) {}
};

// INLINE IMPLEMENTATION ******************************************************

template <typename Class, typename Base>
inline ObjectRef<Base> ClassFactory<Class, Base>::create( void ) const
{
  return ObjectRef<Base>( (Base *) new Class );
}

}

#endif
