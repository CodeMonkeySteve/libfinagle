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

/*! \brief Reference-counting smart pointer.
**
** \sa ReferenceCount, ObjectRefIterator and ObjectRefConstIterator.
*/
template <typename Type, typename RType = Type &, typename PType = Type *>
class ObjectPtr {
public:
  typedef Type  ObjType;
  typedef RType RefType;
  typedef PType PtrType;

public:
  //! %Exception thrown when attempting to dereference an ObjectPtr of \c 0.
  class NullRefEx : public std::exception {
  public:
    NullRefEx( const char *func )
      : _msg( String( "Attempt to dereference a NULL pointer in ObjectPtr: " ) + func ) {}
    ~NullRefEx( void ) throw() {}

    const char *what( void ) const throw() {
      return _msg;
    }
  protected:
    String _msg;
  };

public:
  ObjectPtr( PtrType ptr = 0 );
  ObjectPtr( RefType ref );
  ObjectPtr( ObjectPtr<Type,RType,PType> const &ref );
  template <typename OtherType>  explicit ObjectPtr( OtherType *ptr );
  template <typename OtherType>  explicit ObjectPtr( ObjectPtr<OtherType> const &ref );
 ~ObjectPtr( void );

  ObjectPtr &operator =( ObjectPtr<Type,RType,PType> ref );

  operator bool( void ) const;
  bool operator ==( ObjectPtr<Type,RType,PType> const &ref ) const;
  bool operator ==( PtrType ptr ) const;

  Type const &operator *( void ) const  throw( NullRefEx );
  Type const *operator ->( void ) const;

  operator Type const &( void ) const  throw( NullRefEx );
  operator Type const *( void ) const;

  RefType operator *( void )  throw( NullRefEx );
  PtrType operator ->( void );

  operator RType( void )  throw( NullRefEx );
  operator PType( void );

protected:
  PtrType _ptr;
};


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


/*! \brief Iterator for ObjectPtr containers.
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
  ObjectRefIterator( IterType const &it ) : IterType( it ) {}

public:
  RefType operator  *( void ) {  return  **((IterType) *this);  }
  PtrType operator ->( void ) {  return &**((IterType) *this);  }

  operator RefType( void )   {  return  **((IterType) *this);  }
  operator PtrType( void )   {  return &**((IterType) *this);  }

  operator typename IterType::value_type( void ) {  return *((IterType) *this);  }
};


/*! \brief Constant iterator for ObjectPtr containers.
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
  ObjectRefConstIterator( const IterType &it ) : IterType( it ) {}

public:
  RefType operator  *( void ) {  return  **((IterType) *this);  }
  PtrType operator ->( void ) {  return &**((IterType) *this);  }

  operator RefType( void )   {  return  **((IterType) *this);  }
  operator PtrType( void )   {  return &**((IterType) *this);  }

  operator typename IterType::value_type( void ) {  return *((IterType) *this);  }
};

// INLINE/TEMPLATE IMPLEMENTATION *************************************************************************************************

#define CHECK_PTR() \
  if ( !_ptr )  throw NullRefEx( __FUNCTION__ );

//! Initializes the reference to point to \a ptr (may be \c 0).
template <typename Type, typename RType, typename PType>
inline ObjectPtr<Type, RType, PType>::ObjectPtr( PtrType ptr )
: _ptr( ptr )
{
  if ( _ptr )  _ptr->ref();
}

//! Initializes the reference to point to \a ref.
template <typename Type, typename RType, typename PType>
inline ObjectPtr<Type, RType, PType>::ObjectPtr( RefType ref )
: _ptr( &ref )
{
  ref.ref();
}

//! Initializes the reference to point to \a ref.
template <typename Type, typename RType, typename PType>
inline ObjectPtr<Type, RType, PType>::ObjectPtr( ObjectPtr<Type,RType,PType> const &ref )
: _ptr( ref._ptr )
{
  if ( _ptr )  _ptr->ref();
}

template <typename Type, typename RType, typename PType>
ObjectPtr<Type, RType, PType>::~ObjectPtr( void )
{
  if ( _ptr && _ptr->deref() ) {
    delete _ptr;
    _ptr = 0;
  }
}

//! Returns \c true if the reference is not \c 0.
template <typename Type, typename RType, typename PType>
inline ObjectPtr<Type, RType, PType>::operator bool( void ) const
{
  return _ptr != 0;
}


//! Returns \c true if this reference and \a ref have the same address.
template <typename Type, typename RType, typename PType>
inline bool ObjectPtr<Type, RType, PType>::operator ==( ObjectPtr<Type,RType,PType> const &ref ) const
{
  return _ptr == ref._ptr;
}

//! Returns \c true if this reference and \a ptr have the same address.
template <typename Type, typename RType, typename PType>
inline bool ObjectPtr<Type, RType, PType>::operator ==( PtrType ptr ) const
{
  return _ptr == ptr;
}


/*! \brief Initializes the reference to point to the derived object \a ptr.
**
** Uses \c dynamic_cast to convert the object of \a OtherType to \a Type.
*/
template <typename Type, typename RType, typename PType>
template <typename OtherType>
ObjectPtr<Type, RType, PType>::ObjectPtr( OtherType *ptr )
: _ptr( dynamic_cast<PtrType>( ptr ) )
{
  if ( _ptr )  _ptr->ref();
}

