/*!
** \file URLTest.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Fri Aug 1 2008
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

#include <cppunit/extensions/HelperMacros.h>
#include <Finagle/Net/URL.h>

using namespace Finagle;

class URLTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE( URLTest );
  CPPUNIT_TEST( testHTTPSimple );
  CPPUNIT_TEST( testHTTPFull );
  CPPUNIT_TEST_SUITE_END();

public:
  void testHTTPSimple( void );
  void testHTTPFull( void );
};

CPPUNIT_TEST_SUITE_REGISTRATION( URLTest );


void URLTest::testHTTPSimple( void )
{
  CPPUNIT_ASSERT_EQUAL( URL("http://hostname:80/foo"),
                        URL::HTTP( String(), IPAddress("hostname"), 80, "foo" ) );
}

void URLTest::testHTTPFull( void )
{
  Map<String, String> query;
  query["one"] = "1!";
  query["tw()"] = "2";
  CPPUNIT_ASSERT_EQUAL( URL("http://user:password@hostname:42/foo/bar?one=1%21&tw%28%29=2#fr%40gment"),
                        URL::HTTP( "user:password", IPAddress("hostname"), 42, "foo/bar", query, "fr@gment", '&' ) );
}
