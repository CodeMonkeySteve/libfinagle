/*!
** \file WaitConditionTest.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Thu Apr 5 2007
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
#include <Finagle/WaitCondition.h>
#include <Finagle/ThreadFunc.h>
#include <Finagle/Util.h>

using namespace std;
using namespace Finagle;

class WaitConditionTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( WaitConditionTest );
  CPPUNIT_TEST( testCreateDestroy );
  CPPUNIT_TEST( testWaitSignal );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );
  void tearDown( void );

  void testCreateDestroy( void );
  void testSignalNoOne( void );
  void testWaitSignal( void );

protected:
  void countUp( void );

protected:
  WaitCondition *_cond, *_resp;
  unsigned _count;
};

CPPUNIT_TEST_SUITE_REGISTRATION( WaitConditionTest );


void WaitConditionTest::setUp( void )
{
  CPPUNIT_ASSERT_NO_THROW( _cond = new WaitCondition );
  _resp = 0;
  _count = 0;
}


void WaitConditionTest::tearDown( void )
{
  CPPUNIT_ASSERT_NO_THROW( delete _cond );
  _cond = 0;
  CPPUNIT_ASSERT_NO_THROW( delete _resp );
  _resp = 0;
}


void WaitConditionTest::testCreateDestroy( void )
{
  CPPUNIT_ASSERT( _cond != 0 );
}


void WaitConditionTest::testWaitSignal( void )
{
  CPPUNIT_ASSERT_NO_THROW( _resp = new WaitCondition );

  ClassFuncThread<WaitConditionTest> waitThread( this, &WaitConditionTest::countUp );
  CPPUNIT_ASSERT_NO_THROW( waitThread.start() );
  sleep( 0.01 );

  for ( unsigned i = 1; i <= 100; ++i ) {
    unsigned c = _count;
    _cond->signalOne();
    _resp->wait();
    c += i;
    CPPUNIT_ASSERT_EQUAL( c, _count );
    sleep( 0.01 );
  }

  CPPUNIT_ASSERT_NO_THROW( waitThread.join() );
}


void WaitConditionTest::countUp( void )
{
  for ( unsigned i = 1; i <= 100; ++i ) {
    _cond->wait();
    _count += i;
    sleep( 0.01 );
    _resp->signalOne();
  }
}

