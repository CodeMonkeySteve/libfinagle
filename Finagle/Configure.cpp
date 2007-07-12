/*!
** \file Configure.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Fri Dec 31 2004
** Copyright (C) 2004-2006 by Steve Sloan <steve@inagle.org>
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

#include "Configure.h"
#include "Constructable.h"

using namespace Finagle;
using namespace XML;

/*!
** \class Configurable
**
*/

void Configurable::configure( XML::Element const &El )
{
  if ( !Tag.empty() && (Tag != El.tag()) )
    throw InvalidTagEx( El.tag(), Tag );
}


XML::Element::Ref Configurable::configuration( void ) const
{
  return( new XML::Element( Tag ) );
}


Configurable::Ref Configurable::deserialize( XML::Element const &El )
{
  if ( El.tag().empty() || !(Factories().contains( El.tag() )) )
    return( 0 );

  Configurable::Ref New = Factories()[El.tag()]->construct( El );
  return( New );
}
