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

void *Thread::run( void *arg )
{
  Thread *t = (Thread *) arg;
  t->_running = true;
  t->_exitVal = t->exec();
  t->_running = false;
  return (void *) t->_exitVal;
}


void Thread::start( void )
{
  if ( running() )  return;

  pthread_attr_t attr;
  PTHREAD_ASSERT( pthread_attr_init( &attr ) );
  PTHREAD_ASSERT( pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE ) );
  PTHREAD_ASSERT( pthread_create( &_id, &attr, Thread::run, this ) );
  PTHREAD_ASSERT( pthread_attr_destroy( &attr ) );
}

#include <iostream>
using namespace std;

int Thread::join( void )
{
  if ( !_id )
    return -1;

  void *res = 0;
  ID id( _id );

  int join_res = pthread_join( id, &res );
  _id = 0;
  if ( ((join_res == ESRCH) && !running()) ||                       // Thread not running
       ((join_res == EDEADLK) && (id == Thread::current())) )      // Thread is current
    return _exitVal;

  PTHREAD_ASSERT( join_res );
  return (int) res;
}


void Thread::kill( void )
{
  PTHREAD_ASSERT( pthread_cancel( _id ) );
}
