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
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );

  void testCreateDestroy( void );

protected:
  void onReqDone( Response const &resp );

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
}
