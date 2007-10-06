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
  Rectangle( double left = 0.0, double top = 0.0, double right = 1.0, double bottom = 1.0 );

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
  void width( double width );
  void height( double height );

  void left( double left );
  void top( double top );
  void right( double right );
  void bottom( double bottom );

public:
  //! Emitted when the rectangles coordinates have changed
  signal0<> Changed;

protected:
  double _left, _top, _right, _bottom;
};

// INLINE IMPLEMENTATION ******************************************************

inline Rectangle::Rectangle( double left, double top, double right, double bottom )
: _left( left ), _top( top ), _right( right ), _bottom( bottom )
{}

inline Rectangle &Rectangle::operator=( Rectangle const &that )
{
  if ( *this == that )
    return *this;

  _left = that._left;
  _top = that._top;
  _right = that._right;
  _bottom = that._bottom;
  Changed();

  return *this;
}

inline bool Rectangle::operator==( Rectangle const &that )
{
  return (_left == that._left) && (_top == that._top) && (_right == that._right) && (_bottom == that._bottom);
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

//! Returns \c true if the rectangle is a unit square
inline bool Rectangle::full( void ) const
{
  return (width() == 1.0) && (height() == 1.0);
}

//! Returns \c true if the rectangle has zero dimensions
inline bool Rectangle::empty( void ) const
{
  return (width() == 0.0) || (height() == 0.0);
}

//! Returns the left coordinate
inline double const &Rectangle::x( void ) const
{
  return _left;
}

//! Returns the upper coordinate
inline double const &Rectangle::y( void ) const
{
  return _top;
}

//! Returns the width
inline double Rectangle::width( void ) const
{
  return _right - _left;
}

//! Returns the height
inline double Rectangle::height( void ) const
{
  return _bottom - _top;
}

//! Returns the left coordinate
inline double const &Rectangle::left( void ) const
{
  return _left;
}

//! Returns the upper coordinate
inline double const &Rectangle::top( void ) const
{
  return _top;
}

//! Returns the right coordinate
inline double const &Rectangle::right( void ) const
{
  return _right;
}

//! Returns the lower coordinate
inline double const &Rectangle::bottom( void ) const
{
  return _bottom;
}


//! Sets the left coordinate, clamped to within the unit square.
inline void Rectangle::x( double x )
{
  Arg.clamp( x );
  if ( left() != x ) {
    _left = x;
    Changed();
  }
}

//! Sets the upper coordinate, clamped to within the unit square.
inline void Rectangle::y( double y )
{
  Arg.clamp( y );
  if ( top() != y ) {
    _top = y;
    Changed();
  }
}

//! Sets the left coordinate, clamped to within the unit square.
inline void Rectangle::left( double left )
{
  Arg.clamp( _left );
  if ( _left != left ) {
    _left = left;
    Changed();
  }
}

//! Sets the upper coordinate, clamped to within the unit square.
inline void Rectangle::top( double _top )
{
  Arg.clamp( _top );
  if ( top() != _top ) {
    Rectangle::_top = _top;
    Changed();
  }
}

//! Sets the right coordinate, clamped to within the unit square.
inline void Rectangle::right( double _right )
{
  Arg.clamp( _right );
  if ( right() != _right ) {
    Rectangle::_right = _right;
    Changed();
  }
}

//! Sets the lower coordinate, clamped to within the unit square.
inline void Rectangle::bottom( double _bottom )
{
  Arg.clamp( _bottom );
  if ( bottom() != _bottom ) {
    Rectangle::_bottom = _bottom;
    Changed();
  }
}

}

#endif
