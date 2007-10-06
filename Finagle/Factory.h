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
//#include <Finagle/String.h>

namespace Finagle {

template <typename Name, typename Base>  class FactoryMap;

/*! \brief Defines a class which dynamically creates objects of a named class.
**
** This class must be equal-to or derived-from the \c Base class.  All factories have a name, of type \c Name (usually, although
** not necessarily, a #Finagle::String).
*/
template <typename Base>
class Factory {
public:
  Factory( void ) {}
  template <typename Name>  Factory( Name const &name, FactoryMap<Name, Base> &map );
  virtual ~Factory( void ) {}

  virtual ObjectRef<Base> operator()( void ) const = 0;
};

//! Defines a class which dynamically creates objects of type \a Class.
template <typename Class, typename Base = Class>
class ClassFactory : public Factory<Base> {
public:
  ClassFactory( void ) {}
  template <typename Name>  ClassFactory( Name const &name, class FactoryMap<Name, Base> &map );

  ObjectRef<Base> operator()( void ) const;
};

//! Maps class names to class factories
template <typename Name, typename Base>
class FactoryMap {
public:
  FactoryMap( void ) {}
  ObjectRef<Base> operator()( Name const &name );

protected:
  void insert( Name const &name, Factory<Base> &factory );
  friend class Factory<Base>;

protected:
  Map<Name, Factory<Base> *> _map;
};

// INLINE IMPLEMENTATION ******************************************************

/*! \brief Creates a new factory for class \c Base, known as \a name.
** The factory will be automatically added to the FactoryMap \a map.
*/
template <typename Base>
template <typename Name>
inline Factory<Base>::Factory( Name const &name, FactoryMap<Name, Base> &map )
{
  map.insert( name, *this );
}

/*! \brief Creates a new factory for class \c Class (derived from class \c Base), known as \a name.
** The factory will be automatically added to the FactoryMap \a map.
*/
template <typename Class, typename Base>
template <typename Name>
inline ClassFactory<Class, Base>::ClassFactory( Name const &name, class FactoryMap<Name, Base> &map )
: Factory<Base>( name, map )
{}


//! Instantiates a new object.
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

//! Instantiates a new object of class \a name.
template <typename Name, typename Base>
ObjectRef<Base> FactoryMap<Name, Base>::operator()( Name const &name )
{
  typename Map<Name, Factory<Base> *>::Iterator it( _map.find( name ) );
  return (it != _map.end()) ? (**it)() : 0;
}

}

#endif
