/*!
** \file NodeList.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Wed Nov 28 2007
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

#include "NodeList.h"

/*using namespace std;*/
using namespace Finagle;
using namespace XML;

/*! \class Finagle::XML::NodeList
** \brief Represents a linear collection of node.
*/

NodeList::NodeList( NodeList const &that )
{
  for ( ConstIterator node( that.first() ); node; ++node )
    last( node->dup() );
}

Node::Ptr NodeList::dup( void ) const
{
  return (Node *) new NodeList( *this );
}

//! Removes all nodes
void NodeList::clear( void )
{
  for ( Node::Ptr node( _first ), next; node; node = next ) {
    next = node->next();
    node->clear();
  }

  _first = _last = 0;
}

/*! \brief Appends the text in \a str to the last Text node.
**
** If the last node is a Text node, appends the text to that.  Otherwise, creates a new Text node and appends it
** to the list.
*/
NodeList &NodeList::append( String const &str )
{
  Text::Ptr textNode( _last );
  if ( textNode )
    textNode->text() += str;
  else
    last( new Text(str) );

  return *this;
}

/*! \brief Appends \a node to the list.
**
** If \a node is a Text node, and the last node is also a Text node, appends the text in \a node to the last
** Text node.  Otherwise, appends \a node to the list.
*/
NodeList &NodeList::append( Node::Ptr node )
{
  Text::Ptr a( _last ), b( node );
  if ( a && b )
    a->text() += b->text();
  else
    last( node );

  return *this;
}


void NodeList::render( std::ostream &out ) const
{
  for ( ConstIterator n( _first ); n; ++n )
    n->render( out );
}


void NodeList::insert( Node::Ptr node )
{
  if ( !node->prev() )
    _first = node;

  if ( !node->next() )
    _last = node;
}


void NodeList::remove( Node::Ptr node )
{
  if ( _first == node )
    _first = node->next();

  if ( _last == node )
    _last = node->prev();
}
