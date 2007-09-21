/*!
** \file Rate.h
** \author Steve Sloan <steve@finagle.org>
** \date Wed Jan 28 2004
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

#ifndef FINAGLE_RATE_H
#define FINAGLE_RATE_H

#include <Finagle/DateTime.h>

namespace Finagle {

class Rate {
public:
  Rate( void );

  Rate &operator ++( void );
  Rate &operator ++( int );
  Rate &operator +=( unsigned Counts );

  unsigned count( void ) const;
  double operator()( void ) const;
  double throttleTime( double RateLimit ) const;

  void reset( void );

protected: public:
  unsigned Count;
  Time     Start;
};

// INLINE IMPLEMENTATION ******************************************************

inline Rate::Rate( void )
: Count( 0 )
{
}

//! Signal that an event has occured.
inline Rate &Rate::operator ++( void )
{
  if ( !Count )  Start = Time::now();
  Count++;
  return *this;
}

//! Signal that an event has occured.
inline Rate &Rate::operator ++( int )
{
  if ( !Count )  Start = Time::now();
  Count++;
  return *this;
}

//! Signal that \a Counts events have occured.
inline Rate &Rate::operator +=( unsigned Counts )
{
  if ( !Count )  Start = Time::now();
  Count += Counts;
  return *this;
}


inline unsigned Rate::count( void ) const
{
  return Count;
}


inline double Rate::operator()( void ) const
{
//  return Count ? ((double) Count / (double) (Time::now() - Start)) : 0.0;
  return (double) Count / (double) (Time::now() - Start);
}


}

#endif
