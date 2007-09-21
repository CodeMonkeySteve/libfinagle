/*!
** \file Schedule.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Tue Jan 25 2005
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

#include "Schedule.h"
#include "Constructable.h"

using namespace Finagle;


const char * const Schedule::Tag = "Schedule";
static XML::ObjectFactory<Schedule> Factory( Schedule::Tag );

Schedule::Schedule( void )
: Configurable( Tag )
{}


bool Schedule::operator ()( DateTime const &When ) const
{
  DateTime startTime, endTime;

  startTime = Begin.prev( When );
  if ( !startTime )
    return false;

  if ( End.empty() )
    return true;

  endTime = End.next( startTime );
  return !endTime || (When < endTime);
}


bool Schedule::apply( DateTime const &When, Configurable::Ref Target ) const
{
  if ( !(*this)( When ) )
    return false;

  if ( Config )
    Target->configure( *Config );

  for ( List<Schedule::Ref>::const_reverse_iterator s = SubSched.rbegin(); s != SubSched.rend(); ++s ) {
    if ( (*s)->apply( When, Target ) )
      break;
  }

  return true;
}


void Schedule::configure( XML::Element const &El )
{
  Configurable::configure( El );

  if ( El.attribs().contains( "begin" ) )
    Begin.parse( El.attrib("begin") );

  if ( El.attribs().contains( "end" ) )
    Begin.parse( El.attrib("end") );

  for ( XML::Element::ConstIterator e = El.elements().begin(); e != El.elements().end(); ++e ) {
    if ( e->tag() != Schedule::Tag ) {
      if ( !Config )
        Config = new XML::Element;

      *Config += e;
      continue;
    }

    Schedule::Ref s = Schedule::Ref( deserialize( *e ) );
    if ( s )
      SubSched.push_front( s );
  }
}


XML::Element::Ref Schedule::configuration( void ) const
{
  XML::Element::Ref El = Configurable::configuration();

  if ( !Begin.empty() )
    El->attrib("begin") = Begin;

  if ( !End.empty() )
    El->attrib("end") = End;

  if ( Config )
    *El += Config;

  for ( List<Schedule::Ref>::const_reverse_iterator s = SubSched.rbegin(); s != SubSched.rend(); ++s )
    *El += (*s)->configuration();

  return El;
}


Map<Schedulable::Ref, List<Schedule::ConstRef> > Schedulable::Master;

Schedulable::Schedulable( const char *Tag )
: Configurable( Tag )
{
}


Schedulable::~Schedulable( void )
{
  Master.erase( Ref( this ) );
}


void Schedulable::configure( XML::Element const &El )
{
  Configurable::configure( El );

  for ( XML::Element::ConstIterator e = El.elements().begin(); e != El.elements().end(); ++e ) {
    if ( e->tag() != Schedule::Tag )
      continue;

    Schedule::Ref s( deserialize( *e ) );
    if ( !s )
      continue;

    if ( s->Config )
      s->Config->tag( El.tag() );

    Master[Ref( this )].push_front( Schedule::ConstRef( s ) );
  }
}


XML::Element::Ref Schedulable::configuration( void ) const
{
  XML::Element::Ref El = Configurable::configuration();

  if ( !Master.contains( Ref( const_cast<Schedulable *>( this ) ) ) )
    return El;

  List<Schedule::ConstRef> &Sched = Master[Ref( const_cast<Schedulable *>( this ) )];
  List<Schedule::ConstRef>::const_reverse_iterator End = Sched.rend();
  for ( List<Schedule::ConstRef>::const_reverse_iterator s = Sched.rbegin(); s != End; ++s )
    *El += (*s)->configuration();

  return El;
}


void Schedulable::init( void )
{
  DateTime Now( DateTime::now() );

  for ( Map::Iterator t = Master.begin(); t != Master.end(); ++t ) {
    Configurable::Ref Target( t.key() );
    List<Schedule::ConstRef> const &Sched = t.val();

    if ( !Target )
      continue;

    for ( List<Schedule::ConstRef>::ConstIterator s = Sched.begin(); s != Sched.end(); ++s ) {
      if ( (*s)->apply( Now, Target ) )
        break;
    }
  }
}
