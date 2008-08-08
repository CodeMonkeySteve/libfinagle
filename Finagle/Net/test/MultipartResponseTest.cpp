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

#include <cppunit/extensions/HelperMacros.h>
#include <Finagle/AppLoop.h>
#include <Finagle/Net/MultipartResponse.h>

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
  unsigned _numParts;

  static const unsigned totParts;
};

const unsigned MultipartResponseTest::totParts = 10;


CPPUNIT_TEST_SUITE_REGISTRATION( MultipartResponseTest );

void MultipartResponseTest::setUp( void )
{
  _resp = 0;
  _numParts = 0;
}

void MultipartResponseTest::tearDown( void )
{
  if ( _resp )
    delete _resp;
}


void MultipartResponseTest::testRequest( void )
{
//  Request::Ptr req( new Request( URL("http://192.168.10.200/mjpg/1/video.mjpg") ) );
  Request::Ptr req( new Request( URL("http://home.finagle.org/test/server-push-fast.cgi") ) );

  _resp = new MultipartResponse;
  _resp->request( req );
  _resp->recvPart.connect( this, &MultipartResponseTest::onRecvPart );

  req->perform();
  AppLoop::exec();

  CPPUNIT_ASSERT_EQUAL( totParts, _numParts );
}

void MultipartResponseTest::onRecvPart( Response const &resp )
{
// CPPUNIT_ASSERT_EQUAL( String("image/jpeg"), resp.type() );
// CPPUNIT_ASSERT( resp.size() > 10240 );
 CPPUNIT_ASSERT_EQUAL( String("text/plain"), resp.type() );
 CPPUNIT_ASSERT_EQUAL( 35U, resp.size() );

 CPPUNIT_ASSERT_EQUAL( resp.size(), resp.body().size() );

  if ( ++_numParts == totParts ) {
    _resp->recvPart.disconnect( this );
    AppLoop::exit( 0 );
  }
}
