/*!
** \file Mutex.cpp
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

#include "Mutex.h"
#include "MemTrace.h"

using namespace Finagle;

/*! \class Finagle::Mutex
** \brief Provides a mutually-exclusive synchronization object with recursive locking.
*/

void Mutex::init( void )
{
  pthread_mutexattr_t attr;
  PTHREAD_ASSERT( pthread_mutexattr_init( &attr ) );
  PTHREAD_ASSERT( pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE ) );
  PTHREAD_ASSERT( pthread_mutex_init( &_mutex, &attr ) );
  PTHREAD_ASSERT( pthread_mutexattr_destroy( &attr ) );
}


/*! \class Finagle::Lock
** \brief Provides a lock on a Mutex.
**
** When the Lock is instantiated, it locks the referenced mutex.  When the
** lock object falls out of scope, it unlocks the mutex.
**
** This method of mutex locking is strongly encouraged over calling
** Mutex::lock() and Mutex::unlock() as it ensures that no "dangling
** mutex locks" will occur.  This can be crucial when, for example, using
** exceptions.
**
** Example:
** \code
** Mutex Guard;
** {
**   Lock GuardLock( Guard ); // Mutex lock
**   ...                      // Critical section
** }                          // Mutex unlock
** \endcode
*/

/*! \fn Lock::Lock( Mutex &LockMutex )
** \brief Creates a lock on \a LockMutex.
*/
