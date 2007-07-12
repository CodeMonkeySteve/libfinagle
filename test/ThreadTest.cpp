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
#include <Finagle/Thread.h>
#include <Finagle/ThreadFunc.h>
#include <Finagle/Util.h>

using namespace std;
using namespace Finagle;

class ThreadTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ThreadTest );
  CPPUNIT_TEST( testClassThreadFunc );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void ) {}
  void tearDown( void ) {}

  void testClassThreadFunc( void );

protected:
  int _count;
  void countUp( void );
  void countDown( void );
};


CPPUNIT_TEST_SUITE_REGISTRATION( ThreadTest );

void ThreadTest::countUp( void )
{
  for ( unsigned i = 1; i <= 10000; ++i )
    _count += i;
}

void ThreadTest::countDown( void )
{
  for ( unsigned i = 1; i <= 10000; ++i )
    _count -= i;
}

void ThreadTest::testClassThreadFunc( void )
{
  int start = rand( Range<int>( 100, 1000 ) );
  _count = start;

  ClassFuncThread<ThreadTest> t1( this, &ThreadTest::countUp ), t2( this, &ThreadTest::countDown );
  CPPUNIT_ASSERT_NO_THROW( t1.start() );
  CPPUNIT_ASSERT_NO_THROW( t2.start() );
  CPPUNIT_ASSERT_NO_THROW( t1.join() );
  CPPUNIT_ASSERT_NO_THROW( t2.join() );

  CPPUNIT_ASSERT_EQUAL( start, _count );
}

