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

#ifndef FINAGLE_XML_COLLECTION_H
#define FINAGLE_XML_COLLECTION_H

#include <Finagle/XML/Object.h>

namespace Finagle {  namespace XML {

template <typename Class, typename MapType = Map<String, ObjectRef<Class> > >
class Collection : public Configurable, public MapType {
public:
  typedef MapType Map;
  typedef ObjectRefIterator<typename Map::Iterator> Iterator;
  typedef ObjectRefConstIterator<typename Map::ConstIterator> ConstIterator;

public:
  Collection( String const &tag );

  bool configure( XML::Element const &config );
  XML::Element::Ref configuration( void ) const;

protected:
  String _tag;
};

// INLINE/TEMPLATE IMPLEMENTATION *********************************************

template <typename Class, typename MapType>
inline Collection<Class, MapType>::Collection( String const &tag )
: _tag( tag )
{}

template <typename Class, typename MapType>
bool Collection<Class, MapType>::configure( XML::Element const &config )
{
  static const String ObjName( Class().objName() );
  static const String ObjIDAttrib( Class().objIDAttrib() );

  String const &elName( config.name() );
  if ( elName == ObjName ) {
    String oid( config[ObjIDAttrib] );
    Iterator obj( MapType::find( oid ) );

    if ( (obj == MapType::end()) || !obj->configure( config ) ) {
      ObjectRef<Class> newObj( new Class( oid ) );
      MapType::insert( oid, newObj );
      newObj->configure( config );
    }
    return true;
  }

  if ( elName == _tag ) {
    for ( XML::Element::ConstElementIterator el( config.first() ); el; ++el ) {
      if ( !configure( *el ) )
        return false;
    }
    return true;
  }

  return false;
}

template <typename Class, typename MapType>
XML::Element::Ref Collection<Class, MapType>::configuration( void ) const
{
  XML::Element::Ref config( new XML::Element( _tag ) );
  for ( ConstIterator obj( MapType::begin() ); obj != MapType::end(); ++obj )
    *config << obj->configuration();
  return config;
}

} }

#endif
