/*!
** \file Configurable.h
** \author Steve Sloan <steve@finagle.org>
** \date Wed Nov 28 2007
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

#ifndef FINAGLE_XML_CONFIGURABLE_H
#define FINAGLE_XML_CONFIGURABLE_H

#include <Finagle/XML/Element.h>

namespace Finagle {  namespace XML {

/*!
** \class Finagle::Configurable
** \brief Abstract base class XML-configurable classes
*/

class Configurable : virtual public ReferenceCount {
public:
  typedef ObjectRef<Configurable> Ref;

public:
  Configurable( void );
  virtual ~Configurable( void );

  virtual bool configure( XML::Element const &config ) = 0;
  virtual XML::Element::Ref configuration( void ) const = 0;
};

// INLINE IMPLEMENTATION ******************************************************

inline Configurable::Configurable( void )
{}

inline Configurable::~Configurable( void )
{}

} }

#endif
