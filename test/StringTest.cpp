/*!
** \file StringTest.cpp
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
#include <cppunit/extensions/HelperMacros.h>
#include <Finagle/TextString.h>

using namespace std;
using namespace Finagle;

class StringTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( StringTest );
  CPPUNIT_TEST( testSplit );
  CPPUNIT_TEST_SUITE_END();

public:
  void testSplit( void );
};


CPPUNIT_TEST_SUITE_REGISTRATION( StringTest );

void StringTest::testSplit( void )
{
  {
    String s( "" );
    String::Array a( s.split(':') );
    CPPUNIT_ASSERT_EQUAL( size_t(1), a.size() );
    CPPUNIT_ASSERT_EQUAL( String(),  a[0] );
  }

  {
    String s( "foo" );
    String::Array a( s.split(':') );
    CPPUNIT_ASSERT_EQUAL( size_t(1), a.size() );
    CPPUNIT_ASSERT_EQUAL( s,         a[0] );
  }

  {
    String s( ":" );
    String::Array a( s.split(':') );
    CPPUNIT_ASSERT_EQUAL( size_t(2), a.size() );
    CPPUNIT_ASSERT_EQUAL( String(),  a[0] );
    CPPUNIT_ASSERT_EQUAL( String(),  a[1] );
  }

  {
    String s( "foo:" );
    String::Array a( s.split(':') );
    CPPUNIT_ASSERT_EQUAL( size_t(2),     a.size() );
    CPPUNIT_ASSERT_EQUAL( String("foo"), a[0] );
    CPPUNIT_ASSERT_EQUAL( String(),      a[1] );
  }

  {
    String s( ":bar" );
    String::Array a( s.split(':') );
    CPPUNIT_ASSERT_EQUAL( size_t(2),     a.size() );
    CPPUNIT_ASSERT_EQUAL( String(),      a[0] );
    CPPUNIT_ASSERT_EQUAL( String("bar"), a[1] );
  }

  {
    String s( "foo:bar" );
    String::Array a( s.split(':') );
    CPPUNIT_ASSERT_EQUAL( size_t(2),     a.size() );
    CPPUNIT_ASSERT_EQUAL( String("foo"), a[0] );
    CPPUNIT_ASSERT_EQUAL( String("bar"), a[1] );
  }
}

