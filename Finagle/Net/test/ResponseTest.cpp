/*!
** \file ResponseTest.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Wed May 7 2008
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
#include <Finagle/AppLoop.h>
#include <Finagle/Net/Response.h>

using namespace std;
using namespace Finagle;
using namespace Transfer;

class ResponseTest : public CppUnit::TestFixture, public boost::signals::trackable {
  CPPUNIT_TEST_SUITE( ResponseTest );
  CPPUNIT_TEST( testCreateDestroy );
  CPPUNIT_TEST( testFetch );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );

  void testCreateDestroy( void );
  void testFetch( void );

protected:
  void onBody( void );

protected:
  Response::Ptr _resp;
};

CPPUNIT_TEST_SUITE_REGISTRATION( ResponseTest );

void ResponseTest::setUp( void )
{
  _resp = 0;
}


void ResponseTest::testCreateDestroy( void )
{
  Response::Ptr resp = new Response( URL().host("www.finagle.org") );
  CPPUNIT_ASSERT( resp->request() );
  CPPUNIT_ASSERT_NO_THROW( resp = 0 );
}


void ResponseTest::testFetch( void )
{
  URL url;
  url.host("testcam").username("root").password("dev").path("axis-cgi/view/param.cgi").param("action", "list");

  _resp = new Response( url );
  _resp->recvBody.connect( boost::bind( &ResponseTest::onBody, this ) );
  _resp->request()->perform();
  AppLoop::exec();
}


void ResponseTest::onBody( void )
{
   CPPUNIT_ASSERT( _resp->request()->succeeded() );
   CPPUNIT_ASSERT_EQUAL( 200U, _resp->request()->result() );

//   CPPUNIT_ASSERT_EQUAL( String("text/html"), _type.split(";")[0] );
//   CPPUNIT_ASSERT( !resp.body().empty() );
  AppLoop::exit( 0 );
}
