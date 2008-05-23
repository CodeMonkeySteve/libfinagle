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
#include <Finagle/Net/Response.h>

using namespace std;
using namespace Finagle;
using namespace Transfer;

class ResponseTest : public CppUnit::TestFixture, public sigslot::has_slots<> {
  CPPUNIT_TEST_SUITE( ResponseTest );
  CPPUNIT_TEST( testRequest );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );
  void tearDown( void );

  void testRequest( void );

protected:
  void onBodyStart( String const &type, size_t size );

protected:
  Response *_resp;
  bool _haveBody;
};

CPPUNIT_TEST_SUITE_REGISTRATION( ResponseTest );

void ResponseTest::setUp( void )
{
  _resp = 0;
  _haveBody = false;
}

void ResponseTest::tearDown( void )
{
  if ( _resp )
    delete _resp;
}


void ResponseTest::testRequest( void )
{
  Request::Ptr req = new Request( URI("http://www.finagle.org/") );

  _resp = new Response( req, false );
  req->recvBodyStart.connect( this, &ResponseTest::onBodyStart );
  req->perform();

  CPPUNIT_ASSERT( _haveBody );
  CPPUNIT_ASSERT( req->succeeded() );
  CPPUNIT_ASSERT_EQUAL( 200U, req->result() );
}


void ResponseTest::onBodyStart( String const &type, size_t size )
{
  CPPUNIT_ASSERT_EQUAL( type, _resp->type() );
  CPPUNIT_ASSERT_EQUAL( size, _resp->size() );

  CPPUNIT_ASSERT_EQUAL( String("text/html"), _resp->type().split(";")[0] );
  CPPUNIT_ASSERT( _resp->size() > 1024 );
  _haveBody = true;
}
