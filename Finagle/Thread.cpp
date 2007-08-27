/*!
** \file Thread.cpp
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

#include "Thread.h"
#include "AppLoop.h"
#include "MemTrace.h"

using namespace Finagle;

static class ThreadData {
public:
  ThreadData( void ) {
    PTHREAD_ASSERT( pthread_key_create( &threadClass, 0 ) );
  }

 ~ThreadData( void ) {
    PTHREAD_ASSERT( pthread_key_delete( threadClass ) );
  }

  void setSelf( Thread *cur ) {
    PTHREAD_ASSERT( pthread_setspecific( threadClass, cur ) );
  }

  Thread *getSelf( void ) {
    return (Thread *) pthread_getspecific( threadClass );
  }

public:
  static pthread_key_t threadClass;
} __threadData;

pthread_key_t ThreadData::threadClass;


/*! \class Thread
**
*/

void *Thread::run( void *This )
{
  Thread *t = (Thread *) This;

  __threadData.setSelf( t );
  t->_exitVal = t->exec();
  __threadData.setSelf( 0 );
  t->_running = false;

  return (void *) t->_exitVal;
}


void Thread::start( void )
{
  if ( running() )  return;

  pthread_attr_t attr;
  PTHREAD_ASSERT( pthread_attr_init( &attr ) );
  PTHREAD_ASSERT( pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE ) );
  _running = true;
  PTHREAD_ASSERT( pthread_create( &_id, &attr, Thread::run, this ) );
  PTHREAD_ASSERT( pthread_attr_destroy( &attr ) );
}


int Thread::join( void )
{
  if ( !_id )
    return -1;

  void *res = 0;
  ID id( _id );

  int join_res = pthread_join( id, &res );
  _id = 0;
  if ( ((join_res == ESRCH) && !running()) ||                      // Thread not running
       ((join_res == EDEADLK) && (id == Thread::self_id())) )      // Thread is current
    return _exitVal;

  PTHREAD_ASSERT( join_res );
  return (int) res;
}


void Thread::stop( void )
{
  if ( !running() )
    return;

  // Can't use pthread_cancel, as Mutex needs pthread_cleanup_push/_pop() but, being do/while macros, they are
  // inherently un-Object Orientable.  So, next best thing, set a flag and wait for exec() to check it and quit.
  _running = false;
  join();
}

//! Returns the Thread class for the currently-running thread.
Thread *Thread::self( void )
{
  return __threadData.getSelf();
}
