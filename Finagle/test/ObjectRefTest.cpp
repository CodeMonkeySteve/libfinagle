/*!
** \file ObjectRefTest.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Mon Oct 1 2007
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
#include <Finagle/ReferencedObject.h>

using namespace std;
using namespace Finagle;

class ObjectRefTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ObjectRefTest );
  CPPUNIT_TEST( testDynamicCast );
  CPPUNIT_TEST_SUITE_END();

public:
  void testDynamicCast( void );
};


CPPUNIT_TEST_SUITE_REGISTRATION( ObjectRefTest );

struct Base : public ReferenceCount {
  typedef ObjectRef<Base> Ref;
  Base( void ) {}
  virtual ~Base( void ) {}
};

struct Foo : public Base {
  typedef ObjectRef<Foo> Ref;
  Foo( void ) {}
  virtual ~Foo( void ) {}
};

struct Bar : public Base {
  typedef ObjectRef<Bar> Ref;
  Bar( void ) {}
  virtual ~Bar( void ) {}
};

std::ostream &operator <<( std::ostream &out, Base const &b ) {  return out << "Base";  }
std::ostream &operator <<( std::ostream &out, Foo const &f ) {  return out << "Foo";  }
std::ostream &operator <<( std::ostream &out, Bar const &b ) {  return out << "Bar";  }


void ObjectRefTest::testDynamicCast( void )
{
  for ( unsigned i = 0; i < 10000; ++i ) {
    {
      Base *b = new Base;
      Base::Ref r(b);
      CPPUNIT_ASSERT_EQUAL( r, Base::Ref(r) );
      CPPUNIT_ASSERT( !Foo::Ref(r) );
      CPPUNIT_ASSERT( !Bar::Ref(r) );
    }
    {
      Foo *f = new Foo;
      Foo::Ref r(f);
      CPPUNIT_ASSERT_EQUAL( r, Foo::Ref(r) );
      CPPUNIT_ASSERT_EQUAL( Base::Ref((Base *) f), Base::Ref(r) );
      CPPUNIT_ASSERT( !Bar::Ref(r) );
    }
    {
      Foo::Ref f( new Foo );
      CPPUNIT_ASSERT( f );
      Base::Ref b( f );
      CPPUNIT_ASSERT( b );
      Foo::Ref f2( b );
      CPPUNIT_ASSERT( b );
      Base::Ref b2( f2 );
      CPPUNIT_ASSERT( b2 );
      CPPUNIT_ASSERT_NO_THROW( f = 0 );
      CPPUNIT_ASSERT_NO_THROW( b = 0 );
      CPPUNIT_ASSERT_NO_THROW( f2 = 0 );
      CPPUNIT_ASSERT_NO_THROW( b2 = 0 );
    }
  }
}

