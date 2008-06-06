/*!
** \file DateTime.h
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

#ifndef FINAGLE_DATETIME_H
#define FINAGLE_DATETIME_H

#include <ostream>
#include <ctime>
#include <cmath>

#include <Finagle/TextString.h>

namespace Finagle {

class DateTime {
public:
  typedef struct tm *(*TimeFunc)(  const time_t * const );
  typedef time_t (*MkTimeFunc)( struct tm * const );
  typedef size_t (*StrFTimeFunc)( char * const, const size_t, const char * const, const struct tm * const );

  static TimeFunc     GMTimePtr;
  static TimeFunc     LocalTimePtr;
  static MkTimeFunc   MkTimePtr;
  static StrFTimeFunc StrFTimePtr;

public:
  static DateTime const now( void );
  static bool setSysClock( DateTime const &time );

public:
  DateTime( void );
  DateTime( time_t const &time );
  DateTime( unsigned year, unsigned month, unsigned day, unsigned hour = 0, unsigned min = 0, unsigned sec = 0 );

  bool parse( String str );
  operator bool( void ) const;

  time_t calTime( void ) const;
  const tm &local( void ) const;
        tm  UTC( void ) const;

  bool     DST( void ) const;     // In Daylight Savings Time?
  unsigned year( void ) const;    // 2004-?
  unsigned month( void ) const;   // 1-12
  unsigned day( void ) const;     // 1-31
  unsigned hour( void ) const;    // 0-23
  unsigned minute( void ) const;  // 0-59
  unsigned second( void ) const;  // 0-59

  unsigned weekDay( void ) const; // 0-6  (0 = sunday)
  unsigned absHours( void ) const;

  String format( const char *format = "%Y/%m/%d %H:%M:%S %Z" ) const;

  bool operator <( const DateTime &that ) const;
  bool operator <=( const DateTime &that ) const;
  bool operator >( const DateTime &that ) const;
  bool operator >=( const DateTime &that ) const;
  bool operator ==( const DateTime &that ) const;
  bool operator !=( const DateTime &that ) const;

  DateTime &operator +=( const time_t &that );
  DateTime &operator -=( const time_t &that );
  DateTime  operator +( const time_t &that ) const;
  DateTime  operator -( const time_t &that ) const;

  long operator -( const DateTime &that ) const;

  friend std::istream &operator >>( std::istream &in, DateTime &time );
  friend std::ostream &operator <<( std::ostream &out, DateTime const &time );

protected:
  time_t _time;

  mutable bool _haveLocal;
  mutable tm _localTime;
};


class Time {
public:
  Time( double time = 0.0 );
  Time( unsigned hour, unsigned min, double sec = 0.0 );
  Time( DateTime const &time );
  Time( timeval const &tv );

  static Time const now( void );

  bool isValid( void ) const;

  double hour( void ) const;
  double minute( void ) const;
  double sec( void ) const;
  long   msecs( void ) const;
  long   usecs( void ) const;

  const double &operator()( void ) const;
  operator const double &( void ) const;

  double &operator()( void );
  operator double &( void );

protected:
  double _secs;
};

// INLINE IMPLEMENTATION ******************************************************

//! Initializes the date and time to "invalid".
//! \sa isValid().
inline DateTime::DateTime( void )
: _time(0), _haveLocal(false)
{}

//! Initializes the date and time from a standard \c time_t value.
//! \sa ctime(3).
inline DateTime::DateTime( time_t const &time )
: _time(time), _haveLocal(false)
{}

//! Returns the current date/time.
inline DateTime const DateTime::now( void )
{
  return DateTime( ::time(0) );
}

//! Returns \c true if the date/time holds a valid value.
inline DateTime::operator bool( void ) const
{
  return _time != 0;
}

//! Returns the date/time as a standard \c time_t value.
//! \sa ctime(3).
inline time_t DateTime::calTime( void ) const
{
  return _time;
}

//! Returns the date/time as a standard \c tm structure.
//! \sa ctime(3).
inline const tm &DateTime::local( void ) const
{
  if ( !_haveLocal ) {
    _localTime = *(*LocalTimePtr)( &_time );
    _haveLocal = true;
  }

  return _localTime;
}

//! Returns the date/time as a standard \c tm structure, relative to UTC.
inline tm DateTime::UTC( void ) const
{
  return *(*GMTimePtr)( &_time );
}

inline bool DateTime::DST( void ) const
{
  return local().tm_isdst != 0;
}

//! Returns the year, with century.
inline unsigned DateTime::year( void ) const
{
  return local().tm_year + 1900;
}

//! Returns the month (e.g. \c 1..12).
//! \note This differs from \c localtime(3), which is 0-based.
inline unsigned DateTime::month( void ) const
{
  return local().tm_mon + 1;
}

//! Returns the day of the month (e.g. \c 1..31)
inline unsigned DateTime::day( void ) const
{
  return local().tm_mday;
}

//! Returns the hour of the day (e.g. \c 0..23)
inline unsigned DateTime::hour( void ) const
{
  return local().tm_hour;
}

//! Returns the minute of the hour (e.g. \c 0..59)
inline unsigned DateTime::minute( void ) const
{
  return local().tm_min;
}

//! Returns the second of the minute (e.g. \c 0..59)
inline unsigned DateTime::second( void ) const
{
  return local().tm_sec;
}

//! Returns the day of the week (e.g. \c 0..6), where \c 0 is Sunday.
inline unsigned DateTime::weekDay( void ) const
{
  return local().tm_wday;
}

//! Returns the number of hours since the epoch.
inline unsigned DateTime::absHours( void ) const
{
  return calTime() / 3600;
}

inline bool DateTime::operator <( const DateTime &that ) const
{
  return _time < that._time;
}

inline bool DateTime::operator <=( const DateTime &that ) const
{
  return _time <= that._time;
}

inline bool DateTime::operator >( const DateTime &that ) const
{
  return _time > that._time;
}

inline bool DateTime::operator >=( const DateTime &that ) const
{
  return _time >= that._time;
}

inline bool DateTime::operator ==( const DateTime &that ) const
{
  return _time == that._time;
}

inline bool DateTime::operator !=( const DateTime &that ) const
{
  return _time != that._time;
}

inline DateTime &DateTime::operator +=( const time_t &that )
{
  _time += that;
  _haveLocal = false;
  return *this;
}

inline DateTime &DateTime::operator -=( const time_t &that )
{
  _time -= that;
  _haveLocal = false;
  return *this;
}

inline DateTime DateTime::operator +( const time_t &that ) const
{
  return DateTime( _time + that );
}

inline DateTime DateTime::operator -( const time_t &that ) const
{
  return DateTime( _time - that );
}

inline long DateTime::operator -( const DateTime &that ) const
{
  return (long) _time - (long) that._time;
}

//! Reads the date/time, as a \c time_t value, from the \a in stream.
inline std::istream &operator >>( std::istream &in, DateTime &date )
{
  date._haveLocal = false;
  return in >> date._time;
}

//! Writes the date/time, as a \c time_t value, to the \a out stream.
inline std::ostream &operator <<( std::ostream &out, DateTime const &Date )
{
  return out << Date._time;
}


//! Initializes the time to a \a time second interval.  A zero-length interval is invalid.
//! \sa #isValid().
inline Time::Time( double time )
: _secs( time )
{}

inline Time::Time( unsigned hour, unsigned min, double sec )
: _secs( (hour * 3600) + (min * 60) + sec )
{}

//! Initializes the time value with the time-of-day from \a time.
inline Time::Time( DateTime const &time )
: _secs( (time.hour() * 3600) + (time.minute() * 60) + time.second() )
{}

//! Initializes the time value with the timeval \a tv.
inline Time::Time( timeval const &tv )
: _secs( (double) tv.tv_sec + ((double) tv.tv_usec / 1000000.0) )
{}

//! Returns \c true if the time value is valid (i.e. non-zero).
inline bool Time::isValid( void ) const
{
  return _secs != 0.0;
}

//! Returns the number of hours.
inline double Time::hour( void ) const
{
  return _secs / 3600.0;
}

//! Returns the number of minutes within the last hour.
inline double Time::minute( void ) const
{
  return fmod( _secs / 60.0, 60.0 );
}

//! Returns the number of minutes within the last minute.
inline double Time::sec( void ) const
{
  return fmod( _secs, 60.0 );
}

//! Returns the interval as an (integral) number of milliseconds.
inline long Time::msecs( void ) const
{
  return long( _secs * 1000.0 );
}

//! Returns the interval as an (integral) number of microseconds.
inline long Time::usecs( void ) const
{
  return long( _secs * 1000000 );
}

//! Returns the interval in seconds.
inline const double &Time::operator()( void ) const
{
  return _secs;
}

//! Returns the interval in seconds.
inline Time::operator const double &( void ) const
{
  return _secs;
}

//! Returns the interval in seconds.
inline double &Time::operator()( void )
{
  return _secs;
}

//! Returns the interval in seconds.
inline Time::operator double &( void )
{
  return _secs;
}

}

#endif
