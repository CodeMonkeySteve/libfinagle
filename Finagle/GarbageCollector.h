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

#include <boost/bind.hpp>
#include <boost/signals.hpp>
#include <Finagle/AppLoop.h>
#include <Finagle/List.h>
#include <Finagle/ObjectPtr.h>

namespace Finagle {

//!\brief Provides garbage collection for ObjectPtrs.
template <typename Class>
class GarbageCollector : public boost::signals::trackable {
public:
  GarbageCollector( void );
 ~GarbageCollector( void );
  GarbageCollector &operator +=( ObjectPtr<Class> obj );

  void collect( void );

public:
  boost::signal< void( ObjectPtr<Class> ) > onCollect;

protected:
  List<ObjectPtr<Class> > _trash;
};

// TEMPLATE IMPLEMENTATION ****************************************************

template <typename Class>
GarbageCollector<Class>::GarbageCollector( void )
{}

template <typename Class>
GarbageCollector<Class>::~GarbageCollector( void )
{
  collect();
}

template <typename Class>
GarbageCollector<Class> &GarbageCollector<Class>::operator +=( ObjectPtr<Class> obj )
{
  _trash.push_back( obj );

  if ( _trash.size() == 1 )
    AppLoop::idle.connect( boost::bind( &GarbageCollector<Class>::collect, this ) );

  return *this;
}

template <typename Class>
void GarbageCollector<Class>::collect( void )
{
  for ( typename List<ObjectPtr<Class> >::Iterator obj = _trash.begin(); obj != _trash.end(); ) {
    if ( (*obj)->refs() > 1 ) {  ++obj;  continue;  }
    *obj = 0;
    _trash.erase( obj++ );
  }

  if ( _trash.empty() )
    AppLoop::idle.disconnect( this );
}

}

#endif
