/*!
** \file RequestTest.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Thu Oct 30 2008
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
#include <Finagle/AppLoop.h>
#include <Finagle/Net/Request.h>

using namespace std;
using namespace Finagle;
using namespace Transfer;

class RequestTest : public CppUnit::TestFixture, public boost::signals::trackable {
  CPPUNIT_TEST_SUITE( RequestTest );
  CPPUNIT_TEST( testCreateDestroy );
  CPPUNIT_TEST( testFetch );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );

  void testCreateDestroy( void );
  void testFetch( void );

protected:
  void onBodyStart( String const &type, size_t size );
  void onBodyFrag( const char *data, size_t size );

protected:
  Request::Ptr _req;
  String _type;
  size_t _size;
  String _body;
};

CPPUNIT_TEST_SUITE_REGISTRATION( RequestTest );

void RequestTest::setUp( void )
{
  _req = 0;
  _type.clear();
  _size = 0;
  _body.clear();
}


void RequestTest::testCreateDestroy( void )
{
  Request::Ptr req = new Request( URL().host("www.finagle.org") );
  CPPUNIT_ASSERT_NO_THROW( req = 0 );
}


void RequestTest::testFetch( void )
{
  URL url;
  url.host( "www.finagle.org" );

  _req = new Request( url );
  _req->recvBodyStart.connect( boost::bind( &RequestTest::onBodyStart, this, _1, _2 ) );
  _req->recvBodyFrag .connect( boost::bind( &RequestTest::onBodyFrag,  this, _1, _2 ) );
  _req->perform();
  AppLoop::exec();

  CPPUNIT_ASSERT_EQUAL( String("text/html"), _type.split(";")[0] );

  CPPUNIT_ASSERT_EQUAL( _size, _body.size() );
  CPPUNIT_ASSERT( _size > 1024 );
  CPPUNIT_ASSERT( _req->succeeded() );
  CPPUNIT_ASSERT_EQUAL( 200U, _req->result() );
}


void RequestTest::onBodyStart( String const &type, size_t size )
{
  _type = type;
  _size = size;
}

void RequestTest::onBodyFrag( const char *data, size_t size )
{
  _body.append( String( data, size ) );

  if ( _body.size() >= _size )
    AppLoop::exit(0);
}
