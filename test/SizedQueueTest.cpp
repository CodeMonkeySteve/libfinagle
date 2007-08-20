/*!
** \file SizedSizedQueueTest.cpp
** \date Mon Aug 20 2007
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
#include <Finagle/SizedQueue.h>
#include <Finagle/ThreadFunc.h>
#include <Finagle/Util.h>

using namespace std;
using namespace Finagle;

class SizedQueueTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( SizedQueueTest );
  CPPUNIT_TEST( testCreateDestroy );
  CPPUNIT_TEST( testPush );
  CPPUNIT_TEST( testPushPop );
  CPPUNIT_TEST( testSynchronize );
  CPPUNIT_TEST( testThreadFill );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );
  void tearDown( void );

  void testCreateDestroy( void );
  void testPush( void );
  void testPushPop( void );
  void testSynchronize( void );
  void testThreadFill( void );

protected:
  void enqueue( void );
  void fillQueue( void );
  void squareQueue( void );

protected:
  static const unsigned QueueSize;
  static const unsigned FillSize;
  SizedQueue<unsigned> *_queue;
};

CPPUNIT_TEST_SUITE_REGISTRATION( SizedQueueTest );

const unsigned SizedQueueTest::QueueSize = 100;
const unsigned SizedQueueTest::FillSize = QueueSize * 100;

void SizedQueueTest::setUp( void )
{
  CPPUNIT_ASSERT_NO_THROW( _queue = new SizedQueue<unsigned>( QueueSize ) );
}


void SizedQueueTest::tearDown( void )
{
  CPPUNIT_ASSERT_NO_THROW( delete _queue );
  _queue = 0;
}


void SizedQueueTest::enqueue( void )
{
  sleep( 0.1 ); // Give parent thread a chance to block on the queue before we push to it.
  CPPUNIT_ASSERT_NO_THROW( _queue->push( 42 ) );
}

void SizedQueueTest::fillQueue( void )
{
  for ( unsigned i = 0; i < FillSize; ++i )
    CPPUNIT_ASSERT_NO_THROW( _queue->push( i ) );
}


void SizedQueueTest::testCreateDestroy( void )
{
  CPPUNIT_ASSERT( _queue != 0 );
}

void SizedQueueTest::testPush( void )
{
  CPPUNIT_ASSERT( _queue->empty() );
  CPPUNIT_ASSERT_NO_THROW( _queue->push( 42 ) );
  CPPUNIT_ASSERT_EQUAL( 1U, _queue->size() );
}

void SizedQueueTest::testPushPop( void )
{
  CPPUNIT_ASSERT( _queue->empty() );

  for ( unsigned i = 0; i < QueueSize; ++i ) {
    CPPUNIT_ASSERT_EQUAL( i, _queue->size() );
    CPPUNIT_ASSERT_NO_THROW( _queue->push( i ) );
    CPPUNIT_ASSERT_EQUAL( i + 1, _queue->size() );
  }

  for ( unsigned i = QueueSize; i > 0; --i ) {
    CPPUNIT_ASSERT_EQUAL( i, _queue->size() );
    CPPUNIT_ASSERT_EQUAL( QueueSize - i, _queue->pop() );
    CPPUNIT_ASSERT_EQUAL( i - 1, _queue->size() );
  }

  CPPUNIT_ASSERT( _queue->empty() );
}

void SizedQueueTest::testSynchronize( void )
{
  ClassFuncThread<SizedQueueTest> enqueueThread( this, &SizedQueueTest::enqueue );
  CPPUNIT_ASSERT_NO_THROW( enqueueThread.start() );

  unsigned v;
  CPPUNIT_ASSERT_NO_THROW( v = _queue->pop() );
  CPPUNIT_ASSERT_EQUAL( 42U, v );

  CPPUNIT_ASSERT_NO_THROW( enqueueThread.join() );
  CPPUNIT_ASSERT( _queue->empty() );
}


void SizedQueueTest::testThreadFill( void )
{
  CPPUNIT_ASSERT( _queue->empty() );

  ClassFuncThread<SizedQueueTest> fillQueueThread( this, &SizedQueueTest::fillQueue );
  CPPUNIT_ASSERT_NO_THROW( fillQueueThread.start() );
  sleep( 0.1 );

  for ( unsigned i = FillSize; i > QueueSize; --i ) {
    CPPUNIT_ASSERT( _queue->size() <= QueueSize );
    CPPUNIT_ASSERT_EQUAL( FillSize - i, _queue->pop() );
  }

  for ( unsigned i = QueueSize; i > 0; --i ) {
    CPPUNIT_ASSERT( _queue->size() <= i );
    CPPUNIT_ASSERT_EQUAL( FillSize - i, _queue->pop() );
  }

  CPPUNIT_ASSERT_NO_THROW( fillQueueThread.join() );
  CPPUNIT_ASSERT( _queue->empty() );
}

