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
    BadDateEx( String const &Str );
  };

  static const char *MonthNames[];
  static const char *DayNames[];

public:
  DateTimeMask( void );
  DateTimeMask( DateTime const &When );
  DateTimeMask( String const &MaskStr );

  void parse( String const &MaskStr );
  bool empty( void ) const;

  bool operator()( DateTime const &When ) const;
  operator Finagle::String( void ) const;

  bool yearValid( void ) const    {  return( Year != 0 );  }
  bool monthValid( void ) const   {  return( Mon  != 0 );  }
  bool dayValid( void ) const     {  return( Day  != 0 );  }
  bool weekDayValid( void ) const {  return( WDay != 0 );  }
  bool hourValid( void ) const    {  return( Hour != 0 );  }
  bool minuteValid( void ) const  {  return( Min  != 0 );  }

  unsigned year( void ) const    {  return( Year + 2000 );  }
  unsigned month( void ) const   {  return( Mon );          }
  unsigned day( void ) const     {  return( Day );          }
  unsigned weekDay( void ) const {  return( WDay - 1 );     }
  unsigned hour( void ) const    {  return( Hour - 1 );     }
  unsigned minute( void ) const  {  return( Min - 1 );      }

  DateTime prev( DateTime When ) const;
  DateTime next( DateTime When ) const;

protected:
  unsigned char Year;
  unsigned char Mon;
  unsigned char Day;
  unsigned char WDay;
  unsigned char Hour;
  unsigned char Min;
};

// INLINE IMPLEMENTATION ******************************************************

inline DateTimeMask::BadDateEx::BadDateEx( String const &Str )
: Exception( "Bad date specification: " + Str )
{}

inline DateTimeMask::DateTimeMask( void )
: Year( 0 ), Mon( 0 ), Day( 0 ), WDay( 0 ), Hour( 0 ), Min( 0 )
{
}

inline DateTimeMask::DateTimeMask( String const &MaskStr )
: Year( 0 ), Mon( 0 ), Day( 0 ), WDay( 0 ), Hour( 0 ), Min( 0 )
{
  parse( MaskStr );
}

inline bool DateTimeMask::empty( void ) const
{
  return( !yearValid() && !monthValid() && !dayValid() && !weekDayValid() &&
          !hourValid() && !minuteValid() );
}

}

#endif
