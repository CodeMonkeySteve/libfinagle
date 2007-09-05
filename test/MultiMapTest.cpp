/*!
** \file MultiMapTest.cpp
** \date Tue Sep 4 2007
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
#include <Finagle/MultiMap.h>
#include <Finagle/TextString.h>

using namespace std;
using namespace Finagle;

class MultiMapTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( MultiMapTest );
  CPPUNIT_TEST( testOrder );
  CPPUNIT_TEST_SUITE_END();

public:
  void testOrder( void );
};

CPPUNIT_TEST_SUITE_REGISTRATION( MultiMapTest );

void MultiMapTest::testOrder( void )
{
  MultiMap<double, int> priMap;

  priMap.insert( 2.0, 2 );
  priMap.insert( 2.0, 1 );
  priMap.insert( 1.0, 4 );
  priMap.insert( 1.0, 3 );
  priMap.insert( 0.0, 6 );
  priMap.insert( 0.0, 5 );

  MultiMap<double, int>::ConstIterator i( priMap.begin() );
  CPPUNIT_ASSERT_EQUAL( 0.0,     i.key() );  CPPUNIT_ASSERT_EQUAL( 6, *i );
  CPPUNIT_ASSERT_EQUAL( 0.0, (++i).key() );  CPPUNIT_ASSERT_EQUAL( 5, *i );
  CPPUNIT_ASSERT_EQUAL( 1.0, (++i).key() );  CPPUNIT_ASSERT_EQUAL( 4, *i );
  CPPUNIT_ASSERT_EQUAL( 1.0, (++i).key() );  CPPUNIT_ASSERT_EQUAL( 3, *i );
  CPPUNIT_ASSERT_EQUAL( 2.0, (++i).key() );  CPPUNIT_ASSERT_EQUAL( 2, *i );
  CPPUNIT_ASSERT_EQUAL( 2.0, (++i).key() );  CPPUNIT_ASSERT_EQUAL( 1, *i );
}
