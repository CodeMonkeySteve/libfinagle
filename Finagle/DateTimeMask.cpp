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

/*! \class Finagle::DateTimeMask
** \brief Specifies a set of dates/times by masking datetime fields
*/

const char *DateTimeMask::MonthNames[] = {
  "jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec",
};
const char *DateTimeMask::DayNames[] = {
  "sun", "mon", "tue", "wed", "thu", "fri", "sat",
};


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
      _month = (m - MonthNames) + 1;
      continue;
    }

    const char **DayEnd = &DayNames[sizeof( DayNames ) / sizeof( DayNames[0] )];
    const char **d = find( DayNames, DayEnd, *t );
    if ( d != DayEnd ) {
      _wday = (d - DayNames) + 1;
      continue;
    }

    if ( YearMatch( *t ) ) {
      if ( !YearMatch[1].to( _year ) )
        throw BadDateEx( YearMatch[1] + " is not a valid year" );
      _year -= 2000;
      continue;
    }

    if ( DayMatch( *t ) ) {
      if ( !DayMatch[1].to( _day ) || (_day > 31) )
        throw BadDateEx( DayMatch[1] + " is not a valid day" );
      continue;
    }

    bool HaveHour = HourMatch( *t ), HaveMin = MinMatch( *t );
    if ( HaveHour || HaveMin ) {
      if ( HaveHour ) {
        if ( !HourMatch[1].to( _hour ) || (_hour > 23) )
          throw BadDateEx( HourMatch[1] + " is not a valid hour" );
        _hour++;
      }

      if ( HaveMin ) {
        if ( !MinMatch[1].to( _min ) || (_min > 59) )
          throw BadDateEx( MinMatch[1] + " is not a valid minute" );
        _min++;
      }

      continue;
    }

    throw BadDateEx( "Unknown term: " + *t );
  }
}


DateTimeMask::operator Finagle::String( void ) const
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


bool DateTimeMask::operator()( DateTime const &time ) const
{
  return( (   !yearValid() || (time.year()    == year())) &&
          (  !monthValid() || (time.month()   == month())) &&
          (!weekDayValid() || (time.weekDay() == weekDay())) &&
          (    !dayValid() || (time.day()     == day())) &&
          (   !hourValid() || (time.hour()    == hour())) &&
          ( !minuteValid() || (time.minute()  == minute())) );
}


//! \brief Returns the most recent date/time before \a time that matches this mask.
DateTime DateTimeMask::prev( DateTime time ) const
{
  if ( yearValid() && (year() != time.year()) )
    return (time.year() > year()) ? prev( DateTime( year(), 12, 31, 23, 59 ) ) : DateTime();

  if ( monthValid() && (month() != time.month())) {
    if ( time.month() > month() )
      return prev( DateTime( time.year(), month(), 31, 23, 59 ) );

    return prev( DateTime(  time.year() - 1, 12, 31, 23, 59 ) );
  }

  if ( dayValid() && (day() != time.day() ) ) {
    if ( time.day() > day() )
      return prev( DateTime( time.year(), time.month(), day(), 23, 59 ) );

    if ( time.month() == 1 )
      return prev( DateTime( time.year() - 1, 12, day(), 23, 59 ) );

    return prev( DateTime( time.year(), time.month() - 1, day(), 23, 59 ) );
  }

  if ( weekDayValid() && (weekDay() != time.weekDay() ) ) {
    if ( time.weekDay() > weekDay() )
      time -= (time.weekDay() - weekDay()) * (24 * 60 * 60);
    else
      time -= (7 - (weekDay() - time.weekDay())) * (24 * 60 * 60);

    return prev( DateTime( time.year(), time.month(), time.day(), 23, 59 ) );
  }

  if ( hourValid() && (hour() != time.hour()) ) {
    if ( time.hour() < hour() )
      time -= (24 * 60 * 60);

    return prev( DateTime( time.year(), time.month(), time.day(), hour(), 59 ) );
  }

  if ( minuteValid() && (minute() != time.minute())) {
    if ( time.minute() > minute() )
      return DateTime( time.year(), time.month(), time.day(), time.hour(), minute() );

    time -= (60 * 60);
    return prev( DateTime( time.year(), time.month(), time.day(), time.hour(), minute() ) );
  }

  return time;
}


//! \brief Returns the next date/time on or after \a time that matches this mask.
DateTime DateTimeMask::next( DateTime time ) const
{
  if ( yearValid() && (year() != time.year()) )
    return (time.year() < year()) ? next( DateTime( year() ) ) : DateTime();

  if ( monthValid() && (month() != time.month())) {
    if ( time.month() < month() )
      return next( DateTime( time.year(), month(), 1 ) );

    return next( DateTime( time.year() + 1, 1, 1 ) );
  }

  if ( dayValid() && (day() != time.day() ) ) {
    if ( time.day() < day() )
      return next( DateTime( time.year(), time.month(), day() ) );

    if ( time.month() == 12 )
      return next( DateTime( time.year() + 1, 1, day() ) );

    return next( DateTime( time.year(), time.month() + 1, day() ) );
  }

  if ( weekDayValid() && (weekDay() != time.weekDay() ) ) {
    if ( time.weekDay() < weekDay() )
      time += (weekDay() - time.weekDay()) * (24 * 60 * 60);
    else
      time += (7 - (time.weekDay() - weekDay())) * (24 * 60 * 60);

    return next( DateTime( time.year(), time.month(), time.day() ) );
  }

  if ( hourValid() && (hour() != time.hour()) ) {
    if ( time.hour() > hour() )
      time += (24 * 60 * 60);

    return next( DateTime( time.year(), time.month(), time.day(), hour() ) );
  }

  if ( minuteValid() && (minute() != time.minute())) {
    if ( time.minute() < minute() )
      return DateTime( time.year(), time.month(), time.day(), time.hour(), minute() );

    time += (60 * 60);
    return next( DateTime( time.year(), time.month(), time.day(), time.hour(), minute() ) );
  }

  return time;
}
