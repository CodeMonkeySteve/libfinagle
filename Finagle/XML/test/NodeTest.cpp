/*!
** \file NodeTest.cpp
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
#include <Finagle/XML/Node.h>

using namespace std;
using namespace Finagle;
using namespace XML;

class NodeTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( NodeTest );
  CPPUNIT_TEST( testCreateDestroy );
  CPPUNIT_TEST( testInsertBefore );
  CPPUNIT_TEST( testInsertAfter );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp( void );
  void tearDown( void );

  void testCreateDestroy( void );
  void testInsertBefore( void );
  void testInsertAfter( void );

  class Node : public XML::Node {
  public:
    Node( void ) {}
    virtual XML::Node::Ref dup( void ) const {  return new Node;  }
  };

protected:
  Node::Ref _node;
};


CPPUNIT_TEST_SUITE_REGISTRATION( NodeTest );

void NodeTest::setUp( void )
{
  CPPUNIT_ASSERT_NO_THROW( _node = new Node );
}

void NodeTest::tearDown( void )
{
  CPPUNIT_ASSERT_NO_THROW( _node = 0 );
}


void NodeTest::testCreateDestroy( void )
{
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), _node->parent() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), _node->prev() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), _node->next() );
}


void NodeTest::testInsertBefore( void )
{
  Node::Ref n;
  CPPUNIT_ASSERT_NO_THROW( n = new Node );

  n->insertBefore( _node );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), n->parent() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), n->prev() );
  CPPUNIT_ASSERT_EQUAL( _node,        n->next() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), _node->parent() );
  CPPUNIT_ASSERT_EQUAL( n,            _node->prev() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), _node->next() );

  CPPUNIT_ASSERT_NO_THROW( n->remove() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), n->parent() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), n->prev() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), n->next() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), _node->parent() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), _node->prev() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), _node->next() );
}


void NodeTest::testInsertAfter( void )
{
  Node::Ref n;
  CPPUNIT_ASSERT_NO_THROW( n = new Node );

  n->insertAfter( _node );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), n->parent() );
  CPPUNIT_ASSERT_EQUAL( _node,        n->prev() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), n->next() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), _node->parent() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), _node->prev() );
  CPPUNIT_ASSERT_EQUAL( n,            _node->next() );

  CPPUNIT_ASSERT_NO_THROW( n->remove() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), n->parent() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), n->prev() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), n->next() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), _node->parent() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), _node->prev() );
  CPPUNIT_ASSERT_EQUAL( Node::Ref(0), _node->next() );
}
