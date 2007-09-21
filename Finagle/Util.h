/*!
** \file Util.h
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

#ifndef FINAGLE_UTIL_H
#define FINAGLE_UTIL_H

#include <limits>
#include <cstdlib>
#include <Finagle/File.h>
#include <Finagle/Range.h>

namespace Finagle {

FilePath execFile( pid_t pid = getpid() );
String execTitle( pid_t pid = getpid() );

void sleep( double Secs );

double drand( void );

template <typename CType>
typename CType::value_type pop_front( CType &container );

template <typename CType>
typename CType::value_type pop_back( CType &container );

template <typename Type>
Type absVal( Type const &Val );

// TEMPLATE IMPLEMENTATION ****************************************************

//! Returns a random number in the range [0, 1).
inline double drand( void )
{
  return ((double) ::rand() / RAND_MAX);
}


/*! \brief Pops the front-most item from \a Container and returns it.
** Uses \c front() and \c erase(). If the container is empty, returns an empty value.
*/
template <typename CType>
typename CType::value_type pop_front( CType &container )
{
  if ( container.empty() ) {
    typename CType::value_type dummy;
    return dummy;
  }

  typename CType::value_type item = container.front();
  container.erase( container.begin() );
  return item;
}

/*! \brief Pops the back-most item from \a container and returns it.
** Uses \c back() and \c erase(). If the container is empty, returns an empty value.
*/
template <typename CType>
typename CType::value_type pop_back( CType &container )
{
  if ( container.empty() ) {
    typename CType::value_type dummy;
    return dummy;
  }

  typename CType::value_type item = container.back();
  container.erase( container.end() - 1 );
  return item;
}

//! Returns the absolute value of \a Val.
template <typename Type>
Type absVal( Type const &Val )
{
  if ( !std::numeric_limits<Type>::is_signed )
    return Val;

  return (Val < 0) ? -Val : Val;
}

#ifdef min
#undef min
#endif

//! Returns the smaller of \a a and \a b.
template <typename Type>
inline Type const &min( Type const &a, Type const &b )
{
  return (a <= b) ? a : b;
}

#ifdef max
#undef max
#endif

//! Returns the larger of \a a and \a b.
template <typename Type>
inline Type const &max( Type const &a, Type const &b )
{
  return (a >= b) ? a : b;
}

/*! \brief Clamps \a Val to within \a Min and \a Max.
**
** If \a Val is within the range [\a Min .. \a Max], returns \a Val.
** Otherwise, returns the number within the range nearest to \a Val.
*/
template <typename Type>
inline Type clamp( Type val, Type const &min, Type const &max )
{
  if ( val < min )
    return val = min;

  if ( val > max )
    return val = max;

  return val;
}

//! Returns the square of \a val (i.e. \a val * \a val).
template <typename Type>
inline Type sqr( Type const &val )
{
  return val * val;
}

//! Normalize a numeric type to a double in the range 0..1
template <typename Type>
inline double normalize( Type const &val )
{
  return std::numeric_limits<Type>::is_integer
    ? (double(val) / double(std::numeric_limits<Type>::max()))
    : double(val);
}


//! Expand a double in the range 0..1 to the range of any numeric type.
template <typename Type>
inline Type unnormalize( double const &norm )
{
  return std::numeric_limits<Type>::is_integer
    ? Type( norm * std::numeric_limits<Type>::max() )
    : Type( norm );
}


}

#endif
