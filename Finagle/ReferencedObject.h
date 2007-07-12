/*!
** \file ReferencedObject.h
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

#ifndef FINAGLE_REFERENCEDOBJECT_H
#define FINAGLE_REFERENCEDOBJECT_H

#include <exception>
#include <Finagle/TextString.h>

namespace Finagle {

//! \brief Reference-counting smart pointer.
template <typename Type, typename RType = Type &, typename PType = Type *>
class ObjectRef {
public:
  typedef Type  ObjType;
  typedef RType RefType;
  typedef PType PtrType;

public:
  class NullRefEx : public std::exception {
  public:
    NullRefEx( const char *Func )
    : Msg( String( "Attempt to dereference a NULL pointer in ObjectRef: " ) + Func ) {}
    const char *what( void ) const throw() {
      return( Msg );
    }
    ~NullRefEx( void ) throw() {}
  protected:
    String Msg;
  };

public:
  ObjectRef( PtrType ObjPtr = 0 );
  ObjectRef( RefType ObjRef );
  ObjectRef( ObjectRef<Type,RType,PType> const &Ref );
 ~ObjectRef( void );

  bool isValid( void ) const;

  template <typename OtherType>
  explicit ObjectRef( OtherType *ObjPtr )
  : Ptr( dynamic_cast<PtrType>( ObjPtr ) )
  {
    if ( Ptr )
      Ptr->ref();
  }

  template <typename OtherType>
  explicit ObjectRef( ObjectRef<OtherType> const &Ref )
  : Ptr( dynamic_cast<PtrType>( typename ObjectRef<OtherType>::PtrType( const_cast<ObjectRef<OtherType> &>( Ref ) ) ) )
  {
    if ( Ptr )
      Ptr->ref();
  }

  ObjectRef &operator =( ObjectRef<Type,RType,PType> Ref );

  bool operator ==( PtrType ObjPtr ) const;

  Type const &operator *( void ) const;
  Type const *operator ->( void ) const;

  operator Type const &( void ) const;
  operator Type const *( void ) const;

  RefType operator *( void );
  PtrType operator ->( void );

  operator RefType( void );
  operator PtrType( void );

protected:
  PtrType Ptr;
};


/*! \brief Simple reference-counting implementation
**
** This class implements a reference count, suitable for inheriting into
** classes which are referenced with ObjectRef.  A referenced object need not
** necessarily inherit ReferenceCount, but it must provide the following
** functions.
*/
class ReferenceCount {
public:
  //! Sets reference count to \c 0.
  ReferenceCount( void );

  //! Sets reference count to \c 0.
  ReferenceCount( ReferenceCount const & );

  //! Incremements reference count
  void ref( void ) const;

  //! Decrements reference count.  Returns \c true if the new reference count is \c 0.
  bool deref( void ) const;

  //! Returns the reference count.
  unsigned refCount( void ) const;

protected:
  mutable unsigned RefCount;
};


/*! \brief Iterator for ObjectRef containers.
**
** Provides easier access to containers (e.g. List, Array, etc.) whos
** elements are object references.
*/
template <typename IterType>
class ObjectRefIterator : public IterType {
public:
  typedef typename IterType::value_type::ObjType  ObjType;
  typedef typename IterType::value_type::RefType  RefType;
  typedef typename IterType::value_type::PtrType  PtrType;

public:
  ObjectRefIterator( void ) {}
  ObjectRefIterator( IterType const &It ) : IterType( It ) {}

public:
  RefType operator  *( void ) {  return(  **((IterType) *this) );  }
  PtrType operator ->( void ) {  return( &**((IterType) *this) );  }

  operator RefType( void )   {  return(  **((IterType) *this) );  }
  operator PtrType( void )   {  return( &**((IterType) *this) );  }

  operator typename IterType::value_type( void ) {  return( *((IterType) *this) );  }
};


/*! \brief Constant iterator for ObjectRef containers.
**
** Provides easier access to containers (e.g. List, Array, etc.) whos
** elements are object references.
*/
template <typename IterType>
class ObjectRefConstIterator : public IterType {
public:
  typedef typename IterType::value_type::ObjType const  ObjType;
  typedef typename IterType::value_type::ObjType const &RefType;
  typedef typename IterType::value_type::ObjType const *PtrType;

public:
  ObjectRefConstIterator( void ) {}
  ObjectRefConstIterator( const IterType &It ) : IterType( It ) {}

public:
  RefType operator  *( void ) {  return(  **((IterType) *this) );  }
  PtrType operator ->( void ) {  return( &**((IterType) *this) );  }
  operator RefType( void )   {  return(  **((IterType) *this) );  }
  operator PtrType( void )   {  return( &**((IterType) *this) );  }

