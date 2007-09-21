/*!
** \file PriorityMutex.h
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

#ifndef FINAGLE_PRIORITYLOCK_H
#define FINAGLE_PRIORITYLOCK_H

#include <Finagle/List.h>
#include <Finagle/sigslot.h>

namespace Finagle {

using namespace sigslot;

class PriorityLock;

class PriorityMutex {
public:
  PriorityMutex( void );

  PriorityLock *owner( void ) const;

  bool lock( PriorityLock &Lock );
  void unlock( PriorityLock &Lock );

protected:
  List<PriorityLock *> Locks;

  friend class PriorityLock;
};

class PriorityLock {
public:
  PriorityLock( PriorityMutex *LockMutex = 0, unsigned Priority = 0 );
  PriorityLock( PriorityLock &that );
 ~PriorityLock( void );

  PriorityMutex *mutex( void ) const;
  void mutex( PriorityMutex *LockMutex );

  unsigned priority( void ) const;
  void priority( unsigned Priority );

  bool locked( void ) const;
  bool lock( void );
  void unlock( void );

  bool operator <( PriorityLock const &that );

public:
  signal0<> GainLock;
  signal0<> LoseLock;

protected:
  PriorityMutex *LockMutex;
  unsigned Priority;
};

// INLINE IMPLEMENTATION ******************************************************

inline PriorityLock *PriorityMutex::owner( void ) const
{
  return Locks.empty() ? 0 : Locks.front();
}


inline bool PriorityLock::operator <( PriorityLock const &that )
{
  return Priority < that.Priority;
}

inline bool PriorityLock::locked( void ) const
{
  return LockMutex && (LockMutex->owner() == this);
}

inline bool PriorityLock::lock( void )
{
  return LockMutex && LockMutex->lock( *this );
}

inline void PriorityLock::unlock( void )
{
  if ( LockMutex )
    LockMutex->unlock( *this );
}

inline PriorityMutex *PriorityLock::mutex( void ) const
{
  return LockMutex;
}

inline unsigned PriorityLock::priority( void ) const
{
  return Priority;
}

};

#endif
