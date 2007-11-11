/*!
** \file MutexTest.cpp
** \date Thu Apr 5 2007
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

#include <cppunit/extensions/HelperMacros.h>
#include <Finagle/Mutex.h>
#include <Finagle/ThreadFunc.h>
#include <Finagle/Util.h>

using namespace std;
using namespace Finagle;

class MutexTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( MutexTest );
  CPPUNIT_TEST( testCreateDestroy );
  CPPUNIT_TEST( testLockUnlock );
  CPPUNIT_TEST( testLockErr );
  CPPUNIT_TEST( testUnlockErr );
  CPPUNIT_TEST( testThreadLock );
  CPPUNIT_TEST( testThreadLock2 );
  CPPUNIT_TEST( testRecursiveLock );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );
  void tearDown( void );

  void testCreateDestroy( void );
  void testLockUnlock( void );
  void testLockErr( void );
  void testUnlockErr( void );
  void testThreadLock( void );
  void testThreadLock2( void );
  void testRecursiveLock( void );

protected:
  void countUp( void );

protected:
  Mutex *_guard;
  int _count;
};

CPPUNIT_TEST_SUITE_REGISTRATION( MutexTest );

void MutexTest::setUp( void )
{
  CPPUNIT_ASSERT_NO_THROW( _guard = new Mutex );
  _count = 0;
}


void MutexTest::tearDown( void )
{
  CPPUNIT_ASSERT_NO_THROW( delete _guard );
  _guard = 0;
}


void MutexTest::testCreateDestroy( void )
{
  CPPUNIT_ASSERT( _guard != 0 );
}


void MutexTest::testLockUnlock( void )
{
  CPPUNIT_ASSERT_NO_THROW( _guard->lock() );
  CPPUNIT_ASSERT_NO_THROW( _guard->unlock() );
}


void MutexTest::testLockErr( void )
{
  Mutex *m = 0;
  CPPUNIT_ASSERT_NO_THROW( m = new Mutex );
  CPPUNIT_ASSERT_NO_THROW( m->lock() );
  CPPUNIT_ASSERT_THROW( delete m, PThreadEx );
}


void MutexTest::testUnlockErr( void )
{
  CPPUNIT_ASSERT_NO_THROW( _guard->lock() );
  CPPUNIT_ASSERT_NO_THROW( _guard->unlock() );
  CPPUNIT_ASSERT_THROW( _guard->unlock(), PThreadEx );
}


void MutexTest::testThreadLock( void )
{
  ClassFuncThread<MutexTest> t( this, &MutexTest::countUp );

  CPPUNIT_ASSERT_NO_THROW( _guard->lock() );
  CPPUNIT_ASSERT_NO_THROW( t.start() );
  sleep( 0.01 );
  CPPUNIT_ASSERT_EQUAL( 0, _count );

  CPPUNIT_ASSERT_NO_THROW( _guard->unlock() );
  CPPUNIT_ASSERT_NO_THROW( t.join() );
  CPPUNIT_ASSERT_EQUAL( 100, _count );
}


void MutexTest::testThreadLock2( void )
{
  ClassFuncThread<MutexTest> t( this, &MutexTest::countUp );
  CPPUNIT_ASSERT_NO_THROW( t.start() );

  for ( unsigned i = 0; i < 100; ++i ) {
    Lock _( *_guard );
    int c = _count;

    // Give thread a chance to change the count (if the mutex fails, that is)
    sleep( 0.01 );
    --_count;
    sleep( 0.01 );

    // Thread shouldn't have touched the count, since we're locked.
    CPPUNIT_ASSERT_EQUAL( c - 1, _count );
  }

  CPPUNIT_ASSERT_NO_THROW( t.join() );
  CPPUNIT_ASSERT_EQUAL( 0, _count );
}


void MutexTest::testRecursiveLock( void )
{
  ClassFuncThread<MutexTest> t( this, &MutexTest::countUp );

  _guard->lock();
  _count = 43;
  CPPUNIT_ASSERT_NO_THROW( t.start() );
  {
    Lock _( *_guard );

    // Give thread a chance to change the count (if the mutex fails, that is)
    sleep( 0.01 );
    --_count;
    sleep( 0.01 );

    // Thread shouldn't have touched the count, since we're locked.
    CPPUNIT_ASSERT_EQUAL( 42, _count );
  }
  _guard->unlock();

  CPPUNIT_ASSERT_NO_THROW( t.join() );
  CPPUNIT_ASSERT_EQUAL( 142, _count );
}


void MutexTest::countUp( void )
{
  for ( unsigned i = 0; i < 100; ++i ) {
    Lock _( *_guard );
    _count++;
  }
}


