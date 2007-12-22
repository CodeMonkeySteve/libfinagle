/*!
** \file AppLogTest.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Thu Dec 20 2007
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
#include <Finagle/AppLog.h>

using namespace std;
using namespace Finagle;

class LogToString : public LogToStream {
public:
  LogToString( void )
  : LogToStream( _strm.rdbuf() ) {}

  string str( void ) const {  return _strm.str();  }

public:
  ostringstream _strm;
};

class AppLogTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( AppLogTest );
  CPPUNIT_TEST( testLog );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );
  void tearDown( void );

  void testLog( void );

protected:
  ObjectRef<LogToString> _logger;
};

CPPUNIT_TEST_SUITE_REGISTRATION( AppLogTest );

void AppLogTest::setUp( void )
{
  CPPUNIT_ASSERT_NO_THROW( _logger = new LogToString );
}

void AppLogTest::tearDown( void )
{
  CPPUNIT_ASSERT_NO_THROW( _logger = 0 );
}

void AppLogTest::testLog( void )
{
  String s( "This is a test" );
  CPPUNIT_ASSERT_NO_THROW( LOG_INFO << s );
  CPPUNIT_ASSERT_EQUAL( s + "\n", String(_logger->str()) );
}

