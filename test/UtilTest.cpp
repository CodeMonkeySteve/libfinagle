/*!
** \file UtilTest.cpp
** \date Mon May 8 2007
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
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include <Finagle/Util.h>

using namespace std;
using namespace Finagle;

class UtilTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( UtilTest );
  CPPUNIT_TEST( testRand );
  CPPUNIT_TEST_SUITE_END();

public:
  void testRand( void );
};


CPPUNIT_TEST_SUITE_REGISTRATION( UtilTest );

void UtilTest::testRand( void )
{
  Range<int> r( 0, 0 );
  for ( unsigned i = 0; i < 10; ++i ) {
    int n = rand() % 100000;
    if ( n < r.lower() )  r.lower(n);
    if ( n > r.upper() )  r.upper(n);
  }

  CPPUNIT_ASSERT( r.height() > 10000 );
}
