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


//! Initializes the date from \a Year, \a Month, and \a Day, and initializes
//! the time from \a Hour, \a Min, and \a Sec.
DateTime::DateTime( unsigned Year, unsigned Month, unsigned Day,
                    unsigned Hour, unsigned Min, unsigned Sec )
{
  LocalTime.tm_year = Year - 1900;
  LocalTime.tm_mon = Month - 1;
  LocalTime.tm_mday = Day;
  LocalTime.tm_hour = Hour;
  LocalTime.tm_min = Min;
  LocalTime.tm_sec = Sec;
  LocalTime.tm_isdst = -1;

  Time = (*MkTimePtr)( &LocalTime );
  if ( Time == (time_t) -1 ) {
    Time = 0;
    HaveLocal = false;
  } else
    HaveLocal = true;
}


/*!
** Attempts to parse the string \a Str as a time/date string.  If successful, returns \c true.
**
** \note Currently only supports strings of \c [DATE][TIME], where date is of the format \c YYYYMMDD, and time is of the form \c HHMMSS.
*/
bool DateTime::parse( String Str )
{
  unsigned Year = 0, Mon = 0, Day = 0;
  if ( Str.length() >= 8 ) {
    if ( !Str.substr( 0, 4 ).to( Year ) || !Str.substr( 4, 2 ).to( Mon ) || !Str.substr( 6, 2 ).to( Day ) )
      return false;

    Str = Str.substr( 8 );

    if ( Str.empty() ) {
      *this = DateTime( Year, Mon, Day );
      return true;
    }
  }

  unsigned Hour = 0, Min = 0, Sec = 0;
  if ( Str.length() == 6 ) {
    if ( !Str.substr( 0, 2 ).to( Hour ) || !Str.substr( 2, 2 ).to( Min ) || !Str.substr( 4, 2 ).to( Sec ) )
      return false;

    Str = Str.substr( 6 );

    if ( Str.empty() ) {
      *this = DateTime( Year, Mon, Day, Hour, Min, Sec );
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
    return( String( Buff ) );

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

// Keep the numbers small, count from app startup
//const unsigned long Time::_start = time(0);

//! Returns the current time.
Time const Time::now( void )
{
  struct timeval curTime;
  gettimeofday( &curTime, 0 );
  return Time(curTime);
}
