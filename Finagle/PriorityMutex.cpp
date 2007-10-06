/*!
** \file PriorityMutex.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Tue Sep 21 2004
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

#include <algorithm>

#include "PriorityMutex.h"

using namespace Finagle;

/*! \class Finagle::PriorityMutex
** \brief Provides a mutually-exclusive synchronization object with prioritized locking.
*/

bool PriorityMutex::lock( PriorityLock &Lock )
{
  PriorityLock *oldOwner = owner();

  if ( find( _locks.begin(), _locks.end(), &Lock ) == _locks.end() ) {
    // Insertion sort
    List<PriorityLock *>::Iterator i;
    for ( i = _locks.begin(); i != _locks.end(); ++i ) {
      if ( **i < Lock ) {
        _locks.insert( i, &Lock );
        break;
      }
    }
    if ( i == _locks.end() )
      _locks.push_back( &Lock );
  }

  PriorityLock *Owner = owner();
  if ( oldOwner && (Owner != oldOwner) )
    oldOwner->LoseLock();

  return Lock.locked();
}


void PriorityMutex::unlock( PriorityLock &Lock )
{
  PriorityLock *oldOwner = owner();
  if ( oldOwner == &Lock )
    oldOwner->LoseLock();

  _locks.remove( &Lock );

  PriorityLock *Owner = owner();
  if ( Owner && (Owner != oldOwner) )
    Owner->GainLock();
}

/*! \class Finagle::PriorityLock
** \brief Provides a lock on a PriorityMutex.
*/


PriorityLock::~PriorityLock( void )
{
  if ( _mutex )
    _mutex->unlock( *this );
}

void PriorityLock::mutex( PriorityMutex *mutex )
{
  if ( _mutex )  _mutex->unlock( *this );
  _mutex = mutex;
}

void PriorityLock::priority( unsigned priority )
{
  if ( _mutex )  _mutex->unlock( *this );
  _priority = priority;
}
