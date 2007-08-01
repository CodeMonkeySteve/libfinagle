/*!
** \file UUIDTest.cpp
** \date Tue July 31 2007
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
#include <Finagle/UUID.h>

using namespace std;
using namespace Finagle;

class UUIDTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( UUIDTest );
  CPPUNIT_TEST( testGenerate );
  CPPUNIT_TEST( testImport );
  CPPUNIT_TEST( testExport );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );
  void tearDown( void );

  void testGenerate( void );
  void testImport( void );
  void testExport( void );

protected:
  UUID *_uuid;
};


CPPUNIT_TEST_SUITE_REGISTRATION( UUIDTest );

void UUIDTest::setUp( void )
{
  CPPUNIT_ASSERT_NO_THROW( _uuid = new UUID );

}


void UUIDTest::tearDown( void )
{
  CPPUNIT_ASSERT_NO_THROW( delete _uuid );
}


void UUIDTest::testGenerate( void )
{
  CPPUNIT_ASSERT( _uuid->isNull() );
  _uuid->generate();
  CPPUNIT_ASSERT( !_uuid->isNull() );
}


void UUIDTest::testImport( void )
{
  String s("da19ba02-3fcd-11dc-aa5c-0090278776b7");
  UUID uuid( s );
  CPPUNIT_ASSERT_EQUAL( s, String(s) );
}


void UUIDTest::testExport( void )
{
  testGenerate();
  
  String s( *_uuid );
  CPPUNIT_ASSERT_EQUAL( 37, (int) s.length() );
  CPPUNIT_ASSERT_EQUAL( '-', s(8) );
  CPPUNIT_ASSERT_EQUAL( '-', s(13) );
  CPPUNIT_ASSERT_EQUAL( '-', s(18) );
  CPPUNIT_ASSERT_EQUAL( '-', s(23) );
}


