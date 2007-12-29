/*!
** \file ReferenceCount.h
** \author Steve Sloan <steve@finagle.org>
** \date Sat Nov 1 2003
** Copyright (C) 2004 by Steve Sloan
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

#ifndef FINAGLE_REFERENCECOUNT_H
#define FINAGLE_REFERENCECOUNT_H

namespace Finagle {

/*! \brief Simple reference-counting implementation (suitable for ObjectPtr)
**
** This class implements a reference count, suitable for inheriting into
** classes which are referenced with ObjectPtr.  A referenced object need not
** necessarily inherit ReferenceCount, but it must provide the same
** functions.
*/
class ReferenceCount {
public:
  ReferenceCount( void );
  ReferenceCount( ReferenceCount const & );

  void ref( void ) const;
  bool deref( void ) const;
  unsigned refs( void ) const;

protected:
  mutable unsigned _refs;
};

// INLINE/TEMPLATE IMPLEMENTATION *************************************************************************************************

//! Initializes the reference count to \c 0.
inline ReferenceCount::ReferenceCount( void )
: _refs(0)
{}

//! Initializes the reference count to \c 0.
inline ReferenceCount::ReferenceCount( ReferenceCount const & )
: _refs(0)
{}

/*! \brief Incremements the reference count.
**
** \warning this function \e may not be thread-safe.
*/
inline void ReferenceCount::ref( void ) const
{
  _refs++;
}

/*! \brief Decremements the reference count and returns \c true if it's \c 0.
**
** \warning this function \e is not thread-safe.
*/
inline bool ReferenceCount::deref( void ) const
{
  return --_refs == 0;
}

//! Returns the reference count.
inline unsigned ReferenceCount::refs( void ) const
{
  return _refs;
}


}

#endif
