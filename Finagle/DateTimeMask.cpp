/*!
** \file DateTimeMask.cpp
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

#include <algorithm>
#include "DateTimeMask.h"
#include "RegEx.h"

using namespace std;
using namespace Finagle;

const char *DateTimeMask::MonthNames[] = {
  "jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec",
};
const char *DateTimeMask::DayNames[] = {
  "sun", "mon", "tue", "wed", "thu", "fri", "sat",
};


DateTimeMask::DateTimeMask( DateTime const &When )
: Year( When.year() - 2000 ), Mon( When.month() ), Day( When.day() ), WDay( 0 ),
  Hour( When.hour() + 1 ), Min( When.minute() + 1 )
{
}


void DateTimeMask::parse( Finagle::String const &MaskStr )
{
  static RegEx
    YearMatch( "^(2\\d{3})$" ),
    DayMatch( "^(\\d{2})$" ),
    HourMatch( "^(\\d{1,2}):" ),
    MinMatch( ":(\\d{2})$" );

  String::Array Tokens( String::toLower( MaskStr ).split() );
  for ( String::Array::Iterator t = Tokens.begin(); t != Tokens.end(); ++t ) {
    const char **MonthEnd = &MonthNames[sizeof( MonthNames ) / sizeof( MonthNames[0] )];
    const char **m = find( MonthNames, MonthEnd, *t );
    if ( m != MonthEnd ) {
      Mon = (m - MonthNames) + 1;
      continue;
    }

    const char **DayEnd = &DayNames[sizeof( DayNames ) / sizeof( DayNames[0] )];
    const char **d = find( DayNames, DayEnd, *t );
    if ( d != DayEnd ) {
      WDay = (d - DayNames) + 1;
      continue;
    }

    if ( YearMatch( *t ) ) {
      if ( !YearMatch[1].to( Year ) )
        throw BadDateEx( YearMatch[1] + " is not a valid year" );
      Year -= 2000;
      continue;
    }

    if ( DayMatch( *t ) ) {
      if ( !DayMatch[1].to( Day ) || (Day > 31) )
        throw BadDateEx( DayMatch[1] + " is not a valid day" );
      continue;
    }

    bool HaveHour = HourMatch( *t ), HaveMin = MinMatch( *t );
    if ( HaveHour || HaveMin ) {
      if ( HaveHour ) {
        if ( !HourMatch[1].to( Hour ) || (Hour > 23) )
          throw BadDateEx( HourMatch[1] + " is not a valid hour" );
        Hour++;
      }

      if ( HaveMin ) {
        if ( !MinMatch[1].to( Min ) || (Min > 59) )
          throw BadDateEx( MinMatch[1] + " is not a valid minute" );
        Min++;
      }

      continue;
    }

    throw BadDateEx( "Unknown term: " + *t );
  }
}


DateTimeMask::operator String( void ) const
{
  String::Array s;

  if ( yearValid() )    s.push_back( String( year() ) );
  if ( monthValid() )   s.push_back( MonthNames[month() - 1] );
  if ( dayValid() )     s.push_back( String( day() ) );
  if ( weekDayValid() ) s.push_back( DayNames[weekDay()] );

  if ( hourValid() && minuteValid() )
    s.push_back( String( hour() ) + ":" + String( minute() ) );
  else
  if ( hourValid() )
    s.push_back( String( hour() ) + ":" );
  else
  if ( minuteValid() )
    s.push_back( ":" + String( minute() ) );

  return String::join(s, ' ');
}


bool DateTimeMask::operator()( DateTime const &When ) const
{
  return( (   !yearValid() || (When.year()    == year())) &&
          (  !monthValid() || (When.month()   == month())) &&
          (!weekDayValid() || (When.weekDay() == weekDay())) &&
          (    !dayValid() || (When.day()     == day())) &&
          (   !hourValid() || (When.hour()    == hour())) &&
          ( !minuteValid() || (When.minute()  == minute())) );
}


//! \brief Returns the most recent date/time before \a When that matches this mask.
DateTime DateTimeMask::prev( DateTime When ) const
{
  if ( yearValid() && (year() != When.year()) )
    return (When.year() > year()) ? prev( DateTime( year(), 12, 31, 23, 59 ) ) : DateTime();

  if ( monthValid() && (month() != When.month())) {
    if ( When.month() > month() )
      return prev( DateTime( When.year(), month(), 31, 23, 59 ) );

    return prev( DateTime(  When.year() - 1, 12, 31, 23, 59 ) );
  }

  if ( dayValid() && (day() != When.day() ) ) {
    if ( When.day() > day() )
      return prev( DateTime( When.year(), When.month(), day(), 23, 59 ) );

    if ( When.month() == 1 )
      return prev( DateTime( When.year() - 1, 12, day(), 23, 59 ) );

    return prev( DateTime( When.year(), When.month() - 1, day(), 23, 59 ) );
  }

  if ( weekDayValid() && (weekDay() != When.weekDay() ) ) {
    if ( When.weekDay() > weekDay() )
      When -= (When.weekDay() - weekDay()) * (24 * 60 * 60);
    else
      When -= (7 - (weekDay() - When.weekDay())) * (24 * 60 * 60);

    return prev( DateTime( When.year(), When.month(), When.day(), 23, 59 ) );
  }

  if ( hourValid() && (hour() != When.hour()) ) {
    if ( When.hour() < hour() )
      When -= (24 * 60 * 60);

    return prev( DateTime( When.year(), When.month(), When.day(), hour(), 59 ) );
  }

  if ( minuteValid() && (minute() != When.minute())) {
    if ( When.minute() > minute() )
      return DateTime( When.year(), When.month(), When.day(), When.hour(), minute() );

    When -= (60 * 60);
    return prev( DateTime( When.year(), When.month(), When.day(), When.hour(), minute() ) );
  }

  return When;
}


//! \brief Returns the next date/time on or after \a When that matches this mask.
DateTime DateTimeMask::next( DateTime When ) const
{
  if ( yearValid() && (year() != When.year()) )
    return (When.year() < year()) ? next( DateTime( year() ) ) : DateTime();

  if ( monthValid() && (month() != When.month())) {
    if ( When.month() < month() )
      return next( DateTime( When.year(), month(), 1 ) );

    return next( DateTime( When.year() + 1, 1, 1 ) );
  }

  if ( dayValid() && (day() != When.day() ) ) {
    if ( When.day() < day() )
      return next( DateTime( When.year(), When.month(), day() ) );

    if ( When.month() == 12 )
      return next( DateTime( When.year() + 1, 1, day() ) );

    return next( DateTime( When.year(), When.month() + 1, day() ) );
  }

  if ( weekDayValid() && (weekDay() != When.weekDay() ) ) {
    if ( When.weekDay() < weekDay() )
      When += (weekDay() - When.weekDay()) * (24 * 60 * 60);
    else
      When += (7 - (When.weekDay() - weekDay())) * (24 * 60 * 60);

    return next( DateTime( When.year(), When.month(), When.day() ) );
  }

  if ( hourValid() && (hour() != When.hour()) ) {
    if ( When.hour() > hour() )
      When += (24 * 60 * 60);

    return next( DateTime( When.year(), When.month(), When.day(), hour() ) );
  }

  if ( minuteValid() && (minute() != When.minute())) {
    if ( When.minute() < minute() )
      return DateTime( When.year(), When.month(), When.day(), When.hour(), minute() );

    When += (60 * 60);
    return next( DateTime( When.year(), When.month(), When.day(), When.hour(), minute() ) );
  }

  return When;
}
