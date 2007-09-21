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

template <typename Name, typename Base>  class FactoryMap;

template <typename Base>
class Factory {
public:
  Factory( void ) {}
  template <typename Name>  Factory( Name const &name, FactoryMap<Name, Base> &map );
  virtual ~Factory( void ) {}

  virtual ObjectRef<Base> operator()( void ) const = 0;
};

template <typename Class, typename Base = Class>
class ClassFactory : public Factory<Base> {
public:
  ClassFactory( void ) {}
  template <typename Name>  ClassFactory( Name const &name, class FactoryMap<Name, Base> &map );

  ObjectRef<Base> operator()( void ) const;
};

template <typename Name, typename Base>
class FactoryMap {
public:
  FactoryMap( void ) {}

  void insert( Name const &name, Factory<Base> &factory );

  ObjectRef<Base> operator()( Name const &name );

protected:
  Map<Name, Factory<Base> *> _map;
};

// INLINE IMPLEMENTATION ******************************************************

template <typename Base>
template <typename Name>
inline Factory<Base>::Factory( Name const &name, FactoryMap<Name, Base> &map )
{
  map.insert( name, *this );
}

template <typename Class, typename Base>
template <typename Name>
inline ClassFactory<Class, Base>::ClassFactory( Name const &name, class FactoryMap<Name, Base> &map )
: Factory<Base>( name, map )
{}


template <typename Class, typename Base>
inline ObjectRef<Base> ClassFactory<Class, Base>::operator()( void ) const
{
  return ObjectRef<Base>( (Base *) new Class );
}


template <typename Name, typename Base>
void FactoryMap<Name, Base>::insert( Name const &name, Factory<Base> &factory )
{
  _map.insert( name, &factory );
}

template <typename Name, typename Base>
ObjectRef<Base> FactoryMap<Name, Base>::operator()( Name const &name )
{
  typename Map<Name, Factory<Base> *>::Iterator it( _map.find( name ) );
  return (it != _map.end()) ? (**it)() : 0;
}

}

#endif
