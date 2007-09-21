/*!
** \file FactoryTest.cpp
** \date Thu Sep 20 2007
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
#include <Finagle/Factory.h>

using namespace std;
using namespace Finagle;

class FactoryTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( FactoryTest );
  CPPUNIT_TEST( testCreate );
  CPPUNIT_TEST( testMultiCreate );
  CPPUNIT_TEST( testUnknown );
  CPPUNIT_TEST_SUITE_END();

public:
  void testCreate( void );
  void testMultiCreate( void );
  void testUnknown( void );
};

CPPUNIT_TEST_SUITE_REGISTRATION( FactoryTest );


struct Base : public ReferenceCount {
  typedef ObjectRef<Base> Ref;
  virtual const char *name( void ) const = 0;
};
struct Foo : public Base {
  typedef ObjectRef<Foo> Ref;
  const char *name( void ) const {  return "Foo";  }
};
struct Bar : public Base {
  typedef ObjectRef<Bar> Ref;
  const char *name( void ) const {  return "Bar";  }
};

static ClassFactory<Foo> _factory;

static FactoryMap<String, Base> _multiFactory;
static ClassFactory<Foo, Base>  _fooFactory( String(Foo().name()), _multiFactory );
static ClassFactory<Bar, Base>  _barFactory( String(Bar().name()), _multiFactory );


void FactoryTest::testCreate( void )
{
  Foo::Ref foo = 0;
  CPPUNIT_ASSERT_NO_THROW( foo = _factory.create() );
  CPPUNIT_ASSERT_EQUAL( String("Foo"), String(foo->name()) );
}

void FactoryTest::testMultiCreate( void )
{
  Base::Ref b = 0;
  CPPUNIT_ASSERT_NO_THROW( b = _multiFactory( Foo().name() ) );
  CPPUNIT_ASSERT_EQUAL( String("Foo"), String(b->name()) );

  CPPUNIT_ASSERT_NO_THROW( b = _multiFactory( Bar().name() ) );
  CPPUNIT_ASSERT_EQUAL( String("Bar"), String(b->name()) );
}

void FactoryTest::testUnknown( void )
{
  CPPUNIT_ASSERT( !_multiFactory( "Unknown" ) );
}
