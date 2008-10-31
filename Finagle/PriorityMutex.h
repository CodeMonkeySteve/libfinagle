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

#include <boost/signals.hpp>
#include <Finagle/List.h>

namespace Finagle {

class PriorityLock;

class PriorityMutex {
public:
  PriorityMutex( void );

  PriorityLock *owner( void ) const;

  bool lock( PriorityLock &lock );
  void unlock( PriorityLock &lock );

protected:
  List<PriorityLock *> _locks;

  friend class PriorityLock;
};

class PriorityLock {
public:
  PriorityLock( PriorityMutex *mutex = 0, unsigned priority = 0 );
  PriorityLock( PriorityLock &that );
 ~PriorityLock( void );

  PriorityMutex *mutex( void ) const;
  void mutex( PriorityMutex *mutex );

  unsigned priority( void ) const;
  void priority( unsigned priority );

  bool locked( void ) const;
  bool lock( void );
  void unlock( void );

  bool operator <( PriorityLock const &that );

public:
  //! emitted when this lock has (re-)acquired the mutex
  boost::signal< void() > GainLock;

  //! emitted when this lock has lost the mutex to a higher-priority lock
  boost::signal< void() > LoseLock;

protected:
  PriorityMutex *_mutex;
  unsigned _priority;
};

// INLINE IMPLEMENTATION ******************************************************

inline PriorityMutex::PriorityMutex( void )
{}

inline PriorityLock *PriorityMutex::owner( void ) const
{
  return _locks.empty() ? 0 : _locks.front();
}


inline PriorityLock::PriorityLock( PriorityMutex *mutex, unsigned priority )
: _mutex( mutex ), _priority( priority )
{}


inline PriorityLock::PriorityLock( PriorityLock &that )
: _mutex( that._mutex ), _priority( that._priority )
{}

inline bool PriorityLock::operator <( PriorityLock const &that )
{
  return _priority < that._priority;
}

inline bool PriorityLock::locked( void ) const
{
  return _mutex && (_mutex->owner() == this);
}

inline bool PriorityLock::lock( void )
{
  return _mutex && _mutex->lock( *this );
}

inline void PriorityLock::unlock( void )
{
  if ( _mutex )  _mutex->unlock( *this );
}

inline PriorityMutex *PriorityLock::mutex( void ) const
{
  return _mutex;
}

inline unsigned PriorityLock::priority( void ) const
{
  return _priority;
}

};

#endif
