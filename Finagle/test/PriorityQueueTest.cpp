/*!
** \file PriorityQueueTest.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Tue Sep 4 2007
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
#include <Finagle/PriorityQueue.h>
#include <Finagle/ThreadFunc.h>
#include <Finagle/Util.h>

using namespace std;
using namespace Finagle;

class PriorityQueueTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( PriorityQueueTest );
/*  CPPUNIT_TEST( testCreateDestroy );
  CPPUNIT_TEST( testOrder );
  CPPUNIT_TEST( testPush );
  CPPUNIT_TEST( testPushPop );
  CPPUNIT_TEST( testSynchronize );*/
  CPPUNIT_TEST( testThreadFill );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );
  void tearDown( void );

  void testCreateDestroy( void );
  void testOrder( void );
  void testPush( void );
  void testPushPop( void );
  void testSynchronize( void );
  void testThreadFill( void );

protected:
  void enqueue( void );
  void fillQueue( void );
  void squareQueue( void );

protected:
  class Dummy : public ReferenceCount {
  public:
    typedef ObjectRef<Dummy> Ref;

    Dummy( unsigned val ) : _valid(0xdeadbeef), _val(val) {}
    virtual ~Dummy( void ) { _valid = 0xbdbdbdbd;  }

    bool isValid( void ) const {  return _valid == 0xdeadbeef;  }
    unsigned val( void ) const {  return _val;  }

  protected:
    long _valid;
    unsigned _val;
  };

  class Dummyer : public Dummy {
  public:
    typedef ObjectRef<Dummyer> Ref;
    Dummyer( unsigned val ) : Dummy(val) {}
  };

protected:
  static const unsigned FillSize = 1000000;
  PriorityQueue<unsigned> *_queue;
  PriorityQueue<unsigned> *_squared;
  PriorityQueue<Dummy::Ref> *_dummies;
};

CPPUNIT_TEST_SUITE_REGISTRATION( PriorityQueueTest );


void PriorityQueueTest::setUp( void )
{
  CPPUNIT_ASSERT_NO_THROW( _queue = new PriorityQueue<unsigned> );
  CPPUNIT_ASSERT_NO_THROW( _dummies = new PriorityQueue<Dummy::Ref> );
  _squared = 0;
}

void PriorityQueueTest::tearDown( void )
{
  CPPUNIT_ASSERT_NO_THROW( delete _queue );
  _queue = 0;
  CPPUNIT_ASSERT_NO_THROW( delete _squared );
  _squared = 0;
  CPPUNIT_ASSERT_NO_THROW( delete _dummies );
  _dummies = 0;
}


void PriorityQueueTest::enqueue( void )
{
  sleep( 0.1 ); // Give parent thread a chance to block on the queue before we push to it.
  CPPUNIT_ASSERT_NO_THROW( _queue->push( 42 ) );
}

void PriorityQueueTest::squareQueue( void )
{
  for ( unsigned i = 0; i < FillSize; ++i ) {
    unsigned v;
    CPPUNIT_ASSERT_NO_THROW( v = _queue->pop() );
    CPPUNIT_ASSERT_NO_THROW( _squared->push( sqr(v) ) );
  }
}


void PriorityQueueTest::testCreateDestroy( void )
{
  CPPUNIT_ASSERT( _queue != 0 );
}

void PriorityQueueTest::testOrder( void )
{
  _queue->push( 5, 0.0 );
  _queue->push( 6, 0.0 );
  _queue->push( 3, 1.0 );
  _queue->push( 4, 1.0 );
  _queue->push( 1, 2.0 );
  _queue->push( 2, 2.0 );

  CPPUNIT_ASSERT_EQUAL( 1U, _queue->pop() );
  CPPUNIT_ASSERT_EQUAL( 2U, _queue->pop() );
  CPPUNIT_ASSERT_EQUAL( 3U, _queue->pop() );
  CPPUNIT_ASSERT_EQUAL( 4U, _queue->pop() );
  CPPUNIT_ASSERT_EQUAL( 5U, _queue->pop() );
  CPPUNIT_ASSERT_EQUAL( 6U, _queue->pop() );
}

void PriorityQueueTest::testPush( void )
{
  CPPUNIT_ASSERT( _queue->empty() );
  CPPUNIT_ASSERT_NO_THROW( _queue->push( 42 ) );
  CPPUNIT_ASSERT_EQUAL( 1U, _queue->size() );
  CPPUNIT_ASSERT_EQUAL( 42U, _queue->pop() );
  CPPUNIT_ASSERT( _queue->empty() );
}

void PriorityQueueTest::testPushPop( void )
{
  CPPUNIT_ASSERT( _queue->empty() );

  for ( unsigned i = 0; i < FillSize; ++i ) {
    CPPUNIT_ASSERT_EQUAL( i, _queue->size() );
    CPPUNIT_ASSERT_NO_THROW( _queue->push( i ) );
    CPPUNIT_ASSERT_EQUAL( i + 1, _queue->size() );
  }

  for ( unsigned i = FillSize; i > 0; --i ) {
    CPPUNIT_ASSERT_EQUAL( i, _queue->size() );
    CPPUNIT_ASSERT_EQUAL( FillSize - i, _queue->pop() );
    CPPUNIT_ASSERT_EQUAL( i - 1, _queue->size() );
  }

  CPPUNIT_ASSERT( _queue->empty() );
}


void PriorityQueueTest::testSynchronize( void )
{
  ClassFuncThread<PriorityQueueTest> enqueueThread( this, &PriorityQueueTest::enqueue );
  CPPUNIT_ASSERT_NO_THROW( enqueueThread.start() );

  unsigned v;
  CPPUNIT_ASSERT_NO_THROW( v = _queue->pop() );
  CPPUNIT_ASSERT_EQUAL( 42U, v );

  CPPUNIT_ASSERT_NO_THROW( enqueueThread.join() );
  CPPUNIT_ASSERT( _queue->empty() );
}


void PriorityQueueTest::testThreadFill( void )
{
  CPPUNIT_ASSERT( _queue->empty() );

  ClassFuncThread<PriorityQueueTest> fillQueueThread( this, &PriorityQueueTest::fillQueue );
  CPPUNIT_ASSERT_NO_THROW( fillQueueThread.start() );

  for ( unsigned i = 0; i < FillSize; ++i ) {
    Dummy::Ref d( _dummies->pop() );
    CPPUNIT_ASSERT( d );
    CPPUNIT_ASSERT( d->isValid() );

    Dummyer::Ref d2( d );
    CPPUNIT_ASSERT( d2 );
    CPPUNIT_ASSERT( d2->isValid() );

    CPPUNIT_ASSERT( d->refs() );
    CPPUNIT_ASSERT_EQUAL( i, d->val() );
    d = 0;
    d2 = 0;
  }

  CPPUNIT_ASSERT_NO_THROW( fillQueueThread.join() );
  CPPUNIT_ASSERT( _queue->empty() );
}

void PriorityQueueTest::fillQueue( void )
{
  for ( unsigned i = 0; i < FillSize; ++i ) {
    Dummyer::Ref d( new Dummyer(i) );
    _dummies->push( Dummy::Ref(d) );
    d = 0;
  }
}
