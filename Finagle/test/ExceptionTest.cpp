/*!
** \file ExceptionTest.cpp
** \date Mon Nov 12 2007
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
#include <Finagle/Exception.h>

using namespace std;
using namespace Finagle;

class ExceptionTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ExceptionTest );
  CPPUNIT_TEST( testCreateDestroy );
//  CPPUNIT_TEST( testThrow );
  CPPUNIT_TEST_SUITE_END();

public:
  void testCreateDestroy( void );
  void testThrow( void );
};

CPPUNIT_TEST_SUITE_REGISTRATION( ExceptionTest );


void ExceptionTest::testCreateDestroy( void )
{
  CPPUNIT_ASSERT_NO_THROW( Exception() );

  Exception ex( "This is a test" );
cout << ex << endl;

  CPPUNIT_ASSERT_NO_THROW( Exception( "This is a test" ) );
}

void ExceptionTest::testThrow( void )
{
  Exception ex( "This is a test" );
  CPPUNIT_ASSERT_EQUAL( String("This is a test"), String(ex.what()) );

  CPPUNIT_ASSERT_THROW( throw ex, Exception );
  try {
    throw ex;
    CPPUNIT_ASSERT_THROW( throw ex, Exception );
  } catch ( Exception &e ) {
    CPPUNIT_ASSERT_EQUAL( String(ex.what()), String(e.what()) );
  }
}

