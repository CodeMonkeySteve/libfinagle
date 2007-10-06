/*!
** \file Property.h
** \author Steve Sloan <steve@finagle.org>
** \date Wed Dec 24 2003
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

#ifndef FINAGLE_PROPERTY_H
#define FINAGLE_PROPERTY_H

#include <ostream>

namespace Finagle {

template <typename ValType>
class Attribute {
public:
  typedef ValType Type;

  virtual ~Attribute( void ) {}

  virtual Type const &operator()( void ) const = 0;
  virtual void operator()( Type const &val ) = 0;

public:
  operator Type const &( void ) const;
  Type const *operator ->( void ) const;

  Type *operator ->( void ) {  return &(*this)();  }

  Attribute<Type> &operator  =( Type const &val );
  Attribute<Type> &operator +=( Type const &val );
  Attribute<Type> &operator -=( Type const &val );
  Attribute<Type> &operator *=( Type const &val );
  Attribute<Type> &operator /=( Type const &val );

  template <typename Type>
  friend std::ostream &operator <<( std::ostream &out, Attribute<Type> &val );
};


template <typename ValType>
class Property : public Attribute<ValType> {
public:
  typedef ValType Type;

  Property( void ) {}
  Property( Type const &val ) : val( val ) {}

  Type const &operator()( void ) const;
  Type &operator()( void );
  void operator()( Type const &val );

  Property<Type> &operator  =( Type const &val );
  Property<Type> &operator +=( Type const &val );
  Property<Type> &operator -=( Type const &val );
  Property<Type> &operator *=( Type const &val );
  Property<Type> &operator /=( Type const &val );

protected:
  Type _val;
};

/*
template <typename ValType, class Class, void (Class::*SetFunc)( ValType & )>
class ClassProperty : public Property<ValType> {
public:
  typedef ValType Type;

  ClassProperty( Class *Parent ) : Parent( *Parent ) {}
  ClassProperty( Class *Parent, Type const &val ) : Property<Type>( val ), Parent( *Parent ) {}

  Type const &operator()( void ) const;
  void operator()( Type const &val );

  ClassProperty<ValType,Class,SetFunc> &operator  =( Type const &val );
  ClassProperty<ValType,Class,SetFunc> &operator +=( Type const &val );
  ClassProperty<ValType,Class,SetFunc> &operator -=( Type const &val );
  ClassProperty<ValType,Class,SetFunc> &operator *=( Type const &val );
  ClassProperty<ValType,Class,SetFunc> &operator /=( Type const &val );

protected:
  Class &Parent;
};

template <typename ValType, class Class, void (Class::*StoreFunc)( void )>
class StoredProperty : public Property<ValType> {
public:
  typedef ValType Type;

  StoredProperty( Class *Parent ) : Parent( *Parent ) {}
  StoredProperty( Class *Parent, Type const &val ) : Property<Type>( val ), Parent( *Parent ) {}

  Type const &operator()( void ) const;
  void operator()( Type const &val );

  StoredProperty<ValType,Class,StoreFunc> &operator  =( Type const &val );
  StoredProperty<ValType,Class,StoreFunc> &operator +=( Type const &val );
  StoredProperty<ValType,Class,StoreFunc> &operator -=( Type const &val );
  StoredProperty<ValType,Class,StoreFunc> &operator *=( Type const &val );
  StoredProperty<ValType,Class,StoreFunc> &operator /=( Type const &val );

protected:
  Class &Parent;
};
*/

// INLINE IMPLEMENTATION ******************************************************

template <typename ValType>
inline Attribute<ValType>::operator ValType const &( void ) const
{
  return (*this)();
}

template <typename ValType>
inline ValType const *Attribute<ValType>::operator ->( void ) const
{
  return &(*this)();
}


template <typename ValType>
inline Attribute<ValType> &Attribute<ValType>::operator =( ValType const &val )
{
  (*this)( val );
  return *this;
}

template <typename ValType>
inline Attribute<ValType> &Attribute<ValType>::operator +=( ValType const &val )
{
  return *this = *this + val;
}

template <typename ValType>
inline Attribute<ValType> &Attribute<ValType>::operator -=( ValType const &val )
{
  return *this = *this - val;
}

template <typename ValType>
inline Attribute<ValType> &Attribute<ValType>::operator *=( ValType const &val )
{
  return *this = *this * val;
}

template <typename ValType>
inline Attribute<ValType> &Attribute<ValType>::operator /=( ValType const &val )
{
  return *this = *this / val;
}

