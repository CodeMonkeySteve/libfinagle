/*!
** \file GarbageCollectorTest.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Thu Dec 20 2007
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
#include <Finagle/GarbageCollector.h>

using namespace std;
using namespace Finagle;

class Dummy : public ReferenceCount {
public:
  typedef ObjectPtr<Dummy> Ptr;
  static unsigned Instances;
  Dummy( void ) {  Instances++;  }
 ~Dummy( void ) {  Instances--;  }
};

unsigned Dummy::Instances = 0;

class GarbageCollectorTest : public CppUnit::TestFixture, public has_slots<>
{
  CPPUNIT_TEST_SUITE( GarbageCollectorTest );
  CPPUNIT_TEST( testDummy );
  CPPUNIT_TEST( testAdd );
  CPPUNIT_TEST( testCollect );
  CPPUNIT_TEST_SUITE_END();

public:
  void testDummy( void );
  void testAdd( void );
  void testCollect( void );
};

CPPUNIT_TEST_SUITE_REGISTRATION( GarbageCollectorTest );

void GarbageCollectorTest::testDummy( void )
{
  CPPUNIT_ASSERT_EQUAL( 0U, Dummy::Instances );
  {
    Dummy d;
    CPPUNIT_ASSERT_EQUAL( 1U, Dummy::Instances );
  }
  CPPUNIT_ASSERT_EQUAL( 0U, Dummy::Instances );
}


void GarbageCollectorTest::testAdd( void )
{
  Dummy::Ptr d;
  CPPUNIT_ASSERT_EQUAL( 0U, Dummy::Instances );
  {
    GarbageCollector<Dummy> gc;
    d = new Dummy;

    CPPUNIT_ASSERT_EQUAL( 1U, d->refs() );
    gc += d;
    CPPUNIT_ASSERT_EQUAL( 2U, d->refs() );
  }
  CPPUNIT_ASSERT_EQUAL( 1U, d->refs() );
}


void GarbageCollectorTest::testCollect( void )
{
  GarbageCollector<Dummy> gc;

  CPPUNIT_ASSERT_EQUAL( 0U, Dummy::Instances );

  Dummy::Ptr d( new Dummy );
  CPPUNIT_ASSERT_EQUAL( 1U, Dummy::Instances );
  CPPUNIT_ASSERT_EQUAL( 1U, d->refs() );

  gc += d;
  CPPUNIT_ASSERT_EQUAL( 1U, Dummy::Instances );
  CPPUNIT_ASSERT_EQUAL( 2U, d->refs() );
  d = 0;

  CPPUNIT_ASSERT_EQUAL( 1U, Dummy::Instances );
  AppLoop::process();
  CPPUNIT_ASSERT_EQUAL( 0U, Dummy::Instances );
}
