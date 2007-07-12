/*!
** \file Timer.cpp
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

#include "Timer.h"

using namespace std;
using namespace Finagle;
using namespace AppLoop;

/*! \class Finagle::Timer
** Provides a time-triggered event.
**
**
*/

void Timer::start( void )
{
  if ( isRunning() )
    Timers().remove( this );

  _nextAlarm = Time::now() + _period;
  Timers().insert( this );
}


void Timer::stop( void )
{
  if ( !isRunning() )
    return;

  Timers().remove( this );
  _nextAlarm = 0;
}


void Timer::trigger( void )
{
  if ( !_repeat )
    _nextAlarm = 0;

  Timers().remove( this );

  (*this)();

  if ( _repeat ) {
    _nextAlarm += _period;
    Timers().insert( this );
  }
}
