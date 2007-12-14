/*!
** \file Node.h
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

#ifndef FINAGLE_XML_NODE_H
#define FINAGLE_XML_NODE_H

#include <ostream>
#include <sstream>
#include <Finagle/ReferencedObject.h>
#include <Finagle/TextString.h>

namespace Finagle {  namespace XML {

class Node : public ReferenceCount {
public:
  typedef ObjectRef<Node> Ref;
  typedef ObjectRef<const Node> ConstRef;

public:
  Node( void );
  virtual Node::Ref dup( void ) const = 0;
  virtual ~Node( void );

  Node::ConstRef parent( void ) const;
  Node::ConstRef prev( void ) const;
  Node::ConstRef next( void ) const;

  Node::Ref parent( void );
  Node::Ref prev( void );
  Node::Ref next( void );

  void insertBefore( Node::Ref sib );
  void insertAfter( Node::Ref sib );
  void remove( void );

  virtual void render( std::ostream &out ) const;
  virtual String asString( void ) const;

protected:
  virtual void insert( Node::Ref node );
  virtual void remove( Node::Ref node );
  virtual void clear( void );
  friend class NodeList;

  Node( Node const & ) {}

protected:
  Node::Ref _parent, _prev, _next;
};

extern std::ostream &operator <<( std::ostream &out, Node const &node );

// INLINE IMPLEMENTATION **********************************************************************************************************

//! Initializes the node with no parent or siblings.
inline Node::Node( void )
{}

inline Node::~Node( void )
{
  clear();
}


//! Returns the parent node (or \c 0, if this is the root node).
inline Node::ConstRef Node::parent( void ) const
{
  return ConstRef(_parent);
}

//! Returns the previous sibling node (or \c 0, if this is the parent's first node).
inline Node::ConstRef Node::prev( void ) const
{
  return ConstRef(_prev);
}

//! Returns the next sibling node (or \c 0, if this is the parent's last node).
inline Node::ConstRef Node::next( void ) const
{
  return ConstRef(_next);
}

//! Returns the parent node (or \c 0, if this is the root node).
inline Node::Ref Node::parent( void )
{
  return _parent;
}

//! Returns the previous sibling node (or \c 0, if this is the parent's first node).
inline Node::Ref Node::prev( void )
{
  return _prev;
}

//! Returns the next sibling node (or \c 0, if this is the parent's last node).
inline Node::Ref Node::next( void )
{
  return _next;
}


//! Renders the \a node to the output stream \a out (in %XML form).
inline std::ostream &operator <<( std::ostream &out, Node const &node )
{
  node.render( out );
  return out << std::flush;
}

} }

#endif
