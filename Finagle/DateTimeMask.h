/*!
** \file DateTimeMask.h
** \author Steve Sloan <steve@finagle.org>
** \date Thu Dec 16 2004
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

#ifndef FINAGLE_DATETIMEMASK_H
#define FINAGLE_DATETIMEMASK_H

#include <Finagle/DateTime.h>
#include <Finagle/Exception.h>

namespace Finagle {

class DateTimeMask : public DateTime {
public:
  struct BadDateEx : public Exception {
    BadDateEx( String const &str );
  };

  static const char *MonthNames[];
  static const char *DayNames[];

public:
  DateTimeMask( void );
  DateTimeMask( DateTime const &time );
  DateTimeMask( String const &maskStr );

  void parse( String const &maskStr );
  bool empty( void ) const;

  operator Finagle::String( void ) const;
  bool operator()( DateTime const &time ) const;

  bool yearValid( void ) const    {  return _year  != 0;  }
  bool monthValid( void ) const   {  return _month != 0;  }
  bool dayValid( void ) const     {  return _day   != 0;  }
  bool weekDayValid( void ) const {  return _wday  != 0;  }
  bool hourValid( void ) const    {  return _hour  != 0;  }
  bool minuteValid( void ) const  {  return _min   != 0;  }

  unsigned year( void ) const    {  return _year + 2000;  }
  unsigned month( void ) const   {  return _month;        }
  unsigned day( void ) const     {  return _day;          }
  unsigned weekDay( void ) const {  return _wday - 1;     }
  unsigned hour( void ) const    {  return _hour - 1;     }
  unsigned minute( void ) const  {  return _min - 1;      }

  DateTime prev( DateTime time ) const;
  DateTime next( DateTime time ) const;

protected:
  unsigned char _year;
  unsigned char _month;
  unsigned char _day;
  unsigned char _wday;
  unsigned char _hour;
  unsigned char _min;
};

// INLINE IMPLEMENTATION **********************************************************************************************************

inline DateTimeMask::BadDateEx::BadDateEx( String const &str )
: Exception( "Bad date specification: " + str )
{}

inline DateTimeMask::DateTimeMask( void )
: _year(0), _month(0), _day(0), _wday(0), _hour(0), _min(0)
{}

inline DateTimeMask::DateTimeMask( DateTime const &time )
: _year( time.year() - 2000 ), _month( time.month() ), _day( time.day() ), _wday( 0 ), _hour( time.hour() + 1 ),
  _min( time.minute() + 1 )
{}

inline DateTimeMask::DateTimeMask( String const &maskStr )
: _year(0), _month(0), _day(0), _wday(0), _hour(0), _min(0)
{
  parse( maskStr );
}

inline bool DateTimeMask::empty( void ) const
{
  return !yearValid() && !monthValid() && !dayValid() && !weekDayValid() && !hourValid() && !minuteValid();
}

}

#endif
