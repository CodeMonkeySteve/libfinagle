/*!
** \file MultipartResponseTest.cpp
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
#include <Finagle/Net/MultipartResponse.h>

using namespace std;
using namespace Finagle;
using namespace Transfer;

class MultipartResponseTest : public CppUnit::TestFixture, public sigslot::has_slots<> {
  CPPUNIT_TEST_SUITE( MultipartResponseTest );
  CPPUNIT_TEST( testRequest );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );
  void tearDown( void );

  void testRequest( void );

protected:
  void onRecvPart( Response const &resp );

  MultipartResponse *_resp;
  bool _havePart;
};


CPPUNIT_TEST_SUITE_REGISTRATION( MultipartResponseTest );

void MultipartResponseTest::setUp( void )
{
  _resp = 0;
  _havePart = false;
}

void MultipartResponseTest::tearDown( void )
{
  if ( _resp )
    delete _resp;
}


void MultipartResponseTest::testRequest( void )
{
//  Request::Ptr req( new Request( URI("http://192.168.10.200/mjpg/1/video.mjpg") ) );
  Request::Ptr req( new Request( URI("http://home.finagle.org/test/server-push.cgi") ) );

  _resp = new MultipartResponse ( req );
  _resp->recvPart.connect( this, &MultipartResponseTest::onRecvPart );
  req->perform();
  AppLoop::exec();

  CPPUNIT_ASSERT( _havePart );
}

void MultipartResponseTest::onRecvPart( Response const &resp )
{
  CPPUNIT_ASSERT_EQUAL( String("text/plain"), resp.type() );
  CPPUNIT_ASSERT_EQUAL( 28U, resp.size() );
  _havePart = true;

  _resp->recvPart.disconnect( this );
  AppLoop::exit( 0 );
}