template <typename Type>
inline std::ostream &operator <<( std::ostream &out, Attribute<Type> &val )
{
  Stream << (const Type &) val;
  return Stream;
}


template <typename ValType>
inline ValType const &Property<ValType>::operator()( void ) const
{
  return val;
}

template <typename ValType>
inline ValType &Property<ValType>::operator()( void )
{
  return val;
}

template <typename ValType>
inline void Property<ValType>::operator()( ValType const &val )
{
  _val = val;
}

template <typename ValType>
inline Property<ValType> &Property<ValType>::operator =( ValType const &val )
{
  _val = val;
  return *this;
}

template <typename ValType>
inline Property<ValType> &Property<ValType>::operator +=( ValType const &val )
{
  _val += val;
  return *this;
}

template <typename ValType>
inline Property<ValType> &Property<ValType>::operator -=( ValType const &val )
{
  _val -= val;
  return *this;
}

template <typename ValType>
inline Property<ValType> &Property<ValType>::operator *=( ValType const &val )
{
  _val *= val;
  return *this;
}

template <typename ValType>
inline Property<ValType> &Property<ValType>::operator /=( ValType const &val )
{
  _val /= val;
  return *this;
}

/*
template <typename ValType, class Class, void (Class::*SetFunc)( ValType & )>
inline ValType const &ClassProperty<ValType,Class,SetFunc>::operator()( void ) const
{
  return val;
}

template <class ValType, class Class, void (Class::*SetFunc)( ValType & )>
inline void ClassProperty<ValType,Class,SetFunc>::operator()( ValType const &val )
{
  (Parent.*SetFunc)( ClassProperty::val = val );
}


template <class ValType, class Class, void (Class::*SetFunc)( ValType & )>
inline ClassProperty<ValType,Class,SetFunc> &ClassProperty<ValType,Class,SetFunc>::operator =( Type const &val )
{
  (Parent.*SetFunc)( ClassProperty::val = val );
  return *this;
}

template <class ValType, class Class, void (Class::*SetFunc)( ValType & )>
inline ClassProperty<ValType,Class,SetFunc> &ClassProperty<ValType,Class,SetFunc>::operator +=( Type const &val )
{
  (Parent.*SetFunc)( ClassProperty::val += val );
  return *this;
}

template <class ValType, class Class, void (Class::*SetFunc)( ValType & )>
inline ClassProperty<ValType,Class,SetFunc> &ClassProperty<ValType,Class,SetFunc>::operator -=( Type const &val )
{
  (Parent.*SetFunc)( ClassProperty::val -= val );
  return *this;
}

template <class ValType, class Class, void (Class::*SetFunc)( ValType & )>
inline ClassProperty<ValType,Class,SetFunc> &ClassProperty<ValType,Class,SetFunc>::operator /=( Type const &val )
{
  (Parent.*SetFunc)( ClassProperty::val /= val );
  return *this;
}
*/
/*
template <class ValType, class Class, void (Class::*StoreFunc)( void )>
inline ValType const &StoredProperty<ValType,Class,StoreFunc>::operator()( void ) const
{
  return val;
}

template <class ValType, class Class, void (Class::*StoreFunc)( void )>
inline void StoredProperty<ValType,Class,StoreFunc>::operator()( ValType const &val )
{
  StoredProperty::val = val;
  (Parent.*StoreFunc)();
}


template <class ValType, class Class, void (Class::*StoreFunc)( void )>
inline StoredProperty<ValType,Class,StoreFunc> &StoredProperty<ValType,Class,StoreFunc>::operator =( Type const &val )
{
  StoredProperty::val = val;
  (Parent.*StoreFunc)();
  return *this;
}

template <class ValType, class Class, void (Class::*StoreFunc)( void )>
inline StoredProperty<ValType,Class,StoreFunc> &StoredProperty<ValType,Class,StoreFunc>::operator +=( Type const &val )
{
  StoredProperty::val += val;
  (Parent.*StoreFunc)();
  return *this;
}

template <class ValType, class Class, void (Class::*StoreFunc)( void )>
inline StoredProperty<ValType,Class,StoreFunc> &StoredProperty<ValType,Class,StoreFunc>::operator -=( Type const &val )
{
  StoredProperty::val -= val;
  (Parent.*StoreFunc)();
  return *this;
}

template <class ValType, class Class, void (Class::*StoreFunc)( void )>
inline StoredProperty<ValType,Class,StoreFunc> &StoredProperty<ValType,Class,StoreFunc>::operator /=( Type const &val )
{
  StoredProperty::val /= val;
  (Parent.*StoreFunc)();
  return *this;
}
*/

}

#endif