  operator typename IterType::value_type( void ) {  return( *((IterType) *this) );  }
};

// INLINE/TEMPLATE IMPLEMENTATION *********************************************

#define CHECK_PTR() \
  if ( !Ptr )  throw NullRefEx( __FUNCTION__ );

template <typename Type, typename RType, typename PType>
inline ObjectRef<Type, RType, PType>::ObjectRef( PtrType ObjPtr )
: Ptr( ObjPtr )
{
  if ( Ptr )
    Ptr->ref();
}

template <typename Type, typename RType, typename PType>
inline ObjectRef<Type, RType, PType>::ObjectRef( RefType ObjRef )
: Ptr( &ObjRef )
{
  ObjRef.ref();
}

template <typename Type, typename RType, typename PType>
inline ObjectRef<Type, RType, PType>::ObjectRef( ObjectRef<Type,RType,PType> const &Ref )
: Ptr( Ref.Ptr )
{
  if ( Ptr )
    Ptr->ref();
}

template <typename Type, typename RType, typename PType>
ObjectRef<Type, RType, PType>::~ObjectRef( void )
{
  if ( Ptr && Ptr->deref() ) {
    delete Ptr;
    Ptr = 0;
  }
}

template <typename Type, typename RType, typename PType>
inline bool ObjectRef<Type, RType, PType>::isValid( void ) const
{
  return Ptr != 0;
}

template <typename Type, typename RType, typename PType>
ObjectRef<Type, RType, PType> &ObjectRef<Type, RType, PType>::operator =( ObjectRef<Type,RType,PType> Ref )
{
  if ( Ref.Ptr )
    Ref.Ptr->ref();

  if ( Ptr && Ptr->deref() )
    delete Ptr;

  Ptr = Ref.Ptr;
  return *this;
}

template <typename Type, typename RType, typename PType>
inline bool ObjectRef<Type, RType, PType>::operator ==( PtrType ObjPtr ) const
{
  return Ptr == ObjPtr;
}

template <typename Type, typename RType, typename PType>
inline Type const &ObjectRef<Type, RType, PType>::operator *( void ) const
{
  CHECK_PTR();
  return *Ptr;
}

template <typename Type, typename RType, typename PType>
inline Type const *ObjectRef<Type, RType, PType>::operator ->( void ) const
{
  CHECK_PTR();
  return Ptr;
}

template <typename Type, typename RType, typename PType>
inline ObjectRef<Type, RType, PType>::operator Type const &( void ) const
{
  CHECK_PTR();
  return( *Ptr );
}

template <typename Type, typename RType, typename PType>
inline ObjectRef<Type, RType, PType>::operator Type const *( void ) const
{
  CHECK_PTR();
  return( Ptr );
}


template <typename Type, typename RType, typename PType>
inline RType ObjectRef<Type, RType, PType>::operator *( void )
{
  CHECK_PTR();
  return( *Ptr );
}

template <typename Type, typename RType, typename PType>
inline PType ObjectRef<Type, RType, PType>::operator ->( void )
{
  CHECK_PTR();
  return( Ptr );
}

template <typename Type, typename RType, typename PType>
inline ObjectRef<Type, RType, PType>::operator RType( void )
{
  CHECK_PTR();
  return( *Ptr );
}

template <typename Type, typename RType, typename PType>
inline ObjectRef<Type, RType, PType>::operator PType( void )
{
  return( Ptr );
}

// PASS-THROUGH OPERATORS -----------------------------------------------------

template <typename Type>
inline bool operator <( ObjectRef<Type> const &A, ObjectRef<Type> const &B )
{  return( *A < *B );  }

// REFERENCE COUNT ------------------------------------------------------------

inline ReferenceCount::ReferenceCount( void )
: RefCount( 0 )
{
}

inline ReferenceCount::ReferenceCount( ReferenceCount const & )
: RefCount( 0 )
{
}

inline void ReferenceCount::ref( void ) const
{
  RefCount++;
}

inline bool ReferenceCount::deref( void ) const
{
  return( --RefCount == 0 );
}

inline unsigned ReferenceCount::refCount( void ) const
{
  return( RefCount );
}


}

#endif
