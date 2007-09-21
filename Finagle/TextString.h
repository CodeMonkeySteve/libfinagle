/*!
** \file TextString.h
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

#ifndef FINAGLE_TEXTSTRING_H
#define FINAGLE_TEXTSTRING_H

#include <string>
#include <cstdarg>
#include <sstream>
#include <limits>

#include <Finagle/Array.h>
#include <Finagle/List.h>
#include <Finagle/Set.h>

namespace Finagle {

class String : public std::string {
public:
  typedef std::string::iterator Iterator;
  typedef std::string::const_iterator ConstIterator;

  typedef List<String> List;
  typedef Array<String> Array;
  typedef Set<String> Set;

  class ConversionEx;

public:
  String( void );
  String( char ch );
  String( const char *Str );
  String( const char *Str, unsigned Len );
  String( const std::string &Str, size_type Pos = 0, size_type Len = npos );
  String( String const &str, size_type Pos = 0, size_type Len = npos );
  String( unsigned Count, char Ch );
  String( unsigned num );
  String( int num );
  String( unsigned long num );
  String( long num );
  String( double num );

  String &operator=( const String &that );
  operator const char *( void ) const;
  operator bool( void ) const;

  static String hex( unsigned Num, unsigned Digits = 2 );

  String &makeLower( void );
  String &makeUpper( void );

  bool beginsWith( String const &substr ) const;
  bool endsWith( String const &substr ) const;

  Array split( String const &Sep = "\t " ) const;
  String &trim( const char *Chars = " \t\n\r" );

public: // Conversions
  template <typename Type>
  Type as( void ) const;
  template <typename Type>
  Type as( Type const &defVal ) const;

  template <typename Type>
  bool to( Type &dest ) const;
  template <typename Type>
  bool to( Type &dest, Type const &defVal ) const;

public: // Return-type wrappers
  String substr( size_type Pos = 0, size_type Len = npos ) const;
  reference operator()( size_type n );
  const_reference operator()( size_type n ) const;

public: // Class methods
  static String format( const char *form, ... );
  static String toLower( String const &str );
  static String toUpper( String const &str );
  static String trim( String const &str, const char *Chars = " \t\n\r" );

  template <typename Container>
  static String join( Container const &strs, String const &sep );

public:
  static const String nil;

private:
  void throwConversionEx( String const &src, std::type_info const &dest ) const;
};


struct NoCaseChar : public std::char_traits<char> {
  static bool eq( char c1, char c2 );
  static bool ne( char c1, char c2 );
  static bool lt( char c1, char c2 );
  static int  compare( const char* s1, const char* s2, size_t n );
  static const char *find( const char* s, int n, char a );
};

class NoCase : public std::basic_string<char, NoCaseChar> {
public:
  NoCase( void ) {}
  NoCase( const char *Str ) : std::basic_string<char, NoCaseChar>( Str ) {}
  NoCase( const char *Str, unsigned Len ) : std::basic_string<char, NoCaseChar>( Str, Len ) {}
  NoCase( String const &Str ) : std::basic_string<char, NoCaseChar>( Str ) {}

  operator const char *( void ) {  return c_str();  }
};

template <> bool String::to<bool>( bool &val ) const;
std::ostream &operator <<( std::ostream &out, NoCase const &Str );

// INLINE IMPLEMENTATION **********************************************************************************************************

//! Initializes the string to empty.
inline String::String( void )
{}

//! Initializes the string to a single character.
inline String::String( char ch )
: std::string( 1, ch )
{}

//! Initializes the string to \a Str (null-terminated).  If \a Str is 0,
//! initializes to the empty string.
inline String::String( const char *Str )
: std::string( Str ? Str : "" )
{}

//! Initializes the string to \a Str, with the first \a Len characters.
inline String::String( const char *Str, unsigned Len )
: std::string( Str, Len )
{}

//! Initializes the string to the substring of \a Str, with the \a Len characters
//! starting at \a Pos.  \a Pos and \a Len default to all of \a Str.
inline String::String( const std::string &Str, size_type Pos, size_type Len )
: std::string( Str, Pos, Len )
{}

//! Initializes the string to the substring of \a Str, with the \a Len characters
//! starting at \a Pos.  \a Pos and \a Len default to all of \a Str.
inline String::String( String const &str, size_type pos, size_type len )
: std::string( str, pos, len )
{}

//! Initializes the string with \a Count copies of \a CH.
inline String::String( unsigned Count, char Ch )
: std::string( Count, Ch )
{}

//! Initializes the string with the decimal representation of \a num.
//! \note This is contratry to the STL version of \c string(int).
inline String::String( unsigned num )
: std::string( String::format( "%u", num ) )
{}

//! Initializes the string with the decimal representation of \a num.
//! \note This is contratry to the STL version of \c string(int).
inline String::String( int num )
: std::string( String::format( "%d", num ) )
{}

//! Initializes the string with the decimal representation of \a num.
inline String::String( unsigned long num )
: std::string( String::format( "%lu", num ) )
{}

//! Initializes the string with the decimal representation of \a num.
inline String::String( long num )
: std::string( String::format( "%ld", num ) )
{}

//! Initializes the string with the decimal representation of \a num.
inline String::String( double num )
: std::string( String::format( "%f", num ) )
{}

inline String &String::operator=( const String &that )
{
  std::string::operator=( (const std::string &) that );
  return *this;
}

inline String::operator const char *( void ) const
{
  return c_str();
}

inline String::operator bool( void ) const
{
  return !empty();
}

//! Returns true iff the string contains \a substr at the beginning.
inline bool String::beginsWith( String const &substr ) const
{
  return String::substr( 0, substr.size() ) == substr;
}

//! Returns true iff the string contains \a substr at the end.
inline bool String::endsWith( String const &substr ) const
{
  return String::substr( size() - substr.size(), size() ) == substr;
}

//! Returns a string containing the integer \a num in hexidecimal form, padded
//! to \a digits length.
inline String String::hex( unsigned num, unsigned digits )
{
  return String::format( "0x%0*X", digits, num );
}

inline String String::substr( size_type Pos, size_type Len ) const
{
  return std::string::substr( Pos, Len );
}

//! Alternate index operator
inline String::reference String::operator()( size_type n )
{
  return std::string::operator[]( n );
}

//! Alternate index operator
inline String::const_reference String::operator()( size_type n ) const
{
  return std::string::operator[]( n );
}

inline String operator +( String const &Left, String const &Right )
{
  return String( Left ) += Right;
}

inline String operator +( const char *Left, String const &Right )
{
  return String( Left ) += Right;
}

inline String operator +( String const &Left, const char *Right )
{
  return String( Left ) += Right;
}

inline String operator +( const char Left, const String &Right )
{
  return String( &Left, 1 ) += Right;
}

inline String operator +( String const &Left, const char Right )
{
  return String( Left ) += Right;
}


//! Returns the string \a Str with all characters in lowercase form.
inline String String::toLower( String const &str )
{
  return String(str).makeLower();
}

//! Returns the string \a Str with all characters in uppercase form.
inline String String::toUpper( String const &str )
{
  return String(str).makeUpper();
}

//! Returns the string \a Str without any leading or trailing occurances of \a Chars.
inline String String::trim( String const &str, const char *chars )
{
  return String(str).trim(chars);
}

inline std::ostream &operator <<( std::ostream &out, NoCase const &str )
{
  return out << str.c_str();
}

// TEMPLATE IMPLEMENTATION ********************************************************************************************************

template <typename Container>
String String::join( Container const &strs, String const &sep )
{
  typename Container::ConstIterator str = strs.begin();
  String joined( *str++ );
  for ( ; str != strs.end(); ++str )
    joined += sep + String(*str);

  return joined;
}

// CONVERSIONS ********************************************************************************************************************

/*!
** Attempts to convert the string into an object of type \a Type.  If the string is empty, or if it cannot be converted to
** \a Type, a ConversionEx exception is thrown.  Otherwise, the converted value is returned.
*/
template <typename Type>
inline Type String::as( void ) const
{
  Type val;
  if ( !to<Type>( val ) )
    throwConversionEx( *this, typeid(Type) );

  return val;
}

