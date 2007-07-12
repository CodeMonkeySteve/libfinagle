/*!
** \file Schedule.h
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

#ifndef FINAGLE_SCHEDULE_H
#define FINAGLE_SCHEDULE_H

#include <Finagle/Configure.h>
#include <DateTimeMask.h>

namespace Finagle {

class Schedule : public XML::Configurable {
public:
  static const char * const Tag;
  typedef ObjectRef<Schedule> Ref;
  typedef ObjectRef<Schedule const> ConstRef;

public:
  Schedule( void );

  bool operator ()( DateTime const &When ) const;
  bool apply( DateTime const &When, Configurable::Ref Target ) const;

  void configure( XML::Element const &El );
  XML::Element::Ref configuration( void ) const;

  bool operator <( Schedule const &That ) const {  return( this < &That );  }

protected:
  DateTimeMask Begin, End;
  List<Schedule::Ref> SubSched;
  XML::Element::Ref Config;

  friend class Schedulable;
};


class Schedulable : public XML::Configurable {
public:
  typedef ObjectRef<Schedule> Ref;
  typedef ObjectRef<Schedule const> ConstRef;

public:
  Schedulable( const char *Tag = 0 );
 ~Schedulable( void );

  void configure( XML::Element const &El );
  XML::Element::Ref configuration( void ) const;

protected:
  typedef Map<Schedulable::Ref, List<Schedule::ConstRef> > Map;
  static Map Master;

  static void init( void );
};

}

#endif
