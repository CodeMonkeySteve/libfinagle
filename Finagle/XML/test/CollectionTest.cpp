/*!
** \file CollectionTest.cpp
** \date Wed Nov 14 2007
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
#include <Finagle/XML/Collection.h>
#include <Finagle/XML/Document.h>

using namespace std;
using namespace Finagle;
using namespace XML;

class Person : public Object {
  WITH_OBJECT_ELEMENT( "person", "name", name() );

public:
  Person( String const &name = String() )
  : _name( name )
  {}

  String const &name( void ) const {  return _name;  }
  unsigned age( void ) const {  return _age;  }

  bool configure( XML::Element const &config ) {
    if ( !Object::configure(config) )
      return false;

    _age = config["age"].as<unsigned>();
    return true;
  }

  XML::Element::Ref configuration( void ) const {
    XML::Element::Ref config( Object::configuration() );
    (*config)["age"] = String(_age);
    return config;
  }

protected:
  String _name;
  unsigned _age;
};


class CollectionTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( CollectionTest );
  CPPUNIT_TEST( testCreateDestroy );
  CPPUNIT_TEST( testConfigure );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );
  void tearDown( void );

  void testCreateDestroy( void );
  void testConfigure( void );

protected:

  Collection<Person> *_people;
};

CPPUNIT_TEST_SUITE_REGISTRATION( CollectionTest );

void CollectionTest::setUp( void )
{
  CPPUNIT_ASSERT_NO_THROW( _people = new Collection<Person>( "people" ) );
}

void CollectionTest::tearDown( void )
{
  CPPUNIT_ASSERT_NO_THROW( delete _people );
}


void CollectionTest::testCreateDestroy( void )
{
  CPPUNIT_ASSERT( _people->empty() );
  CPPUNIT_ASSERT_EQUAL( String("<people/>"), _people->configuration()->asString() );
}

void CollectionTest::testConfigure( void )
{
  String name( "J. Random Luser" );
  unsigned age( 42 );
  {
    Document config( String(
      "<people>"
      "  <person name='" + name + "' age='" + String(age) + "' />"
      "</people>"
    ) );
    CPPUNIT_ASSERT( _people->empty() );
    CPPUNIT_ASSERT( _people->configure( *((Element::Ref) config.root()) ) );
    CPPUNIT_ASSERT_EQUAL( 1U, _people->size() );

    ObjectRef<Person> p( (*_people)[name] );
    CPPUNIT_ASSERT( p );
    CPPUNIT_ASSERT_EQUAL( name, p->name() );
    CPPUNIT_ASSERT_EQUAL( age, p->age() );
  }

  age = 84;
  {
    Document config( String(
      "<person name='" + name + "' age='" + String(age) + "' />"
    ) );
    CPPUNIT_ASSERT_EQUAL( 1U, _people->size() );
    CPPUNIT_ASSERT( _people->configure( *((Element::Ref) config.root()) ) );
    CPPUNIT_ASSERT_EQUAL( 1U, _people->size() );

    ObjectRef<Person> p( (*_people)[name] );
    CPPUNIT_ASSERT( p );
    CPPUNIT_ASSERT_EQUAL( age, p->age() );
  }
}
