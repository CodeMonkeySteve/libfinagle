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

template <typename Type>
class Iterator {
public:
  typedef ObjectRef<Type> TypeRef;

public:
  Iterator( void );
  Iterator( TypeRef node );

  operator bool( void ) const;
  operator TypeRef( void );

  Type &operator *( void );
  Type *operator ->( void );
  operator Type &( void );
  operator Type *( void );

  Iterator<Type> &toParent( void );
  Iterator<Type> &toPrev( void );
  Iterator<Type> &toNext( void );
  Iterator<Type> &toFirstChild( void );
  Iterator<Type> &toLastChild( void );

  Iterator<Type> &operator --( void );
  Iterator<Type>  operator --( int );
  Iterator<Type> &operator ++( void );
  Iterator<Type>  operator ++( int );

protected:
  TypeRef _node;
};

// INLINE IMPLEMENTATION **********************************************************************************************************

template <typename Type>
inline Iterator<Type>::Iterator( void )
{}

template <typename Type>
inline Iterator<Type>::Iterator( TypeRef node )
: _node(node)
{}

template <typename Type>
inline Iterator<Type>::operator bool( void ) const
{
  return _node;
}

template <typename Type>
inline Iterator<Type>::operator ObjectRef<Type>( void )
{
  return _node;
}

template <typename Type>
inline Type &Iterator<Type>::operator *( void )
{
  return _node;
}

template <typename Type>
inline Type *Iterator<Type>::operator ->( void )
{
  return _node;
}

template <typename Type>
inline Iterator<Type>::operator Type &( void )
{
  return _node;
}

template <typename Type>
inline Iterator<Type>::operator Type *( void )
{
  return _node;
}


template <typename Type>
inline Iterator<Type> &Iterator<Type>::toParent( void )
{
  if ( _node )  _node = _node->parent();
  return *this;
}

template <typename Type>
inline Iterator<Type> &Iterator<Type>::toPrev( void )
{
  if ( _node )  _node = _node->prev();
  return *this;
}

template <typename Type>
inline Iterator<Type> &Iterator<Type>::toNext( void )
{
  if ( _node )  _node = _node->next();
  return *this;
}

template <typename Type>
inline Iterator<Type> &Iterator<Type>::toFirstChild( void )
{
  if ( _node )  _node = _node->firstChild();
  return *this;
}

template <typename Type>
inline Iterator<Type> &Iterator<Type>::toLastChild( void )
{
  if ( _node )  _node = _node->lastChild();
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
