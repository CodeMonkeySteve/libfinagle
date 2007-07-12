/*!
** \file GarbageCollector.h
** \author Steve Sloan <steve@finagle.org>
** \date Wed Jan 5 2005
** Copyright (C) 2005 by Steve Sloan
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

#ifndef FINAGLE_GARBAGECOLLECTOR_H
#define FINAGLE_GARBAGECOLLECTOR_H

#include <Finagle/AppLoop.h>
#include <Finagle/ReferencedObject.h>
#include <Finagle/sigslot.h>
#include <Finagle/Array.h>

namespace Finagle {

using namespace sigslot;

//!\brief Provides garbage collection for ReferencedObject.
template <typename Class>
class GarbageCollector : public has_slots<> {
public:
  GarbageCollector( void ) {}
  GarbageCollector &operator +=( ObjectRef<Class> Obj );

protected:
  void collect( void );

protected:
  List<ObjectRef<Class> > Items;
};

// TEMPLATE IMPLEMENTATION ****************************************************

template <typename Class>
GarbageCollector<Class> &GarbageCollector<Class>::operator +=( ObjectRef<Class> Obj )
{
  Items.push_back( Obj );

  if ( Items.size() == 1 )
    AppLoop::Idle.connect( this, &GarbageCollector<Class>::collect );

  return *this;
}

template <typename Class>
void GarbageCollector<Class>::collect( void )
{
  for ( List<ObjectRef<Class> >::Iterator Obj = Items.begin(); Obj != Items.end(); ) {
    if ( (*Obj)->refCount() == 1 ) {
      *Obj = 0;
      Items.erase( Obj++ );
    } else
      ++Obj;
  }

  if ( Items.empty() )
    AppLoop::Idle.disconnect( this );
}

}

#endif
