/*!
** \file Collection.h
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

#ifndef FINAGLE_COLLECTION_H
#define FINAGLE_COLLECTION_H

#include <Finagle/Configure.h>
#include <Finagle/Constructable.h>
#include <sigslot/sigslot.h>

namespace Finagle {

using namespace sigslot;

namespace XML {

template <typename Base = Configurable>
class Collectable : public Base {
public:
  typedef ObjectRef<Collectable> Ref;

  struct InvalidIDEx : public Exception {
    InvalidIDEx( String const &wrongID, String const &rightID );
  };

public:
  Collectable( String const &tag, String const &id = String() );

  String const &id( void ) const;

  void configure( XML::Element const &config );
  XML::Element::Ref configuration( void ) const;

protected:
  String _id;
};


template <typename Class, typename MapType = Map<String, ObjectRef<Class> > >
class Collection : public ObjectFactory<Class>, public MapType {
public:
  typedef MapType Map;
  typedef ObjectRefIterator<typename Map::Iterator> Iterator;
  typedef ObjectRefConstIterator<typename Map::ConstIterator> ConstIterator;

public:
  Collection( String const &Tag );

  Configurable::Ref construct( XML::Element const &El );
  XML::Element::Ref configuration( void ) const;

  void clear( void );

public:
  signal0<> Changed;
};

// INLINE/TEMPLATE IMPLEMENTATION *********************************************

template <typename Base>
inline Collectable<Base>::Collectable( String const &tag, String const &id )
: Base(tag), _id(id)
{}

template <typename Base>
inline String const &Collectable<Base>::id( void ) const
{
  return _id;
}

template <typename Base>
inline Collectable<Base>::InvalidIDEx::InvalidIDEx( String const &wrongID, String const &rightID )
: Exception( "Wrong ID: \"" + wrongID + "\" should be \"" + rightID + "\"" )
{}

template <typename Base>
void Collectable<Base>::configure( XML::Element const &config )
{
  Base::configure( config );

  if ( _id && config.attrib("id") && (config.attrib("id") != _id) )
    throw InvalidIDEx( config.attrib("id"), _id );
}

template <typename Base>
XML::Element::Ref Collectable<Base>::configuration( void ) const
{
  XML::Element::Ref config = Base::configuration();
  if ( !_id.empty() )
    config->attrib("id") = _id;

  return config;
}


template <typename Class, typename MapType>
inline Collection<Class, MapType>::Collection( String const &Tag )
: ObjectFactory<Class>( Tag )
{}

template <typename Class, typename MapType>
inline void Collection<Class, MapType>::clear( void )
{
  if ( Map::empty() )
    return;

  Map::clear();
  Changed();
}

template <typename Class, typename MapType>
Configurable::Ref Collection<Class, MapType>::construct( XML::Element const &El )
{
  if ( !El.attribs().contains( "id" ) )
    return ObjectFactory<Class>::construct( El );

  Configurable::Ref Obj;
  if ( Map::contains( El.attrib("id") ) ) {
    Obj = Configurable::Ref( Map::operator[]( El.attrib("id") ) );
    Obj->configure( El );
  } else {
    ObjectRef<Class> New = new Class( El.attrib("id") );
    Map::operator[]( El.attrib("id") ) = New;
    Obj = Configurable::Ref( New );
    Obj->configure( El );
    Changed();
  }

  return Obj;
}


template <typename Class, typename MapType>
XML::Element::Ref Collection<Class, MapType>::configuration( void ) const
{
  XML::Element::Ref El = new XML::Element( 0 );
  for ( ConstIterator Obj = Map::begin(); Obj != Map::end(); ++Obj )
    *El += Obj->configuration();

  return El;
}

} }

#endif
