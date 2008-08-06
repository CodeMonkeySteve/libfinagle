/*!
** \file IPAddressTest.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Tue Aug 5 2008
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
#include <Finagle/Net/IPAddress.h>

using namespace Finagle;

class IPAddressTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( IPAddressTest );
  CPPUNIT_TEST( testConversion );
  CPPUNIT_TEST_SUITE_END();

public:
  void testConversion( void );
};

CPPUNIT_TEST_SUITE_REGISTRATION( IPAddressTest );


void IPAddressTest::testConversion( void )
{
  String s( IPAddress::local.ipStr() );

  IPAddress p;
  CPPUNIT_ASSERT( s.to<>( p ) );
  CPPUNIT_ASSERT_EQUAL( s, p.ipStr() );

  CPPUNIT_ASSERT_EQUAL( s, s.as<IPAddress>().ipStr() );
}
