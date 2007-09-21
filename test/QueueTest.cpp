/*!
** \file QueueTest.cpp
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

#include <cppunit/extensions/HelperMacros.h>
#include <Finagle/Queue.h>
#include <Finagle/ThreadFunc.h>
#include <Finagle/Util.h>

using namespace std;
using namespace Finagle;

class QueueTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( QueueTest );
  CPPUNIT_TEST( testCreateDestroy );
  CPPUNIT_TEST( testPush );
  CPPUNIT_TEST( testPushPop );
  CPPUNIT_TEST( testSynchronize );
  CPPUNIT_TEST( testThreadFill );
//  CPPUNIT_TEST( testCoprocess );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );
  void tearDown( void );

  void testCreateDestroy( void );
  void testPush( void );
  void testPushPop( void );
  void testSynchronize( void );
  void testThreadFill( void );

  void testCoprocess( void );

protected:
  void enqueue( void );
  void fillQueue( void );
  void squareQueue( void );

protected:
  static const unsigned FillSize = 10000;
  Queue<unsigned> *_queue;
  Queue<unsigned> *_squared;
};

CPPUNIT_TEST_SUITE_REGISTRATION( QueueTest );


void QueueTest::setUp( void )
{
  CPPUNIT_ASSERT_NO_THROW( _queue = new Queue<unsigned> );
  _squared = 0;
}


void QueueTest::tearDown( void )
{
  CPPUNIT_ASSERT_NO_THROW( delete _queue );
  _queue = 0;
  CPPUNIT_ASSERT_NO_THROW( delete _squared );
  _squared = 0;
}


void QueueTest::enqueue( void )
{
  sleep( 0.1 ); // Give parent thread a chance to block on the queue before we push to it.
  CPPUNIT_ASSERT_NO_THROW( _queue->push_back( 42 ) );
}

void QueueTest::fillQueue( void )
{
  sleep( 0.1 ); // Give parent thread a chance to block on the queue before we push to it.
  for ( unsigned i = 0; i < FillSize; ++i )
    CPPUNIT_ASSERT_NO_THROW( _queue->push_back( i ) );
}

void QueueTest::squareQueue( void )
{
  for ( unsigned i = 0; i < FillSize; ++i ) {
    unsigned v = 0;
    CPPUNIT_ASSERT_NO_THROW( v = _queue->pop_front() );
    CPPUNIT_ASSERT_NO_THROW( _squared->push_back( sqr(v) ) );
  }
}


void QueueTest::testCreateDestroy( void )
{
  CPPUNIT_ASSERT( _queue != 0 );
}

void QueueTest::testPush( void )
{
  CPPUNIT_ASSERT( _queue->empty() );
  CPPUNIT_ASSERT_NO_THROW( _queue->push_back( 42 ) );
  CPPUNIT_ASSERT_EQUAL( 1U, _queue->size() );
}

void QueueTest::testPushPop( void )
{
  CPPUNIT_ASSERT( _queue->empty() );

  for ( unsigned i = 0; i < FillSize; ++i ) {
    CPPUNIT_ASSERT_EQUAL( i, _queue->size() );
    CPPUNIT_ASSERT_NO_THROW( _queue->push_back( i ) );
    CPPUNIT_ASSERT_EQUAL( i + 1, _queue->size() );
  }

  for ( unsigned i = FillSize; i > 0; --i ) {
    CPPUNIT_ASSERT_EQUAL( i, _queue->size() );
    CPPUNIT_ASSERT_EQUAL( FillSize - i, _queue->pop_front() );
    CPPUNIT_ASSERT_EQUAL( i - 1, _queue->size() );
  }

  CPPUNIT_ASSERT( _queue->empty() );
}

void QueueTest::testSynchronize( void )
{
  ClassFuncThread<QueueTest> enqueueThread( this, &QueueTest::enqueue );
  CPPUNIT_ASSERT_NO_THROW( enqueueThread.start() );

  unsigned v;
  CPPUNIT_ASSERT_NO_THROW( v = _queue->pop_front() );
  CPPUNIT_ASSERT_EQUAL( 42U, v );

  CPPUNIT_ASSERT_NO_THROW( enqueueThread.join() );
  CPPUNIT_ASSERT( _queue->empty() );
}


void QueueTest::testThreadFill( void )
{
  CPPUNIT_ASSERT( _queue->empty() );

  ClassFuncThread<QueueTest> fillQueueThread( this, &QueueTest::fillQueue );
  CPPUNIT_ASSERT_NO_THROW( fillQueueThread.start() );

  for ( unsigned i = 0; i < FillSize; ++i )
    CPPUNIT_ASSERT_EQUAL( i, _queue->pop_front() );

  CPPUNIT_ASSERT_NO_THROW( fillQueueThread.join() );
  CPPUNIT_ASSERT( _queue->empty() );
}


void QueueTest::testCoprocess( void )
{
  CPPUNIT_ASSERT_NO_THROW( _squared = new Queue<unsigned> );
  CPPUNIT_ASSERT( _queue->empty() );

  ClassFuncThread<QueueTest> squareQueueThread( this, &QueueTest::squareQueue );
  CPPUNIT_ASSERT_NO_THROW( squareQueueThread.start() );

  for ( unsigned i = 0; i < FillSize; ++i ) {
    CPPUNIT_ASSERT_NO_THROW( _queue->push_back( i ) );
    CPPUNIT_ASSERT_EQUAL( sqr(i), _squared->pop_front() );
  }

  CPPUNIT_ASSERT_NO_THROW( squareQueueThread.join() );
  CPPUNIT_ASSERT( _queue->empty() );
  CPPUNIT_ASSERT( _squared->empty() );
}

