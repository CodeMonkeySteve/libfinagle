/*!
** \file InitializerTest.cpp
** \date Sat Feb 16 2008
** \author Steve Sloan <steve@finagle.org>
** Copyright (C) 2008 by Steve Sloan
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
#include <Finagle/Initializer.h>

using namespace std;
using namespace Finagle;

class InitializerTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( InitializerTest );
  CPPUNIT_TEST( testStartup );
  CPPUNIT_TEST_SUITE_END();

public:
  void testStartup( void );
};

class TestInitializer {
public:
  TestInitializer( void ) { ++runCount; }
 ~TestInitializer( void ) { --runCount; }
  static unsigned runCount;
};
unsigned TestInitializer::runCount = 0;

static Initializer<TestInitializer> Test;

CPPUNIT_TEST_SUITE_REGISTRATION( InitializerTest );

void InitializerTest::testStartup( void )
{
  CPPUNIT_ASSERT_EQUAL( 1U, TestInitializer::runCount );
}

