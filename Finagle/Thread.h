/*!
** \file Thread.h
** \author Steve Sloan <steve@finagle.org>
** \date Fri Oct 8 2004
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

#ifndef FINAGLE_THREAD_H
#define FINAGLE_THREAD_H

#include <Finagle/PThreadEx.h>

namespace Finagle {

class Thread {
public:
  typedef pthread_t ID;

public:
  Thread( void );
  virtual ~Thread( void );

  bool running( void ) const;
  Thread::ID const &id( void ) const;
  int  exitVal( void ) const;

  void start( void );
  int  join( void );
  void stop( void );

public:
  static Thread *self( void );
  static Thread::ID self_id( void );
  static void exit( int res );

protected:
  virtual int exec( void ) = 0;

protected:
  ID _id;
  bool _running;
  int _exitVal;

private:
  static pthread_key_t _thread;
  static void *run( void *This );
};

// INLINE IMPLEMENTATION ******************************************************

//! Creates a new (stopped) thread
inline Thread::Thread( void )
: _id(0), _running(false), _exitVal(0)
{}

//! Stops the thread, if running
inline Thread::~Thread( void )
{
  stop();
}

//! Returns this thread's \c pthreads id
inline Thread::ID const &Thread::id( void ) const
{
  return _id;
}

//! Returns \c true if the thread is running (or should be running)
inline bool Thread::running( void ) const
{
  return _running;
}

//! Returns the final exit value (or \c 0 if the thread is still running)
inline int Thread::exitVal( void ) const
{
  return running() ? 0 : _exitVal;
}

//! Causes the currently running thread to exit, returning \a res.
inline void Thread::exit( int res )
{
  pthread_exit( (void *) res );
}

//! Returns the \c pthreads id for the currently-running thread.
inline Thread::ID Thread::self_id( void )
{
  return pthread_self();
}

}

#endif
