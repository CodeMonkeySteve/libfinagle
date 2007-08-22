/*!
** \file Range.h
** \author Steve Sloan <steve@finagle.org>
** \date Sun Jan 16 2005
** Copyright (C) 2005 by Steve Sloan
**
** This program is free software; you can redistribute it and/or modify it
** under the terms of the GNU General Public License as published by the
** Free Software Foundation; either version 2 of the License, or (at your
** option) any later version.
**
** This program is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this library; if not, you may access it via the web at
** http://www.gnu.org/copyleft/gpl.html
*/

#ifndef FINAGLE_RANGE_H
#define FINAGLE_RANGE_H

namespace Finagle {

template <typename RType>
class Range {
public:
  typedef RType Type;

public:
  Range( void );
  Range( RType const &lower, RType const &upper );

  Type const &lower( void ) const;
  Type const &upper( void ) const;

  Type const &lower( Type const &lower );
  Type const &upper( Type const &upper );

  bool contains( Type const &val ) const;
  void include( Type const &val );

  bool clamp( Type &val ) const;
  Type height( void ) const;

protected:
  Type _lower, _upper;
};

// INLINE/TEMPLATE IMPLEMENTATION *********************************************


/*! \class Range
** \brief Represents a range of values.
*/

//! Initializes the range to 0..0
template <typename Type>
inline Range<Type>::Range( void )
: _lower(0), _upper(0)
{}


//! Initializes the range to go from \a lower to \a upper.
template <typename Type>
inline Range<Type>::Range( Type const &lower, Type const &upper )
{
  if ( lower <= upper ) {
    _lower = lower;  _upper = upper;
  } else {
    _lower = upper;  _upper = lower;
  }
}

template <typename Type>
inline Type const &Range<Type>::lower( void ) const
{
  return _lower;
}

template <typename Type>
inline Type const &Range<Type>::upper( void ) const
{
  return _upper;
}

template <typename Type>
inline Type const &Range<Type>::lower( Type const &lower )
{
  return( _lower = lower );
}

template <typename Type>
inline Type const &Range<Type>::upper( Type const &upper )
{
  return( _upper = upper );
}

//! Returns \c true if \a val is within [\a lower ... \a Higher], inclusively.
template <typename Type>
inline bool Range<Type>::contains( Type const &val ) const
{
  return (val >= _lower) && (val <= _upper);
}

//! Expands the range to include \a val.
template <typename Type>
void inline Range<Type>::include( Type const &val )
{
       if ( val < _lower )  _lower = val;
  else if ( val > _upper )  _upper = val;
}

//! If \a val is outside of the range, sets it such that it is within the range,
//! and returns \c true.  Returns \c false, otherwise.
template <typename Type>
inline bool Range<Type>::clamp( Type &val ) const
{
  if ( val < _lower )
    val = _lower;
  else
  if ( val > _upper )
    val = _upper;
  else
    return false;

  return true;
}

template <typename Type>
inline Type Range<Type>::height( void ) const
{
  return std::numeric_limits<Type>::is_integer ? (_upper - _lower + 1) : (_upper - _lower);
}


}

#endif
