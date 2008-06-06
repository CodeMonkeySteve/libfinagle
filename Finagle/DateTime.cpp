/*!
** \file DateTime.cpp
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

#include <sys/time.h>

#include "DateTime.h"
#include "AppLog.h"
#include "MemTrace.h"

using namespace std;
using namespace Finagle;

/*! \class Finagle::DateTime
** \brief Represents a date and time of day.
**
** Time is assumed to be within the current timezone, and is limited to second
** resolution.
*/

DateTime::TimeFunc      DateTime::GMTimePtr =    (DateTime::TimeFunc)     gmtime;
DateTime::TimeFunc      DateTime::LocalTimePtr = (DateTime::TimeFunc)     localtime;
DateTime::MkTimeFunc    DateTime::MkTimePtr =    (DateTime::MkTimeFunc)   mktime;
DateTime::StrFTimeFunc  DateTime::StrFTimePtr =  (DateTime::StrFTimeFunc) strftime;


//! Initializes the date from \a year, \a month, and \a day, and initializes
//! the time from \a hour, \a min, and \a sec.
DateTime::DateTime( unsigned year, unsigned month, unsigned day, unsigned hour, unsigned min, unsigned sec )
{
  _localTime.tm_year = year - 1900;
  _localTime.tm_mon = month - 1;
  _localTime.tm_mday = day;
  _localTime.tm_hour = hour;
  _localTime.tm_min = min;
  _localTime.tm_sec = sec;
  _localTime.tm_isdst = -1;

  _time = (*MkTimePtr)( &_localTime );
  if ( _time == (time_t) -1 ) {
    _time = 0;
    _haveLocal = false;
  } else
    _haveLocal = true;
}


/*!
** Attempts to parse the string \a Str as a time/date string.  If successful, returns \c true.
**
** \note Currently only supports strings of \c [DATE][TIME], where date is of the format \c YYYYMMDD, and time is of the form \c HHMMSS.
*/
bool DateTime::parse( String Str )
{
  unsigned year = 0, Mon = 0, day = 0;
  if ( Str.length() >= 8 ) {
    if ( !Str.substr( 0, 4 ).to( year ) || !Str.substr( 4, 2 ).to( Mon ) || !Str.substr( 6, 2 ).to( day ) )
      return false;

    Str = Str.substr( 8 );

    if ( Str.empty() ) {
      *this = DateTime( year, Mon, day );
      return true;
    }
  }

  unsigned hour = 0, min = 0, sec = 0;
  if ( Str.length() == 6 ) {
    if ( !Str.substr( 0, 2 ).to( hour ) || !Str.substr( 2, 2 ).to( min ) || !Str.substr( 4, 2 ).to( sec ) )
      return false;

    Str = Str.substr( 6 );

    if ( Str.empty() ) {
      *this = DateTime( year, Mon, day, hour, min, sec );
      return true ;
    }
  }

  return false;
}


//! Returns a string representation of the current date/time, in same form as \c strftime().
String DateTime::format( const char *Format ) const
{
  char Buff[512];
  tm LT = local();

  if ( (*StrFTimePtr)( Buff, sizeof( Buff ) - 1, Format, &LT ) != 0 )
    return String( Buff );

  // Too big for static buffer -- dynamically allocate a buffer.
  unsigned Len = sizeof( Buff );
  char *BuffPtr = 0;

  do {
    delete [] BuffPtr;
    Len *= 2;
    BuffPtr = new char[Len];
  } while ( !(*StrFTimePtr)( Buff, sizeof( Buff ) - 1, Format, &LT ) );

  String Str( BuffPtr );
  delete BuffPtr;

  return Str;
}


/*! \class Finagle::Time
** \brief Represents a time with fractional seconds.
*/

//! Returns the current time.
Time const Time::now( void )
{
  struct timeval curTime;
  gettimeofday( &curTime, 0 );
  return Time(curTime);
}
