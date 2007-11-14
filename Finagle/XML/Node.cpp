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
** \brief Renders this node (including all children) to the given output stream.
*/
void Node::render( std::ostream &out ) const
{}


/*! \internal
** \brief Removes the child node.
**
** Use Node::remove() to remove a particular node.
*/
void Node::remove( Node::Ref child )
{
  child->clear();
}

/*! \internal
** \brief Inserts a child node.
**
** Use #insertBefore or #insertAfter to add a node.
*/
void Node::insert( Node::Ref child )
{
  child->_parent = this;
}

/*! \internal
** \brief Clears the node's DOM pointers (should only be done after removing)
*/
void Node::clear( void )
{
  _prev = _next = _parent = 0;
}
