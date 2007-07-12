/*!
** \file ThreadFunc.h
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

#ifndef FINAGLE_THREADFUNC_H
#define FINAGLE_THREADFUNC_H

#include <Finagle/Thread.h>

namespace Finagle {

template <typename ClassType, typename FuncType = void (ClassType::*)(void)>
class ClassFuncThread : public Thread {
public:
  ClassFuncThread( ClassType *klass, FuncType func );

protected:
  int exec( void );

private:
  ClassType *_class;
  FuncType _func;
};

// INLINE/TEMPLATE IMPLEMENTATION *********************************************

template <typename ClassType, typename FuncType>
inline ClassFuncThread<ClassType, FuncType>::ClassFuncThread( ClassType *klass, FuncType func )
: _class(klass), _func(func)
{}


template <typename ClassType, typename FuncType>
int ClassFuncThread<ClassType, FuncType>::exec( void )
{
  (_class->*_func)();
  return 0;
}

}

#endif
