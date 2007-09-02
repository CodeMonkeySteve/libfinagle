/*!
** \file WaitCondition.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Sat Nov 1 2003
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

#include "WaitCondition.h"

using namespace Finagle;

/*! \class Finagle::WaitCondition
** \brief Provides a semaphore-style mutex.
** This object can be used to have threads wait for an event (signalled from
** another thread).
*/

//! Waits for another thread to #signal this WaitCondition, and returns \c true.  If the thread has not been signaled after
//! \a timeout seconds, returns \c false.
bool WaitCondition::wait( Time timeout )
{
  Time t( Time::now() + timeout );
  timespec end;
  end.tv_sec = time_t( trunc(t) );
  end.tv_nsec = long( (t - end.tv_sec) * 1000000000 );

  int res = 0;
  {
    Lock _( *this );
    res = pthread_cond_timedwait( &_cond, &_mutex, &end );
  }
  if ( res == 0 )
    return true;

  if ( res != ETIMEDOUT )
    throw PThreadEx( "pthread_cond_timedwait( &_cond, &_mutex, &end )", res );

  return false;
}
