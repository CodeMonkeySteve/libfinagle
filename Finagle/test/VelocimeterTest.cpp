/*!
** \file VelocimeterTest.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Sat July 21 2007
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
#include <Finagle/Velocimeter.h>
#include <Finagle/Util.h>

using namespace std;
using namespace Finagle;

class VelocimeterTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( VelocimeterTest );
  CPPUNIT_TEST( testRate );
  CPPUNIT_TEST_SUITE_END();

public:
  void testRate( void );
  void testSmooth( void );
};


CPPUNIT_TEST_SUITE_REGISTRATION( VelocimeterTest );

void VelocimeterTest::testRate( void )
{
  Velocimeter v( 0.1 );
  for ( unsigned i = 0; i < 10; ++i ) {
    v += 1;
    sleep( 0.1 );
  }

  CPPUNIT_ASSERT_DOUBLES_EQUAL( 10.0, v.rate(), 0.1 );
}