/*! \brief Initializes the reference to point to the derived object \a ref.
**
** Uses \c dynamic_cast and \c const_cast to convert the object of \a OtherType to \a Type.
*/
template <typename Type, typename RType, typename PType>
template <typename OtherType>
ObjectPtr<Type, RType, PType>::ObjectPtr( ObjectPtr<OtherType> const &ref )
: _ptr( dynamic_cast<PtrType>( typename ObjectPtr<OtherType>::PtrType( const_cast<ObjectPtr<OtherType> &>( ref ) ) ) )
{
  if ( _ptr )  _ptr->ref();
}

//! Assignment operator
template <typename Type, typename RType, typename PType>
ObjectPtr<Type, RType, PType> &ObjectPtr<Type, RType, PType>::operator =( ObjectPtr<Type,RType,PType> ref )
{
  if ( ref._ptr )
    ref._ptr->ref();

  if ( _ptr && _ptr->deref() )
    delete _ptr;

  _ptr = ref._ptr;
  return *this;
}

/*! \brief Returns a const-reference to the object.
**
** Will throw NullRefEx if the object reference is \c 0.
*/
template <typename Type, typename RType, typename PType>
inline Type const &ObjectPtr<Type, RType, PType>::operator *( void ) const  throw( NullRefEx )
{
  CHECK_PTR();
  return *_ptr;
}

//! \brief Returns a pointer to the object (may be \c 0).
template <typename Type, typename RType, typename PType>
inline Type const *ObjectPtr<Type, RType, PType>::operator ->( void ) const
{
  return _ptr;
}

/*! \brief Returns a const-reference to the object.
**
** Will throw NullRefEx if the object reference is \c 0.
*/
template <typename Type, typename RType, typename PType>
inline ObjectPtr<Type, RType, PType>::operator Type const &( void ) const  throw( NullRefEx )
{
  CHECK_PTR();
  return *_ptr;
}

//! Returns a const-pointer to the object (may be \c 0).
template <typename Type, typename RType, typename PType>
inline ObjectPtr<Type, RType, PType>::operator Type const *( void ) const
{
  return _ptr;
}


/*! \brief Returns a reference to the object.
**
** Will throw NullRefEx if the object reference is \c 0.
*/
template <typename Type, typename RType, typename PType>
inline RType ObjectPtr<Type, RType, PType>::operator *( void )  throw( NullRefEx )
{
  CHECK_PTR();
  return *_ptr;
}

//! Returns a pointer to the object (may be \c 0).
template <typename Type, typename RType, typename PType>
inline PType ObjectPtr<Type, RType, PType>::operator ->( void )
{
  return _ptr;
}

/*! \brief Returns a reference to the object.
**
** Will throw NullRefEx if the object reference is \c 0.
*/
template <typename Type, typename RType, typename PType>
inline ObjectPtr<Type, RType, PType>::operator RType( void )  throw( NullRefEx )
{
  CHECK_PTR();
  return *_ptr;
}

//! Returns a pointer to the object (may be \c 0).
template <typename Type, typename RType, typename PType>
inline ObjectPtr<Type, RType, PType>::operator PType( void )
{
  return _ptr;
}

// PASS-THROUGH OPERATORS ---------------------------------------------------------------------------------------------------------

//! Pass-through operator
template <typename Type>
inline bool operator <( ObjectPtr<Type> const &a, ObjectPtr<Type> const &b )
{
  return *a < *b;
}

//! Pass-through operator
template <typename Type, typename SrcType>
inline Type &operator <<( ObjectPtr<Type> obj, SrcType const &t )
{
  return *obj << t;
}

//! Pass-through operator
template <typename Type>
inline std::ostream &operator <<( std::ostream &out, ObjectPtr<Type> const &obj )
{
  return out << *obj;
}

//! Pass-through operator
template <typename Type>
inline std::istream &operator >>( std::ostream &in, ObjectPtr<Type> &obj )
{
  return in >> *obj;
}

// REFERENCE COUNT ----------------------------------------------------------------------------------------------------------------

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
