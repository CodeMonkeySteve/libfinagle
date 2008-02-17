/*!
** \file Initializer.h
** \author Steve Sloan <steve@finagle.org>
** \date Sat Feb 16 2008
** Copyright (C) 2008 by Steve Sloan
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

#ifndef FINAGLE_INITIALIZER_H
#define FINAGLE_INITIALIZER_H

namespace Finagle {

template <typename Func>
class Initializer {
public:
  Initializer( void );
 ~Initializer( void );

private:
  static unsigned _useCount;
  static const Func *_inst;
};

// INLINE IMPLEMENTATION **********************************************************************************************************

template <typename Func>
unsigned Initializer<Func>::_useCount = 0;

template <typename Func>
const Func *Initializer<Func>::_inst = 0;

template <typename Func>
inline Initializer<Func>::Initializer( void )
{
  if ( !_useCount++ )
    _inst = new Func;
}

template <typename Func>
inline Initializer<Func>::~Initializer( void )
{
  if ( !--_useCount )
    delete _inst;
}

};

#endif
