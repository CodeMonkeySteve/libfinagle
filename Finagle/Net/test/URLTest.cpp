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

using namespace std;
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
  CPPUNIT_ASSERT_EQUAL( (string) "http://hostname:80/foo",
                        (string const &) URL().host("hostname").port(80).path("foo") );
}

void URLTest::testHTTPFull( void )
{
  URL::ParamMap query;
  query["one"] = "1!";
  query["tw()"] = "2";
  CPPUNIT_ASSERT_EQUAL( (string) "http://user:password@hostname:42/foo/bar?one=1%21&tw%28%29=2#fr%40gment",
                        (string const &) URL().host("hostname").username("user").password("password").port(42)
                                              .path("foo/bar").query(query).fragment("fr@gment").delim('&') );
}
