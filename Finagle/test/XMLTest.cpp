/*!
** \file XMLTest.cpp
** \date Sat July 21 2007
** \author Steve Sloan <steve@finagle.org>
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
#include <Finagle/XML.h>

using namespace std;
using namespace Finagle;

class XMLTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( XMLTest );
  CPPUNIT_TEST( testParse );
  CPPUNIT_TEST_SUITE_END();

public:
  void testParse( void );
};


CPPUNIT_TEST_SUITE_REGISTRATION( XMLTest );

void XMLTest::testParse( void )
{
}

