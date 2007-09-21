/*!
** \file Rate.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Thu Jan 29 2004
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

#include "Rate.h"

using namespace Finagle;

/*!
** \class Finagle::Rate
** \brief This class computes the average rate of occurance for some periodic event.
**
** This class is intended for measuring (and/or limiting), e.g. data
** transmission (sending a byte), rendering (drawing a frame), or anything
** else that happens periodically.
*/

/*! \fn Rate::operator()()
** Returns the average rate of event occurance, in events per second.  Returns
** 0.0 If no events have yet occured (i.e. since the object was instantiated or
** #reset() was last called).
*/


/*!
** Resets the rate counter.  Until an event next occurs #average() will return
** 0.0 .
*/
void Rate::reset( void )
{
  Count = 0;
}


/*!
** If a rate limit of \a RateLimit is to be imposed, this returns the minimum
** time until the next event can occur.  Apps that are trying to throttle, e.g.
** upstream bandwidth, should delay (perhaps #sleep()) for the duration of the
** return value, before sending another byte.  Will return 0.0 if the current
** average rate is within the limit.
**
*/
double Rate::throttleTime( double RateLimit ) const
{
  if ( (*this)() <= RateLimit )
    return 0.0;

  return ((Count + 1) / RateLimit) + Start - (const double &) Time::now();
}







