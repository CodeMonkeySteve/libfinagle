/*!
** \file Object.cpp
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

#include "Object.h"

using namespace Finagle;
using namespace XML;

/*!
** \class XML::Object
** \brief Abstract base class for %XML-configurable objects
*/

bool Object::configure( XML::Element const &config )
{
  if ( config.name() != objName() )
    return false;

  String const selfID( id() );
  return !selfID || (selfID == config[objIDAttrib()]);
}

XML::Element::Ref Object::configuration( void ) const
{
  XML::Element::Ref config( new XML::Element( objName() ) );
  String oid( id() );
  if ( oid )
    (*config)[objIDAttrib()] = oid;
  return config;
}
