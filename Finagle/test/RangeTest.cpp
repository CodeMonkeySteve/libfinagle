/*!
** \file RangeTest.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Mon May 8 2007
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
#include <Finagle/Range.h>

#include <iostream>
using namespace std;
using namespace Finagle;

class RangeTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( RangeTest );
  CPPUNIT_TEST( testLowerUpper );
  CPPUNIT_TEST( testRand );
  CPPUNIT_TEST_SUITE_END();

public:
  void testLowerUpper( void );
  void testRand( void );
};


CPPUNIT_TEST_SUITE_REGISTRATION( RangeTest );

void RangeTest::testLowerUpper( void )
{
  Range<int> r( 0, 0 );
  for ( unsigned i = 0; i < 100; ++i ) {
    int n = rand() % 100000;
    if ( n < r.lower() )  r.lower(n);
    if ( n > r.upper() )  r.upper(n);
  }

  CPPUNIT_ASSERT( r.height() > 10000 );
}


void RangeTest::testRand( void )
{
  Range<unsigned > r( 10, 100 );
  for ( unsigned i = 0; i < 100; ++i ) {
    unsigned v = r.rand();
    CPPUNIT_ASSERT( v >= r.lower() );
    CPPUNIT_ASSERT( v <= r.upper() );
  }
}
