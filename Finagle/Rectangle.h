/*!
** \file Rectangle.h
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

#ifndef FINAGLE_RECTANGLE_H
#define FINAGLE_RECTANGLE_H

#include <Finagle/sigslot.h>
#include <Util.h>

namespace Finagle {

using namespace sigslot;

class Rectangle {
public:
  static Range<double> const Arg;

public:
  Rectangle( double Left = 0.0, double Top = 0.0, double Right = 1.0, double Bottom = 1.0 );

  Rectangle &operator=( Rectangle const &that );

  bool operator==( Rectangle const &that );
  bool operator!=( Rectangle const &that );

  Rectangle &operator *=( Rectangle const &that );
  Rectangle  operator *( Rectangle const &that ) const;

  bool full( void ) const;
  bool empty( void ) const;

public:  // Accessors
  double const &x( void ) const;
  double const &y( void ) const;
  double        width( void ) const;
  double        height( void ) const;

  double const &left( void ) const;
  double const &top( void ) const;
  double const &right( void ) const;
  double const &bottom( void ) const;

public:  // Mutators
  void x( double x );
  void y( double y );
  void width( double Width );
  void height( double Height );

  void left( double Left );
  void top( double Top );
  void right( double Right );
  void bottom( double Bottom );

public:
  signal0<> Changed;

protected:
  double Left, Top, Right, Bottom;
};

// INLINE IMPLEMENTATION ******************************************************

inline Rectangle::Rectangle( double Left, double Top, double Right, double Bottom )
: Left( Left ), Top( Top ), Right( Right ), Bottom( Bottom )
{
}

inline Rectangle &Rectangle::operator=( Rectangle const &that )
{
  if ( *this == that )
    return *this;

  Left = that.Left;
  Top = that.Top;
  Right = that.Right;
  Bottom = that.Bottom;
  Changed();

  return *this;
}

inline bool Rectangle::operator==( Rectangle const &that )
{
  return (Left == that.Left) && (Top == that.Top) && (Right == that.Right) && (Bottom == that.Bottom);
}

inline bool Rectangle::operator!=( Rectangle const &that )
{
  return !operator==( that );
}

inline Rectangle Rectangle::operator *( Rectangle const &that ) const
{
  Rectangle r( *this );
  return r *= that;
}

inline bool Rectangle::full( void ) const
{
  return (width() == 1.0) && (height() == 1.0);
}

inline bool Rectangle::empty( void ) const
{
  return (width() == 0.0) || (height() == 0.0);
}

inline double const &Rectangle::x( void ) const
{
  return Left;
}

inline double const &Rectangle::y( void ) const
{
  return Top;
}

inline double Rectangle::width( void ) const
{
  return Right - Left;
}

inline double Rectangle::height( void ) const
{
  return Bottom - Top;
}

inline double const &Rectangle::left( void ) const
{
  return Left;
}

inline double const &Rectangle::top( void ) const
{
  return Top;
}

inline double const &Rectangle::right( void ) const
{
  return Right;
}

inline double const &Rectangle::bottom( void ) const
{
  return Bottom;
}


inline void Rectangle::x( double x )
{
  Arg.clamp( x );
  if ( left() != x ) {
    Rectangle::Left = x;
    Changed();
  }
}

inline void Rectangle::y( double y )
{
  Arg.clamp( y );
  if ( top() != y ) {
    Rectangle::Top = y;
    Changed();
  }
}

inline void Rectangle::left( double Left )
{
  Arg.clamp( Left );
  if ( left() != Left ) {
    Rectangle::Left = Left;
    Changed();
  }
}

inline void Rectangle::top( double Top )
{
  Arg.clamp( Top );
  if ( top() != Top ) {
    Rectangle::Top = Top;
    Changed();
  }
}

inline void Rectangle::right( double Right )
{
  Arg.clamp( Right );
  if ( right() != Right ) {
    Rectangle::Right = Right;
    Changed();
  }
}

inline void Rectangle::bottom( double Bottom )
{
  Arg.clamp( Bottom );
  if ( bottom() != Bottom ) {
    Rectangle::Bottom = Bottom;
    Changed();
  }
}

}

#endif
