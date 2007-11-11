/*!
** \file ThreadTest.cpp
** \date Mon Apr 2 2007
** \author Steve Sloan <steve@finagle.org>
** Copyright (C) 2007 by Steve Sloan
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

#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
#include <Finagle/Queue.h>
#include <Finagle/Thread.h>
#include <Finagle/ThreadFunc.h>
#include <Finagle/Util.h>

using namespace std;
using namespace Finagle;

class ThreadTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ThreadTest );
  CPPUNIT_TEST( testCreateDestroy );
  CPPUNIT_TEST( testClassThreadFunc );
  CPPUNIT_TEST( testStop );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );
  void tearDown( void );

  void testCreateDestroy( void );
  void testClassThreadFunc( void );
  void testStop( void );

protected:
  int _count;
  void countUp( void );
  void countDown( void );

  Mutex *_wait;
  void eternalWait( void );
};


CPPUNIT_TEST_SUITE_REGISTRATION( ThreadTest );

void ThreadTest::setUp( void )
{
  CPPUNIT_ASSERT_NO_THROW( _wait = new Mutex );
  _count = 0;
}


void ThreadTest::tearDown( void )
{
  CPPUNIT_ASSERT_NO_THROW( delete _wait );
  _wait = 0;
}


void ThreadTest::countUp( void )
{
  for ( unsigned i = 1; i <= 1000; ++i ) {
    _count += i;
  }
}

void ThreadTest::countDown( void )
{
  for ( unsigned i = 1; i <= 1000; ++i ) {
    _count -= i;
  }
}

void ThreadTest::eternalWait( void )
{
  _count++;

  while ( Thread::self()->running() )
    sleep( 0.01 );
  return;

  // Should never get here!
  _count++;
}


void ThreadTest::testCreateDestroy( void )
{
  for ( unsigned i = 0; i < 100; ++i ) {
    _count = 0;
    CPPUNIT_ASSERT_EQUAL( 0, _count );
    Thread *t = new ClassFuncThread<ThreadTest>( this, &ThreadTest::eternalWait );;
    t->start();
    delete t;
    CPPUNIT_ASSERT_EQUAL( 1, _count );
  }
}


void ThreadTest::testClassThreadFunc( void )
{
  int start = (rand() % 1000) + 100;
  _count = start;

  ClassFuncThread<ThreadTest> t1( this, &ThreadTest::countUp ), t2( this, &ThreadTest::countDown );
  CPPUNIT_ASSERT_NO_THROW( t1.start() );
  CPPUNIT_ASSERT_NO_THROW( t2.start() );
  CPPUNIT_ASSERT_NO_THROW( t1.join() );
  CPPUNIT_ASSERT_NO_THROW( t2.join() );

  CPPUNIT_ASSERT_EQUAL( start, _count );
}


void ThreadTest::testStop( void )
{
  _count = 0;
  ClassFuncThread<ThreadTest> t( this, &ThreadTest::eternalWait );
  CPPUNIT_ASSERT_NO_THROW( t.start() );
  sleep( 0.01 );

  CPPUNIT_ASSERT_NO_THROW( t.stop() );

  CPPUNIT_ASSERT_EQUAL( 1, _count );
  CPPUNIT_ASSERT_NO_THROW( delete _wait );
  _wait = 0;
}
