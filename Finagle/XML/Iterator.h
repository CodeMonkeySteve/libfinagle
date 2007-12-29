/*!
** \file Iterator.h
** \author Steve Sloan <steve@finagle.org>
** \date Mon Nov 12 2007
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

#ifndef FINAGLE_XML_ITERATOR_H
#define FINAGLE_XML_ITERATOR_H

#include <Finagle/XML/Node.h>

namespace Finagle {  namespace XML {

/*! \brief Utility class for iterating across nodes of a particular type.
**
** Iterates across a node collection, starting with the node passed to the constructor.  Nodes which are not of type
** \a Type are ignored.  If \a Type \e is Node, all nodes are iterated.
*/
template <typename Type = Node>
class Iterator {
public:
  typedef ObjectPtr<Type> TypeRef;

public:
  Iterator( void );
  Iterator( Node::Ptr node );

  operator bool( void ) const;
  operator TypeRef( void );

  Type &operator *( void );
  Type *operator ->( void );
  operator Type &( void );
  operator Type *( void );

  Iterator<Type> &toParent( void );
  Iterator<Type> &toPrev( void );
  Iterator<Type> &toNext( void );

  Iterator<Type> &operator --( void );
  Iterator<Type>  operator --( int );
  Iterator<Type> &operator ++( void );
  Iterator<Type>  operator ++( int );

protected:
  Node::Ptr _node;
};

/*! \brief Utility class for iterating across nodes of a particular type.
**
** \sa Iterator.
*/
template <typename Type = Node>
class ConstIterator : public Iterator<Type const> {
public:
  ConstIterator( void ) {}
  ConstIterator( Node::Ptr node )
  : Iterator<Type const>( node ) {}
  ConstIterator( Node::ConstPtr node )
  : Iterator<Type const>( const_cast<Node *>((Node const *) node) ) {}
};

// INLINE IMPLEMENTATION **********************************************************************************************************

//! Default constructor
template <typename Type>
inline Iterator<Type>::Iterator( void )
{}

/*! \brief Initializes the iterator to point to \a node.
**
** If \a node is not of type \a Type, finds the next sibling node which is (if any).
*/
template <typename Type>
inline Iterator<Type>::Iterator( Node::Ptr node )
: _node(node)
{
  while ( _node ) {
    if ( TypeRef(_node) )  break;
    _node = _node->next();
  }
}

//! Returns \c true if the iterator points to a valid node.
template <typename Type>
inline Iterator<Type>::operator bool( void ) const
{
  return _node;
}

//! Returns an ObjectPtr to the iterated object (may be \c 0).
template <typename Type>
inline Iterator<Type>::operator TypeRef( void )
{
  return _node;
}

//! Returns a reference to the iterated object.
template <typename Type>
inline Type &Iterator<Type>::operator *( void )
{
  return TypeRef(_node);
}

//! Returns a pointer to the iterated object (may be \c 0).
template <typename Type>
inline Type *Iterator<Type>::operator ->( void )
{
  return TypeRef(_node);
}

//! Returns a reference to the iterated object.
template <typename Type>
inline Iterator<Type>::operator Type &( void )
{
  return TypeRef(_node);
}

//! Returns a pointer to the iterated object (may be \c 0).
template <typename Type>
inline Iterator<Type>::operator Type *( void )
{
  return TypeRef(_node);
}


//! Moves the iterator to the parent node.
template <typename Type>
inline Iterator<Type> &Iterator<Type>::toParent( void )
{
  while ( _node ) {
    _node = _node->parent();
    if ( _node && TypeRef(_node) )
      break;
  }
  return *this;
}

//! Moves the iterator to the previous sibling node.
template <typename Type>
inline Iterator<Type> &Iterator<Type>::toPrev( void )
{
  while ( _node ) {
    _node = _node->prev();
    if ( _node && TypeRef(_node) )
      break;
  }
  return *this;
}

//! Moves the iterator to the next sibling node.
template <typename Type>
inline Iterator<Type> &Iterator<Type>::toNext( void )
{
  while ( _node ) {
    _node = _node->next();
    if ( _node && TypeRef(_node) )
      break;
  }
  return *this;
}


//! Pre-decrement operator (alias for #toPrev).
template <typename Type>
inline Iterator<Type> &Iterator<Type>::operator --( void )
{
  return toPrev();
}

//! Post-decrement operator
template <typename Type>
inline Iterator<Type> Iterator<Type>::operator --( int )
{
  Iterator cur( *this );
  operator --();
  return cur;
}

//! Pre-increment operator (alias for #toNext).
template <typename Type>
inline Iterator<Type> &Iterator<Type>::operator ++( void )
{
  return toNext();
}

//! Post-increment operator
template <typename Type>
inline Iterator<Type> Iterator<Type>::operator ++( int )
{
  Iterator cur( *this );
  operator ++();
  return cur;
}


} }

#endif
