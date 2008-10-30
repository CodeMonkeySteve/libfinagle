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

#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
#include <Finagle/AppLoop.h>
#include <Finagle/Net/Response.h>

using namespace std;
using namespace Finagle;
using namespace Transfer;

class ResponseTest : public CppUnit::TestFixture, public sigslot::has_slots<> {
  CPPUNIT_TEST_SUITE( ResponseTest );
  CPPUNIT_TEST( testRequestCreateDestroy );
  CPPUNIT_TEST( testRequest );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );

  void testRequestCreateDestroy( void );
  void testRequest( void );

protected:
  void onBodyStart( String const &type, size_t size );
  void onRecvBody( Response const &resp );

protected:
  Response::Ptr _resp;
  bool _haveBody;
};

CPPUNIT_TEST_SUITE_REGISTRATION( ResponseTest );

void ResponseTest::setUp( void )
{
  _resp = 0;
  _haveBody = false;
}


void ResponseTest::testRequestCreateDestroy( void )
{
  Request::Ptr req = new Request( URL("http://www.finagle.org/") );
  CPPUNIT_ASSERT_NO_THROW( req = 0 );
}


void ResponseTest::testRequest( void )
{
  URL url( "http://www.finagle.org/" );

  _resp = new Response( url );
  _resp->recvBody.connect( this, &ResponseTest::onRecvBody );
  _resp->req()->recvBodyStart.connect( this, &ResponseTest::onBodyStart );
  _resp->req()->perform();
  AppLoop::exec();

  CPPUNIT_ASSERT( _haveBody );
  CPPUNIT_ASSERT( _resp->req()->succeeded() );
  CPPUNIT_ASSERT_EQUAL( 200U, _resp->req()->result() );
}


void ResponseTest::onBodyStart( String const &type, size_t size )
{
  CPPUNIT_ASSERT_EQUAL( String("text/html"), type.split(";")[0] );
  CPPUNIT_ASSERT( size > 1024 );
}


void ResponseTest::onRecvBody( Response const &resp )
{
  CPPUNIT_ASSERT_EQUAL( String("text/html"), _resp->type().split(";")[0] );
  CPPUNIT_ASSERT( _resp->size() > 1024 );

  CPPUNIT_ASSERT_EQUAL( _resp->size(), _resp->body().size() );

  _haveBody = true;
  AppLoop::exit( 0 );
}
