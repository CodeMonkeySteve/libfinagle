/*!
** \file DocumentTest.cpp
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
#include <Finagle/XML/Document.h>

using namespace std;
using namespace Finagle;
using namespace XML;

class DocumentTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( DocumentTest );
  CPPUNIT_TEST( testCreateDestroy );
  CPPUNIT_TEST( testChildIndex );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );
  void tearDown( void );

  void testCreateDestroy( void );
  void testChildIndex( void );

protected:
  Document *_doc;
};

static String TestContent =
  "<document>"
  "  Intro text"
  "  <stuff name='foo'>"
  "    Some text"
  "    <item value='42'>Forty-Two</item>"
  "    <item value='83'>Eight-Three</item>"
  "    <item value='3.14159'>Pi (yum!)</item>"
  "    Some more text"
  "  </stuff>"
  "  <stuff name='bar'>Bar's Text</stuff>"
  "  Outro text"
  "</document>";


CPPUNIT_TEST_SUITE_REGISTRATION( DocumentTest );

void DocumentTest::setUp( void )
{
  CPPUNIT_ASSERT_NO_THROW( _doc = new Document( TestContent ) );
}

void DocumentTest::tearDown( void )
{
  CPPUNIT_ASSERT_NO_THROW( _doc = 0 );
}


void DocumentTest::testCreateDestroy( void )
{
  Element::Ref root( _doc->root() );

  CPPUNIT_ASSERT( root );
  CPPUNIT_ASSERT( root->hasChildren() );
  CPPUNIT_ASSERT_EQUAL( String("document"), root->name() );
}


void DocumentTest::testChildIndex( void )
{
  Element::Ref root( _doc->root() );

  CPPUNIT_ASSERT( root );
  CPPUNIT_ASSERT( (*root)("stuff") );
  CPPUNIT_ASSERT( (*root)("stuff")("item") );
  CPPUNIT_ASSERT_EQUAL( 42U, (*root)("stuff")("item")["value"].as<unsigned>() );
}


/*
void DocumentTest::testAppendStr( void )
{
  String str( "this is a test" );

  {
    CPPUNIT_ASSERT( !_el->hasChildren() );
    _el->append( str );
    CPPUNIT_ASSERT_EQUAL( _el->firstChild(), _el->lastChild() );
    CPPUNIT_ASSERT( _el->hasChildren() );

    Text::Ref t( _el->firstChild() );
    CPPUNIT_ASSERT( t );
    CPPUNIT_ASSERT_EQUAL( str, t->text() );
    CPPUNIT_ASSERT_EQUAL( str, _el->text() );
  }

  {
    _el->append( str );
    CPPUNIT_ASSERT_EQUAL( _el->firstChild(), _el->lastChild() );
    CPPUNIT_ASSERT( _el->hasChildren() );

    Text::Ref t( _el->firstChild() );
    CPPUNIT_ASSERT( t );
    CPPUNIT_ASSERT_EQUAL( str + str, t->text() );
    CPPUNIT_ASSERT_EQUAL( str + str, _el->text() );
  }
}


void DocumentTest::testAppendElem( void )
{
  String str( "this is a test" );

  {
    Text::Ref text( new Text(str) );
    CPPUNIT_ASSERT( !_el->hasChildren() );
    _el->append( Node::Ref(text) );
    CPPUNIT_ASSERT_EQUAL( _el->firstChild(), _el->lastChild() );
    CPPUNIT_ASSERT( _el->hasChildren() );

    Text::Ref t( _el->firstChild() );
    CPPUNIT_ASSERT( t );
    CPPUNIT_ASSERT_EQUAL( str, t->text() );
    CPPUNIT_ASSERT_EQUAL( str, _el->text() );
  }

  {
    Text::Ref text( new Text(str) );
    _el->append( Node::Ref(text) );
    CPPUNIT_ASSERT_EQUAL( _el->firstChild(), _el->lastChild() );
    CPPUNIT_ASSERT( _el->hasChildren() );

    Text::Ref t( _el->firstChild() );
    CPPUNIT_ASSERT( t );
    CPPUNIT_ASSERT_EQUAL( str + str, t->text() );
    CPPUNIT_ASSERT_EQUAL( str + str, _el->text() );
  }
}


void DocumentTest::testInsert( void )
{
  Node::Ref a( new Node ), b( new Node ), c( new Node );

  CPPUNIT_ASSERT( !_el->hasChildren() );

  _el->firstChild( a );
  CPPUNIT_ASSERT( _el->hasChildren() );
  CPPUNIT_ASSERT_EQUAL( a, _el->firstChild() );
  CPPUNIT_ASSERT_EQUAL( a, _el->lastChild() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(_el), a->parent() );

  _el->lastChild( b );
  CPPUNIT_ASSERT( _el->hasChildren() );
  CPPUNIT_ASSERT_EQUAL( a, _el->firstChild() );
  CPPUNIT_ASSERT_EQUAL( b, _el->lastChild() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(_el), b->parent() );

  _el->firstChild( c );
  CPPUNIT_ASSERT( _el->hasChildren() );
  CPPUNIT_ASSERT_EQUAL( c, _el->firstChild() );
  CPPUNIT_ASSERT_EQUAL( b, _el->lastChild() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(_el), c->parent() );


  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), c->prev() );
  CPPUNIT_ASSERT_EQUAL( a,            c->next() );

  CPPUNIT_ASSERT_EQUAL( c, a->prev() );
  CPPUNIT_ASSERT_EQUAL( b, a->next() );

  CPPUNIT_ASSERT_EQUAL( a, b->prev() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), b->next() );
}




void DocumentTest::testRender( void )
{
  ostringstream strm;
  _el->render( strm );
  CPPUNIT_ASSERT_EQUAL( string("<foo/>"), strm.str() );
}*/
