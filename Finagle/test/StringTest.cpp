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
#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include <Finagle/TextString.h>
#include <Finagle/FilePath.h>
#include <Finagle/Util.h>

using namespace std;
using namespace Finagle;

class StringTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( StringTest );
  CPPUNIT_TEST( testInitializers );
  CPPUNIT_TEST( testConversions );
  CPPUNIT_TEST( testSplit );
  CPPUNIT_TEST( testNoCase );
  CPPUNIT_TEST( testNoCaseOut );
  CPPUNIT_TEST_SUITE_END();

public:
  void testInitializers( void );
  void testConversions( void );
  void testSplit( void );
  void testNoCase( void );
  void testNoCaseOut( void );
};


CPPUNIT_TEST_SUITE_REGISTRATION( StringTest );

void StringTest::testInitializers( void )
{
  for ( unsigned i = 0; i <= 100; ++i ) {
    {
      short n = rand();
      ostringstream strm;
      strm << (int) n;

      CPPUNIT_ASSERT_EQUAL( strm.str(), (string) String( (short) n ) );
      CPPUNIT_ASSERT_EQUAL( strm.str(), (string) String( (int) n ) );
      CPPUNIT_ASSERT_EQUAL( strm.str(), (string) String( (long) n ) );
    }

    {
      unsigned short n = rand();
      ostringstream strm;
      strm << (unsigned) n;

      CPPUNIT_ASSERT_EQUAL( strm.str(), (string) String( (unsigned short) n ) );
      CPPUNIT_ASSERT_EQUAL( strm.str(), (string) String( (unsigned) n ) );
      CPPUNIT_ASSERT_EQUAL( strm.str(), (string) String( (unsigned long) n ) );
    }
  }
}


void StringTest::testConversions( void )
{
  for ( unsigned i = 0; i <= 100; ++i ) {
    {
      int n = rand();
      String s( n );

      int i = 0;
      CPPUNIT_ASSERT( s.to<int>( i ) );
      CPPUNIT_ASSERT_EQUAL( n, i );

      CPPUNIT_ASSERT_EQUAL( n, s.as<int>() );
    }

    {
      double n = drand() * 100000;
      String s( n );

      double d = 0;
      CPPUNIT_ASSERT( s.to<>( d ) );
      CPPUNIT_ASSERT_DOUBLES_EQUAL( n, d, 0.000001 );

      CPPUNIT_ASSERT_DOUBLES_EQUAL( n, s.as<double>(), 0.000001 );
    }
  }

  {
    String s( "And now for something completely different ..." );

    String t;
    CPPUNIT_ASSERT( s.to<>( t ) );
    CPPUNIT_ASSERT_EQUAL( s, t );

    CPPUNIT_ASSERT_EQUAL( s, s.as<String>() );
  }

  {
    String s( "foo/bar/baaz" );

    FilePath p;
    CPPUNIT_ASSERT( s.to<>( p ) );
    CPPUNIT_ASSERT_EQUAL( s, p.path() );

    CPPUNIT_ASSERT_EQUAL( s, s.as<FilePath>().path() );
  }
}


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


void StringTest::testNoCase( void )
{
  String s( "this is a test" );
  NoCase nc( s );

  CPPUNIT_ASSERT( nc == s );
  CPPUNIT_ASSERT( s == nc );

  CPPUNIT_ASSERT( nc == String::toUpper(s) );
  CPPUNIT_ASSERT( String::toUpper(s) == nc );

  CPPUNIT_ASSERT( nc == String::toLower(s) );
  CPPUNIT_ASSERT( String::toLower(s) == nc );
}


void StringTest::testNoCaseOut( void )
{
  std::string s( "this is a test" );
  NoCase nc( s );

  std::ostringstream out;
  out << nc;
  CPPUNIT_ASSERT_EQUAL( s, out.str() );
}
