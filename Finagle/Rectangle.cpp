/*!
** \file Rectangle.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Fri Jan. 7 2005
** Copyright (C) 2005 by Steve Sloan
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

#include "Rectangle.h"

using namespace Finagle;

Range<double> const Rectangle::Arg( 0.0, 1.0 );

/*! \class Finagle::Rectangle
** \brief Represents a normalized rectangle (with double precision)
**
** This rectangle class has maximum dimensions of 1.0x1.0, i.e. a unit-square.
*/

Rectangle &Rectangle::operator *=( Rectangle const &that )
{
  if ( that.full() )
    return *this;

  if ( that.empty() ) {
    _left = _top = _right = _bottom = 0.0;
    return *this;
  }

  double w = width(), h = height();
  _left += that._left * w;
  _top += that._top * h;
  _right = _left + (w * that.width());
  _bottom = _top + (h * that.height());

  return *this;
}


void Rectangle::width( double w )
{
  Arg.clamp( w );
  if ( width() == w )
    return;

  if ( width() < w ) {
    // Expand
    _left -= w / 2.0;
    Arg.clamp( _left );
  } else {
    // Shrink
    _left += w / 2.0;
  }

  _right = _left + w;
  Changed();
}


void Rectangle::height( double h )
{
  Arg.clamp( h );
  if ( height() == h )
    return;

  if ( height() < h ) {
    // Expand
    _top -= h / 2.0;
    Arg.clamp( _top );
  } else {
    // Shrink
    _top += h / 2.0;
  }

  _bottom = _top + h;
  Changed();
}