/*!
** Attempts to convert the string into an object of type \a Type.  If the string is empty, returns \a defVal.  If the string
** cannot be converted ** \a Type, a ConversionEx exception is thrown.  Otherwise, the converted value is returned.
*/
template <typename Type>
inline Type String::as( Type const &defVal ) const
{
  return empty() ? defVal : as<Type>();
}

/*!
** Attempts to convert the string into an object of type \a Type.  If the string is empty, sets \a val to defVal and returns
** \c true.  Otherwise, if the conversion is succesfsul, sets \a val and returns \c true.  If the string cannot be converted to
** \a Type, returns \c false (\a val is unchanged).
*/
template <typename Type>
inline bool String::to( Type &val, Type const &defVal ) const
{
  if ( empty() ) {
    val = defVal;
    return true;
  }

  return to<Type>( val );
}

/*!
** Attempts to convert the string into an object of the type \a Type.  If the conversion
** is succesfsul, returns \c true and sets \a val.  Returns \c false, otherwise,
** and \a val is unchanged.
**
** Default template supports all integer and floating point types, using \c strto{l,ul,d}.
** \note Only hex and decimal are supported (who uses octal?!)
*/
template <typename Type>
bool String::to( Type &val ) const
{
  if ( !std::numeric_limits<Type>::is_specialized ) {
    // Not a numeric type -- attempt assignment.
    val = *this;
    return true;
  }

  const char *start = c_str();
  char *end;
  Type temp;
  if ( std::numeric_limits<Type>::is_integer ) {
    unsigned base = ((start[0] == '0') && (tolower( start[1] ) == 'x')) ? 16 : 10;

    if ( std::numeric_limits<Type>::is_signed )
      temp = (Type) strtol( start, &end, base );
    else
      temp = (Type) strtoul( start, &end, base );
  } else
    temp = (Type) strtod( start, &end );

  if ( (end == start) || (*end != '\0') )
    return false;

  val = temp;
  return true;
}


//! Template partial-instantiation for conversion of strings (i.e. no conversion).
//! \note always returns \c true.
template <>
inline bool String::to<String>( String &val ) const
{
  val = *this;
  return true;
}

//! Template partial-instantiation for conversion of strings (i.e. no conversion).
//! \note always returns \c true.
template <>
inline bool String::to<std::string>( std::string &val ) const
{
  val = *this;
  return true;
}


//! Template partial-instantiation for conversion of strings (i.e. no conversion).
//! \note always returns \c true.
template <>
inline bool String::to<char *>( char *&val ) const
{
  strncpy( val, c_str(), length() );
  return true;
}

}

#endif
