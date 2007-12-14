/*!
** \file NodeList.h
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

#ifndef FINAGLE_XML_NODELIST_H
#define FINAGLE_XML_NODELIST_H

#include <Finagle/XML/Node.h>
#include <Finagle/XML/Iterator.h>
#include <Finagle/XML/Text.h>

namespace Finagle {  namespace XML {

class NodeList {
public:
  typedef XML::Iterator<Node>      Iterator;
  typedef XML::ConstIterator<Node> ConstIterator;

public:
  NodeList( void ) {}
  NodeList( NodeList const &that );
  virtual Node::Ref dup( void ) const;
  virtual ~NodeList( void ) {}

  bool empty() const;
  String const &text( void ) const;
  void render( std::ostream &out ) const;

  Node::ConstRef first( void ) const;
  Node::ConstRef last( void ) const;
  Node::Ref first( void );
  Node::Ref last( void );
  Node *first( Node *node );
  Node *last( Node *node );

  NodeList &append( String const &str );
  NodeList &append( Node &node );
  NodeList &append( Node::Ref node );

  void clear( void );

protected:
  virtual void insert( Node::Ref node );
  virtual void remove( Node::Ref node );

protected:
  Node::Ref _first, _last;
};

// INLINE IMPLEMENTATION **********************************************************************************************************

inline bool NodeList::empty() const
{
  return ! _first;
}

/*! \brief Text node accessor
**
** If the element's last node is a Text node, returns its value.  Otherwise returns String::nil.
*/
inline String const &NodeList::text( void ) const
{
  XML::Text::ConstRef t( _last );
  return t ? t->text() : String::nil;
}

//! Returns the first node (or \c 0, if the list is empty).
inline Node::ConstRef NodeList::first( void ) const
{
  return Node::ConstRef(_first);
}

//! Returns the last node (or \c 0, if the list is empty).
inline Node::ConstRef NodeList::last( void ) const
{
  return Node::ConstRef(_last);
}

//! Returns the first node (or \c 0, if the list is empty).
inline Node::Ref NodeList::first( void )
{
  return _first;
}

//! Returns the last node (or \c 0, if the list is empty).
inline Node::Ref NodeList::last( void )
{
  return _last;
}


//! Inserts \a node at head of the list.
inline Node *NodeList::first( Node *node )
{
  if ( empty() )
    insert( node );
  else
    node->insertBefore( _first );

  return node;
}

//! Inserts \a child at the end of the list.
inline Node *NodeList::last( Node *node )
{
  if ( empty() )
    insert( node );
  else
    node->insertAfter( _last );

  return node;
}


inline NodeList &NodeList::append( Node &node )
{
  return append( &node );
}


} }

#endif
