/*!
** \file TextString.cpp
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

#include <algorithm>
#include <locale>

#include "TextString.h"
#include "Exception.h"
#include "MemTrace.h"

using namespace std;
using namespace Finagle;

const String String::nil;

class String::ConversionEx : public Exception {
public:
  ConversionEx( String const &src, std::type_info const &dest )
  : Exception( "Unable to convert \"" + src + "\" to type \"" + dest.name() + "\"" ) {}
};

void String::throwConversionEx( String const &src, std::type_info const &dest ) const
{
  throw ConversionEx( src, dest );
}

// These are necessary, as std::tolower()/::toupper() are ambiguous.
static char ToLower( char Ch ) {  return std::tolower( Ch, locale::classic() );  }
static char ToUpper( char Ch ) {  return std::toupper( Ch, locale::classic() );  }

/*! \class Finagle::String
** \brief An extended version of the standard string class.
**
** This class inherits std::string, and ads a few additional functions, as well
** some sring-related global functions.
*/

//! Changes all characters of the string to lowercase form.
String &String::makeLower( void )
{
  transform( begin(), end(), begin(), ToLower );
  return *this;
}

//! Changes all characters of the string to uppercase form.
String &String::makeUpper( void )
{
  transform( begin(), end(), begin(), ToUpper );
  return *this;
}

//! Splits the string into words, each divided by a character in \a Sep.
//! Words are returned as a String Array.
String::Array String::split( String const &sep ) const
{
  const size_type len = length();
  String::Array words;

  // Tokenize the input string
  // Note: there may be an easier way of doing this using istringstream and
  // getline().
  size_type start = 0, end;
  while ( true ) {
    end = find_first_of( sep, start );
    if ( end == npos ) {
      // Last word
      words.push_back( substr( start ) );
      break;
    }

    words.push_back( substr( start, end - start ) );

    start = end + 1;
    if ( start == len ) {
      // Empty after seperator
      words.push_back( String() );
      break;
    }
  }

  return words;
}


//! Removes any leading and trailing occurances of \a Chars from the string.
String &String::trim( const char *Chars )
{
  const size_type i = find_first_not_of( Chars );
  if ( i == npos ) {
    clear();
    return *this;
  }

  size_type j = find_last_not_of( Chars );
  if ( j != npos )
    j = j - i + 1;

  return *this = substr( i, j );
}


/*! \brief Formats a string in the style of sprintf().
**
** Returns a string based on the \a form string, and a variable number of
** arguments.
*/
String String::format( const char *form, ... )
{
  char buff[512];
  va_list args;
  va_start( args, form );

  if ( vsnprintf( buff, sizeof(buff) - 1, form, args ) != -1 )
    return buff;

  // Too big for static buffer -- dynamically allocate a buffer.
  unsigned len = sizeof(buff);
  char *dynBuff = 0;

  do {
    delete [] dynBuff;
    len *= 2;
    dynBuff = new char[len];
  } while ( vsnprintf( dynBuff, len - 1, form, args ) == -1 );

  String str( dynBuff );
  delete dynBuff;
  va_end( args );

  return str;
}


/*!
** Template partial-instantiation for conversion of boolean values.  Boolean
** strings can contain "true"/"false", "yes"/"no", "on"/"off", or "1"/"0"
** case insensitive).
*/
template <>
bool String::to<bool>( bool &val ) const
{
  NoCase str( *this );

  if ( (str == "false") || (str == "no") || (str == "off") || (str == "0") ) {
    val = false;
    return true;
  }

  if ( (str == "true") || (str == "yes") || (str == "on") || (str == "1") ) {
    val = true;
    return true;
  }

  return false;
}

// NO-CASE STRING *************************************************************

#include <cstring>

/*! \class Finagle::NoCase
** This is a modified form of std::basic_string, with character traits that are
** case insensitive.  Intended to be used for case-insensitive string comparisions.
*/

bool NoCaseChar::eq( char c1, char c2 )
{
  return tolower( c1 ) == tolower( c2 );
}

bool NoCaseChar::ne( char c1, char c2 )
{
  return !eq( c1, c2 );
}

bool NoCaseChar::lt( char c1, char c2 )
{
  return tolower( c1 ) < tolower( c2 );
}

int NoCaseChar::compare( const char* s1, const char* s2, size_t n )
{
  return strncasecmp( s1, s2, n );

}

const char *NoCaseChar::find( const char* s, int n, char a )
{
  while ( n-- && (tolower( *s ) != tolower( a )) )
    s++;

  return( s );
}
