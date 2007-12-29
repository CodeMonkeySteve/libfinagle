/*!
** \file Timer.h
** \author Steve Sloan <steve@finagle.org>
** \date Fri Sep 3 2004
** Copyright (C) 2004 by Steve Sloan
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

#ifndef FINAGLE_ALARM_H
#define FINAGLE_ALARM_H

#include <Finagle/AppLoop.h>
#include <Finagle/ObjectPtr.h>
#include <Finagle/List.h>
#include <Finagle/DateTime.h>
#include <Finagle/Singleton.h>

#include <iostream>

namespace Finagle {

class Timer : public ReferenceCount, public signal0<> {
public:
  typedef ObjectPtr<Timer> Ptr;

public:
  static Timer::Ptr SingleShot( Time const &delay );
  static Timer::Ptr Recurring( Time const &period );
  Timer( Time const &period, bool repeat );

  bool isRunning( void ) const;

  void start( Time _period );
  void start( void );
  void stop( void );
  void restart( void );

  Time const &nextAlarm( void ) const;
  bool operator <( Timer const &that ) const;

protected:
  void trigger( void );

protected:
  Time _nextAlarm;
  Time _period;
  bool _repeat;

protected:
  friend void AppLoop::process( Time );
};

// INLINE IMPLEMENTATION ******************************************************

inline Timer::Ptr Timer::SingleShot( Time const &delay )
{
  return new Timer( delay, false );
}

inline Timer::Ptr Timer::Recurring( Time const &period )
{
  return new Timer( period, true );
}

inline Timer::Timer( Time const &period, bool repeat )
: _nextAlarm( 0 ), _period( period ), _repeat( repeat )
{
  start();
}

inline bool Timer::isRunning( void ) const
{
  return _nextAlarm != 0;
}

inline void Timer::start( Time period )
{
  _period = period;
  start();
}

inline void Timer::restart( void )
{
  stop();
  start();
}

inline Time const &Timer::nextAlarm( void ) const
{
  return _nextAlarm;
}

inline bool Timer::operator <( Timer const &that ) const
{
  return _nextAlarm < that._nextAlarm;
}

namespace AppLoop {
  static Singleton<List<Timer::Ptr> > Timers;
}

};

#endif
