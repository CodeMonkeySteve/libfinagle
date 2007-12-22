/*!
** \file FilePathTest.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Sat July 21 2007
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
#include <Finagle/FilePath.h>

using namespace std;
using namespace Finagle;

class FilePathTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( FilePathTest );
  CPPUNIT_TEST( testIsAbsolute );
  CPPUNIT_TEST_SUITE_END();

public:
  void testIsAbsolute( void );
};


CPPUNIT_TEST_SUITE_REGISTRATION( FilePathTest );

void FilePathTest::testIsAbsolute( void )
{
  CPPUNIT_ASSERT( FilePath("/foo/bar/baaz").isAbsolute() );
  CPPUNIT_ASSERT( !FilePath("foo/bar/baaz").isAbsolute() );
}

