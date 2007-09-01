/*!
** \file SpoolTest.cpp
** \date Tue Aug 21 2007
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
#include <Finagle/Spool.h>
#include <Finagle/ThreadFunc.h>
#include <Finagle/Util.h>

using namespace std;
using namespace Finagle;

class SpoolTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( SpoolTest );
  CPPUNIT_TEST( testCreateDestroy );
//  CPPUNIT_TEST( testPush );
//  CPPUNIT_TEST( testPushPop );
//  CPPUNIT_TEST( testSynchronize );
//  CPPUNIT_TEST( testThreadFill );
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
  void fillSpool( void );
  void squareSpool( void );

protected:
  static const Dir SpoolDir;
  static const unsigned SpoolSize, FillSize;
  Spool<unsigned, unsigned *> *_spool;
};

const Dir SpoolTest::SpoolDir( "./spool" );
const unsigned SpoolTest::SpoolSize = 100;
const unsigned SpoolTest::FillSize = SpoolSize; // * 100;


CPPUNIT_TEST_SUITE_REGISTRATION( SpoolTest );


void SpoolTest::setUp( void )
{
  if ( !SpoolDir.exists() )
    SpoolDir.create();

  CPPUNIT_ASSERT_NO_THROW( (_spool = new Spool<unsigned, unsigned *>(SpoolDir, SpoolSize)) );
}


void SpoolTest::tearDown( void )
{
  CPPUNIT_ASSERT_NO_THROW( delete _spool );
  _spool = 0;

  if ( SpoolDir.exists() )
    SpoolDir.erase( true );
}


void SpoolTest::enqueue( void )
{
  sleep( 0.1 ); // Give parent thread a chance to block on the queue before we push to it.
  CPPUNIT_ASSERT_NO_THROW( _spool->push_back( new unsigned(42) ) );
}

void SpoolTest::fillSpool( void )
{
  sleep( 0.1 ); // Give parent thread a chance to block on the queue before we push to it.
  for ( unsigned i = 0; i < SpoolSize; ++i )
    CPPUNIT_ASSERT_NO_THROW( _spool->push_back( new unsigned(i) ) );
}


void SpoolTest::testCreateDestroy( void )
{
  for ( unsigned i = 0; i < 1000; ++i ) {
    CPPUNIT_ASSERT( _spool != 0 );
    CPPUNIT_ASSERT_NO_THROW( delete _spool );
    _spool = 0;
    CPPUNIT_ASSERT_NO_THROW( (_spool = new Spool<unsigned, unsigned *>(SpoolDir, SpoolSize)) );
  }
}

void SpoolTest::testPush( void )
{
  for ( unsigned i = 0; i < 1000; ++i ) {
    CPPUNIT_ASSERT( _spool->empty() );
    unsigned *v = new unsigned(42);
    CPPUNIT_ASSERT_NO_THROW( _spool->push_back( v ) );
    CPPUNIT_ASSERT_EQUAL( 1U, _spool->size() );
    CPPUNIT_ASSERT_EQUAL( v, _spool->pop_front() );
    delete v;
    CPPUNIT_ASSERT( _spool->empty() );
  }
}

void SpoolTest::testPushPop( void )
{
  CPPUNIT_ASSERT( _spool->empty() );

  for ( unsigned i = 0; i < FillSize; ++i ) {
    CPPUNIT_ASSERT_EQUAL( i, _spool->size() );
    CPPUNIT_ASSERT_NO_THROW( _spool->push_back( new unsigned(i) ) );
    CPPUNIT_ASSERT_EQUAL( i + 1, _spool->size() );
  }

  for ( unsigned i = FillSize; i > 0; --i ) {
    unsigned *v;
    CPPUNIT_ASSERT_EQUAL( i, _spool->size() );
    CPPUNIT_ASSERT_NO_THROW( v = _spool->pop_front() );
    CPPUNIT_ASSERT_EQUAL( SpoolSize - i, *v );
    CPPUNIT_ASSERT_EQUAL( i - 1, _spool->size() );
    delete v;
  }

  CPPUNIT_ASSERT( _spool->empty() );
}

void SpoolTest::testSynchronize( void )
{
  ClassFuncThread<SpoolTest> enqueueThread( this, &SpoolTest::enqueue );
  CPPUNIT_ASSERT_NO_THROW( enqueueThread.start() );

  unsigned *v;
  CPPUNIT_ASSERT_NO_THROW( v = _spool->pop_front() );
  CPPUNIT_ASSERT_EQUAL( 42U, *v );
  delete v;

  CPPUNIT_ASSERT_NO_THROW( enqueueThread.join() );
  CPPUNIT_ASSERT( _spool->empty() );
}


void SpoolTest::testThreadFill( void )
{
  CPPUNIT_ASSERT( _spool->empty() );

  ClassFuncThread<SpoolTest> fillSpoolThread( this, &SpoolTest::fillSpool );
  CPPUNIT_ASSERT_NO_THROW( fillSpoolThread.start() );

  for ( unsigned i = 0; i < SpoolSize; ++i ) {
    unsigned *v;
    CPPUNIT_ASSERT_NO_THROW( v = _spool->pop_front() );
    CPPUNIT_ASSERT_EQUAL( i, *v );
    delete v;
  }

  CPPUNIT_ASSERT_NO_THROW( fillSpoolThread.join() );
  CPPUNIT_ASSERT( _spool->empty() );
}
