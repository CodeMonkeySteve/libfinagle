/*!
** \file Mutex.h
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

#ifndef FINAGLE_MUTEX_H
#define FINAGLE_MUTEX_H

#include <Finagle/PThreadEx.h>

namespace Finagle {

class Mutex {
public:
  Mutex( void );
 ~Mutex( void );

  void lock( void );
  void unlock( void );

protected:
  pthread_mutex_t _mutex;

private:
  Mutex( Mutex const & ) {}
  Mutex &operator =( Mutex const & ) {  return *this; }
};

class Lock {
public:
  Lock( Mutex &mutex );
 ~Lock( void );

protected:
  Mutex &_mutex;
};

// INLINE IMPLEMENTATION ******************************************************

//! Destroys the mutex.
inline Mutex::~Mutex( void )
{
  PTHREAD_ASSERT( pthread_mutex_destroy( &_mutex ) );
}

//! \brief Locks the mutex.
inline void Mutex::lock( void )
{
  PTHREAD_ASSERT( pthread_mutex_lock( &_mutex ) );
}

//! \brief Unlocks the mutex.
inline void Mutex::unlock( void )
{
  PTHREAD_ASSERT( pthread_mutex_unlock( &_mutex ) );
}


inline Lock::Lock( Mutex &mutex )
: _mutex( mutex )
{
  _mutex.lock();
}

inline Lock::~Lock( void )
{
  _mutex.unlock();
}


}

#endif
