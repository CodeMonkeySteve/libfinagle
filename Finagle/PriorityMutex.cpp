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

PriorityMutex::PriorityMutex( void )
{
}


bool PriorityMutex::lock( PriorityLock &Lock )
{
  PriorityLock *OldOwner = owner();

  if ( find( Locks.begin(), Locks.end(), &Lock ) == Locks.end() ) {
    // Insertion sort
    List<PriorityLock *>::Iterator i;
    for ( i = Locks.begin(); i != Locks.end(); ++i ) {
      if ( **i < Lock ) {
        Locks.insert( i, &Lock );
        break;
      }
    }
    if ( i == Locks.end() )
      Locks.push_back( &Lock );
  }

  PriorityLock *Owner = owner();
  if ( OldOwner && (Owner != OldOwner) )
    OldOwner->LoseLock();

  return Lock.locked();
}


void PriorityMutex::unlock( PriorityLock &Lock )
{
  PriorityLock *OldOwner = owner();
  if ( OldOwner == &Lock )
    OldOwner->LoseLock();

  Locks.remove( &Lock );

  PriorityLock *Owner = owner();
  if ( Owner && (Owner != OldOwner) )
    Owner->GainLock();
}


PriorityLock::PriorityLock( PriorityMutex *LockMutex, unsigned Priority )
: LockMutex( LockMutex ), Priority( Priority )
{
}


PriorityLock::PriorityLock( PriorityLock &That )
: LockMutex( That.LockMutex ), Priority( That.Priority )
{
}


PriorityLock::~PriorityLock( void )
{
  if ( LockMutex )
    LockMutex->unlock( *this );
}


void PriorityLock::mutex( PriorityMutex *LockMutex )
{
  if ( LockMutex )
    LockMutex->unlock( *this );

  PriorityLock::LockMutex = LockMutex;
}


void PriorityLock::priority( unsigned Priority )
{
  if ( LockMutex )
    LockMutex->unlock( *this );

  PriorityLock::Priority = Priority;
}
