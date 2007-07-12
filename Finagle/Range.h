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

template <typename ValType>
class Range {
public:
  typedef ValType Type;

public:
  Range( void );
  Range( ValType const &Lower, ValType const &Upper );

  Type const &lower( void ) const;
  Type const &upper( void ) const;

  Type const &lower( Type const &Lower );
  Type const &upper( Type const &Upper );

  bool contains( Type const &Val ) const;
  bool clamp( Type &Val ) const;
  Type height( void ) const;

protected:
  Type Lower, Upper;
};

// INLINE/TEMPLATE IMPLEMENTATION *********************************************


/*! \class Range
** \brief Represents a range of values.
*/

//! Initializes the range to the maximum possible range.
template <typename Type>
inline Range<Type>::Range( void )
: Lower( std::numeric_limits<Type>::min() ),
  Upper( std::numeric_limits<Type>::max() )
{
}

//! Initializes the range to go from \a Lower to \a Higher.
template <typename Type>
inline Range<Type>::Range( Type const &Lower, Type const &Upper )
: Lower( Lower ), Upper( Upper )
{
}

template <typename Type>
inline Type const &Range<Type>::lower( void ) const
{
  return( Lower );
}

template <typename Type>
inline Type const &Range<Type>::upper( void ) const
{
  return( Upper );
}

template <typename Type>
inline Type const &Range<Type>::lower( Type const &Lower )
{
  return( Range::Lower = Lower );
}

template <typename Type>
inline Type const &Range<Type>::upper( Type const &Upper )
{
  return( Range::Upper = Upper );
}

//! Returns \c true if \a Val is within [\a Lower ... \a Higher], inclusively.
template <typename Type>
inline bool Range<Type>::contains( Type const &Val ) const
{
  return( (Val >= Lower) && (Val <= Upper) );
}

//! If \a Val is outside of the range, sets it such that it is within the range,
//! and returns \c true.  Returns \c false, otherwise.
template <typename Type>
inline bool Range<Type>::clamp( Type &Val ) const
{
  if ( Val < Lower )
    Val = Lower;
  else
  if ( Val > Upper )
    Val = Upper;
  else
    return false;

  return true;
}

template <typename Type>
inline Type Range<Type>::height( void ) const
{
  if ( std::numeric_limits<Type>::is_integer )
    return( Upper - Lower + 1 );
  else
    return( Upper - Lower );
}


}

#endif
