/*!
** \file Singleton.h
** \author Steve Sloan <steve@finagle.org>
** \date Thu Oct 14 2004
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

#ifndef FINAGLE_SINGLETON_H
#define FINAGLE_SINGLETON_H

#include <Finagle/Mutex.h>
#include <Finagle/ReferencedObject.h>

namespace Finagle {

/*!
** Application-wide singleton object.
**
** Creates an instance of the object on first use (except when the copy constructor is used).  Destroy when last the last code unit
** (i.e. source file that includes the executable) has been unloaded.
**
** * Usage
** - Add the Singleton as a static object in your header file(s), e.g.:
**   static Singleton<ThingClass> theThing;
** - Access via function operator, e.g.:
**    theThing().doStuff();
*/
template <typename Type, typename TypePtr = Type *>
class Singleton {
public:
  Singleton( void );
  Singleton( Type const &init );
 ~Singleton( void );

  Type &operator ()( void );
  Type &operator ()( Type *that );

private:
  Type &alloc( void );
  Type &alloc( Type const &init );

private:
  static unsigned _useCount;
  static TypePtr _inst;
};

// INLINE/TEMPLATE IMPLEMENTATION *********************************************

template <typename Type, typename TypePtr>
unsigned Singleton<Type, TypePtr>::_useCount = 0;

template <typename Type, typename TypePtr>
TypePtr Singleton<Type, TypePtr>::_inst = 0;


template <typename Type, typename TypePtr>
inline Singleton<Type, TypePtr>::Singleton( void )
{
  _useCount++;
}

template <typename Type, typename TypePtr>
inline Singleton<Type, TypePtr>::Singleton( Type const &that )
{
  _useCount++;
  _inst = new Type(that);
}

template <typename Type, typename TypePtr>
inline Singleton<Type, TypePtr>::~Singleton( void )
{
  if ( _inst && !--_useCount ) {
    delete _inst;
    _inst = 0;
  }
}

template <typename Type, typename TypePtr>
inline Type &Singleton<Type, TypePtr>::operator ()( void )
{
  return _inst ? *_inst : alloc();
}

template <typename Type, typename TypePtr>
inline Type &Singleton<Type, TypePtr>::operator ()( Type *that )
{
  delete _inst;
  _inst = that;
  return *_inst;
}


template <typename Type, typename TypePtr>
Type &Singleton<Type, TypePtr>::alloc( void )
{
  static Mutex Guard;
  Lock _( Guard );

  if ( !_inst )
    _inst = new Type;

  return *_inst;
}

};

#endif
