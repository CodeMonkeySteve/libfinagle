/*!
** \file DirTest.cpp
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
#include <Finagle/Dir.h>

using namespace std;
using namespace Finagle;

class DirTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( DirTest );
  CPPUNIT_TEST( testCreateTemp );
  CPPUNIT_TEST( testCreate );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );
  void tearDown( void );

  void testCreateTemp( void );
  void testCreate( void );

protected:
  TempDir *_sandbox;
};

void DirTest::setUp( void )
{
  _sandbox = new TempDir;
  CPPUNIT_ASSERT( _sandbox->exists() );
}

void DirTest::tearDown( void )
{
  Dir d( _sandbox->path() );
  delete _sandbox;
  CPPUNIT_ASSERT( ! d.exists() );
}


CPPUNIT_TEST_SUITE_REGISTRATION( DirTest );

void DirTest::testCreateTemp( void )
{}

void DirTest::testCreate( void )
{
  FilePath p( *_sandbox + "foo/bar/baaz/splat" );
  Dir d( p.dir() );

  CPPUNIT_ASSERT( ! d.exists() );
  CPPUNIT_ASSERT( d.create() );

  CPPUNIT_ASSERT(   d.exists() );
  CPPUNIT_ASSERT( d.erase( true ) );
  CPPUNIT_ASSERT( ! d.exists() );

  CPPUNIT_ASSERT_EQUAL( *_sandbox + "foo/bar/baaz/splat", p );
}

