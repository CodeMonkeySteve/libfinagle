/*!
** \file Velocimeter.h
** \author Steve Sloan <steve@finagle.org>
** \date Wed Feb 20 2008
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

#ifndef FINAGLE_VELOCIMETER_H
#define FINAGLE_VELOCIMETER_H

#include <Finagle/DateTime.h>

namespace Finagle {

class Velocimeter {
public:
  Velocimeter( double smooth = 1.0 );

  double rate( void ) const;
  Velocimeter &operator +=( double incr );

protected:
  double _rate, _smooth;
  Time _lastIncr;
};

// INLINE IMPLEMENTATION ******************************************************

inline Velocimeter::Velocimeter( double smooth )
: _rate( 0.0 ), _smooth( 1.0 / smooth ), _lastIncr( Time::now() )
{}

inline Velocimeter &Velocimeter::operator +=( double incr )
{
  Time now = Time::now();
  double dt = now - _lastIncr;
  _lastIncr = now;

  double e = exp( -dt * _smooth );
  _rate = (e * _rate) + ((1 - e) * (incr / dt));
  return *this;
}

inline double Velocimeter::rate( void ) const
{
  return _rate;
}

};

#endif
