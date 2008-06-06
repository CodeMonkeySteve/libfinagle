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
#include <Finagle/ObjectPtr.h>
#include <Finagle/TextString.h>

namespace Finagle {  namespace XML {

class Node : public ReferenceCount {
public:
  typedef ObjectPtr<Node> Ptr;
  typedef ObjectPtr<const Node> ConstPtr;

public:
  Node( void );
  virtual Node::Ptr dup( void ) const = 0;
  virtual ~Node( void );

  Node::ConstPtr parent( void ) const;
  Node::ConstPtr prev( void ) const;
  Node::ConstPtr next( void ) const;

  Node::Ptr parent( void );
  Node::Ptr prev( void );
  Node::Ptr next( void );

  void insertBefore( Node::Ptr sib );
  void insertAfter( Node::Ptr sib );
  void remove( void );

  virtual void render( std::ostream &out ) const;
  virtual String asString( void ) const;

protected:
  virtual void insert( Node::Ptr node );
  virtual void remove( Node::Ptr node );
  virtual void clear( void );
  friend class NodeList;

  Node( Node const & );

protected:
  Node::Ptr _parent, _prev, _next;
};

extern std::ostream &operator <<( std::ostream &out, Node const &node );

// INLINE IMPLEMENTATION **********************************************************************************************************

//! Initializes the node with no parent or siblings.
inline Node::Node( void )
{}

inline Node::Node( Node const & ) 
: ReferenceCount()
{}

inline Node::~Node( void )
{
  clear();
}


//! Returns the parent node (or \c 0, if this is the root node).
inline Node::ConstPtr Node::parent( void ) const
{
  return ConstPtr(_parent);
}

//! Returns the previous sibling node (or \c 0, if this is the parent's first node).
inline Node::ConstPtr Node::prev( void ) const
{
  return ConstPtr(_prev);
}

//! Returns the next sibling node (or \c 0, if this is the parent's last node).
inline Node::ConstPtr Node::next( void ) const
{
  return ConstPtr(_next);
}

//! Returns the parent node (or \c 0, if this is the root node).
inline Node::Ptr Node::parent( void )
{
  return _parent;
}

//! Returns the previous sibling node (or \c 0, if this is the parent's first node).
inline Node::Ptr Node::prev( void )
{
  return _prev;
}

//! Returns the next sibling node (or \c 0, if this is the parent's last node).
inline Node::Ptr Node::next( void )
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
