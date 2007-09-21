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

/*! \class Rectangle
** \brief A normalized rectangle
*/

Rectangle &Rectangle::operator *=( Rectangle const &that )
{
  if ( that.full() )
    return *this;

  if ( that.empty() ) {
    Left = Top = Right = Bottom = 0.0;
    return *this;
  }

  double w = width(), h = height();
  Left += that.Left * w;
  Top += that.Top * h;
  Right = Left + (w * that.width());
  Bottom = Top + (h * that.height());

  return *this;
}


void Rectangle::width( double Width )
{
  Arg.clamp( Width );
  if ( width() == Width )
    return;

  if ( width() < Width ) {
    // Expand
    Left -= Width / 2.0;
    Arg.clamp( Left );
  } else {
    // Shrink
    Left += Width / 2.0;
  }

  Right = Left + Width;
  Changed();
}


void Rectangle::height( double Height )
{
  Arg.clamp( Height );
  if ( height() == Height )
    return;

  if ( height() < Height ) {
    // Expand
    Top -= Height / 2.0;
    Arg.clamp( Top );
  } else {
    // Shrink
    Top += Height / 2.0;
  }

  Bottom = Top + Height;
  Changed();
}
