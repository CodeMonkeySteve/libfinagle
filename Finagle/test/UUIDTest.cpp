/*!
** \file UUIDTest.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Tue July 31 2007
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
#include <Finagle/UUID.h>

using namespace std;
using namespace Finagle;

class UUIDTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( UUIDTest );
  CPPUNIT_TEST( testCreateDestroy );
  CPPUNIT_TEST( testGenerate );
  CPPUNIT_TEST( testImport );
  CPPUNIT_TEST( testExport );
  CPPUNIT_TEST_SUITE_END();

public:
  void testCreateDestroy( void );
  void testGenerate( void );
  void testImport( void );
  void testExport( void );
};


CPPUNIT_TEST_SUITE_REGISTRATION( UUIDTest );

void UUIDTest::testCreateDestroy( void )
{
  UUID *id = 0;
  // Default constructor/destructor
  CPPUNIT_ASSERT_NO_THROW( id = new UUID );
  CPPUNIT_ASSERT_NO_THROW( delete id );

  CPPUNIT_ASSERT_NO_THROW( id = new UUID );
  CPPUNIT_ASSERT_NO_THROW( id->generate() );

  UUID *id2 = 0;
  // Copy constructor
  CPPUNIT_ASSERT_NO_THROW( id2 = new UUID( *id ) );
  CPPUNIT_ASSERT_NO_THROW( delete id2 );

  // Assignment operator
  CPPUNIT_ASSERT_NO_THROW( id2 = new UUID );
  CPPUNIT_ASSERT_NO_THROW( *id2 = *id );
  CPPUNIT_ASSERT_NO_THROW( delete id2 );

  CPPUNIT_ASSERT_NO_THROW( delete id );
}


void UUIDTest::testGenerate( void )
{
  UUID id;
  CPPUNIT_ASSERT( id.isNil() );
  id.generate();
  CPPUNIT_ASSERT( !id.isNil() );

  CPPUNIT_ASSERT(  UUID().isNil() );
  CPPUNIT_ASSERT( !UUID().generate().isNil() );
}


void UUIDTest::testImport( void )
{
  String s("da19ba02-3fcd-11dc-aa5c-0090278776b7");
  UUID id( s );
  CPPUNIT_ASSERT( !id.isNil() );
  CPPUNIT_ASSERT_EQUAL( s, String(id) );
}


void UUIDTest::testExport( void )
{
  String s( UUID().generate() );
  CPPUNIT_ASSERT_EQUAL( 36, (int) s.length() );
  CPPUNIT_ASSERT_EQUAL( '-', s[8] );
  CPPUNIT_ASSERT_EQUAL( '-', s[13] );
  CPPUNIT_ASSERT_EQUAL( '-', s[18] );
  CPPUNIT_ASSERT_EQUAL( '-', s[23] );

  CPPUNIT_ASSERT_EQUAL( String(), String(UUID()) );
}


