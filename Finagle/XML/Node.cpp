/*!
** \file Node.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Sun Nov 11 2007
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

#include "Node.h"

using namespace std;
using namespace Finagle;
using namespace XML;

/*! \namespace Finagle::XML
** \brief provides %XML parsing, representation, iteration, and output.
*/

/*! \class Finagle::XML::Node
** \brief Base class for all %XML nodes.
**
** Provides basic functionality for tracking a node's siblings and parent, as well as adding and removing nodes.
*/

//! Inserts this node before \a sib.
void Node::insertBefore( Node::Ref sib )
{
  _next = sib;
  _prev = sib->_prev;
  if ( _prev )  _prev->_next = this;
  sib->_prev = this;

  _parent = sib->_parent;
  if ( _parent )  _parent->insert( this );
}

//! Inserts this node after \a sib.
void Node::insertAfter( Node::Ref sib )
{
  _prev = sib;
  _next = sib->_next;
  if ( _next )  _next->_prev = this;
  sib->_next = this;

  _parent = sib->_parent;
  if ( _parent )  _parent->insert( this );
}

//! Removes this node from its parent.
void Node::remove( void )
{
  if ( _prev )  _prev->_next = _next;
  if ( _next )  _next->_prev = _prev;
  if ( _parent )
    _parent->remove( this );
  else
    clear();
}

/*! \internal
** \brief Renders this node to the given output stream.
*/
void Node::render( std::ostream &out ) const
{}

//! Renders the \a node as a \c String (in %XML form).
String Node::asString( void ) const
{
  std::ostringstream out;
  out << *this;
  return out.str();
}


/*! \internal
** \brief Removes the child node.
**
** Use Node::remove() to remove a particular node.
*/
void Node::remove( Node::Ref node )
{
  node->clear();
}

/*! \internal
** \brief Inserts a child node.
**
** Use #insertBefore or #insertAfter to add a node.
*/
void Node::insert( Node::Ref node )
{
  node->_parent = this;
}

/*! \internal
** \brief Clears the node's parent and sibling references.
**
** \warning This should only be called \e after the node has been removed from its parent.
*/
void Node::clear( void )
{
  _prev = _next = _parent = 0;
}
