/*!
** \file ElementTest.cpp
** \date Tue Nov 13 2007
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
#include <Finagle/XML/Element.h>

using namespace std;
using namespace Finagle;
using namespace XML;

class ElementTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( ElementTest );
  CPPUNIT_TEST( testCreateDestroy );
  CPPUNIT_TEST( testAppendStr );
  CPPUNIT_TEST( testAppendElem );
  CPPUNIT_TEST( testInsert );
  CPPUNIT_TEST( testChildIndex );
  CPPUNIT_TEST( testRender );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );
  void tearDown( void );

  void testCreateDestroy( void );
  void testAppendStr( void );
  void testAppendElem( void );
  void testInsert( void );
  void testChildIndex( void );
  void testRender( void );

protected:
  Element::Ref _el;
};


CPPUNIT_TEST_SUITE_REGISTRATION( ElementTest );

void ElementTest::setUp( void )
{
  CPPUNIT_ASSERT_NO_THROW( _el = new Element("foo") );
}

void ElementTest::tearDown( void )
{
  CPPUNIT_ASSERT_NO_THROW( _el = 0 );
}


void ElementTest::testCreateDestroy( void )
{
  CPPUNIT_ASSERT_EQUAL( String("foo"), _el->name() );
  CPPUNIT_ASSERT_EQUAL( 0U,            _el->attribs().size() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0),  _el->parent() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0),  _el->prev() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0),  _el->next() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0),  _el->first() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0),  _el->last() );

  Element::Ref el;
  CPPUNIT_ASSERT_NO_THROW( el = new Element( "foo", "splat" ) );
  CPPUNIT_ASSERT( el->first() );
  CPPUNIT_ASSERT_EQUAL( el->first(), el->last() );
  CPPUNIT_ASSERT( Text::Ref( el->first() ) );
}


void ElementTest::testAppendStr( void )
{
  String str( "this is a test" );

  {
    CPPUNIT_ASSERT( !_el->hasChildren() );
    _el->append( str );
    CPPUNIT_ASSERT_EQUAL( _el->first(), _el->last() );
    CPPUNIT_ASSERT( _el->hasChildren() );

    Text::Ref t( _el->first() );
    CPPUNIT_ASSERT( t );
    CPPUNIT_ASSERT_EQUAL( str, t->text() );
    CPPUNIT_ASSERT_EQUAL( str, _el->text() );
  }

  {
    _el->append( str );
    CPPUNIT_ASSERT_EQUAL( _el->first(), _el->last() );
    CPPUNIT_ASSERT( _el->hasChildren() );

    Text::Ref t( _el->first() );
    CPPUNIT_ASSERT( t );
    CPPUNIT_ASSERT_EQUAL( str + str, t->text() );
    CPPUNIT_ASSERT_EQUAL( str + str, _el->text() );
  }
}


void ElementTest::testAppendElem( void )
{
  String str( "this is a test" );

  {
    Text::Ref text( new Text(str) );
    CPPUNIT_ASSERT( !_el->hasChildren() );
    _el->append( Node::Ref(text) );
    CPPUNIT_ASSERT_EQUAL( _el->first(), _el->last() );
    CPPUNIT_ASSERT( _el->hasChildren() );

    Text::Ref t( _el->first() );
    CPPUNIT_ASSERT( t );
    CPPUNIT_ASSERT_EQUAL( str, t->text() );
    CPPUNIT_ASSERT_EQUAL( str, _el->text() );
  }

  {
    Text::Ref text( new Text(str) );
    _el->append( Node::Ref(text) );
    CPPUNIT_ASSERT_EQUAL( _el->first(), _el->last() );
    CPPUNIT_ASSERT( _el->hasChildren() );

    Text::Ref t( _el->first() );
    CPPUNIT_ASSERT( t );
    CPPUNIT_ASSERT_EQUAL( str + str, t->text() );
    CPPUNIT_ASSERT_EQUAL( str + str, _el->text() );
  }
}


void ElementTest::testInsert( void )
{
  Node::Ref a( new Node ), b( new Node ), c( new Node );

  CPPUNIT_ASSERT( !_el->hasChildren() );

  _el->first( a );
  CPPUNIT_ASSERT( _el->hasChildren() );
  CPPUNIT_ASSERT_EQUAL( a, _el->first() );
  CPPUNIT_ASSERT_EQUAL( a, _el->last() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(_el), a->parent() );

  _el->last( b );
  CPPUNIT_ASSERT( _el->hasChildren() );
  CPPUNIT_ASSERT_EQUAL( a, _el->first() );
  CPPUNIT_ASSERT_EQUAL( b, _el->last() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(_el), b->parent() );

  _el->first( c );
  CPPUNIT_ASSERT( _el->hasChildren() );
  CPPUNIT_ASSERT_EQUAL( c, _el->first() );
  CPPUNIT_ASSERT_EQUAL( b, _el->last() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(_el), c->parent() );


  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), c->prev() );
  CPPUNIT_ASSERT_EQUAL( a,            c->next() );

  CPPUNIT_ASSERT_EQUAL( c, a->prev() );
  CPPUNIT_ASSERT_EQUAL( b, a->next() );

  CPPUNIT_ASSERT_EQUAL( a, b->prev() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), b->next() );
}


void ElementTest::testChildIndex( void )
{
  Element::Ref el, tmp;
  tmp = (Element *) _el->last( new Element("bar") );
  CPPUNIT_ASSERT_EQUAL( (Element const *) tmp, &(*_el)("bar") );
  el = tmp;

  tmp = (Element *) el->last( new Element("baaz") );
  CPPUNIT_ASSERT_EQUAL( (Element const *) tmp, &(*el)("baaz") );
  el = tmp;

  tmp = (Element *) el->last( new Element("splat") );
  CPPUNIT_ASSERT_EQUAL( (Element const *) tmp, &(*el)("splat") );
  CPPUNIT_ASSERT_EQUAL( (Element const *) tmp, &(*_el)("bar")("baaz")("splat") );
  el = tmp;

  el->attrib("value") = "42";
  CPPUNIT_ASSERT_EQUAL( String("42"), el->attrib("value") );

  CPPUNIT_ASSERT_EQUAL( 42U, (*_el)("bar")("baaz")("splat")["value"].as<unsigned>() );
}


void ElementTest::testRender( void )
{
  ostringstream strm;
  _el->render( strm );
  CPPUNIT_ASSERT_EQUAL( string("<foo/>"), strm.str() );
}
