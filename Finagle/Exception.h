/*!
** \file Exception.h
** \author Steve Sloan <steve@finagle.org>
** \date Tue Apr. 13 2004
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

#ifndef FINAGLE_EXCEPTION_H
#define FINAGLE_EXCEPTION_H

#include <exception>
#include <stdexcept>
#include <cerrno>
#include <cstring>

#include <Finagle/AppLogEntry.h>

namespace Finagle {

/*! \class Finagle::Exception
** \brief An extension of the standard exception class (\c std::exception).
**
** Adds a virtual function for providing the error string, as well as caching for (static) error strings.
*/
class Exception : public std::exception {
public:
  Exception( String const &str = String() );
 ~Exception( void ) throw() {}

  XML::Element const &xwhat( void ) const throw();
  const char *what( void ) const throw();

protected:
  LogEntry::Ref _what;
};

/*! \class Finagle::SystemEx
** \brief An exception representing a standard system error.
*/
class SystemEx : public Exception {
public:
  static int    sysErrCode( void );
  static String sysErrStr( int ErrNum = sysErrCode() );

public:
  SystemEx( String const &str = String(), int ErrCode = sysErrCode() );
};

// INLINE IMPLEMENTATION ******************************************************

//! Sets the error to string \a str.
inline Exception::Exception( String const &str )
: _what( new LogEntry("exception") )
{
  _what->attrib("time") = String( (unsigned) DateTime::now().calTime() );
  if ( !str.empty() )
    _what->append( str );
}

//! Returns the error description as an XML element.
inline XML::Element const &Exception::xwhat( void ) const throw()
{
  return _what;
}

//! Returns the error description as C-style string.
inline const char *Exception::what( void ) const throw()
{
  return _what->text();
}


inline SystemEx::SystemEx( String const &str, int errCode )
: Exception( str + " (" + sysErrStr( errCode ) + ")" )
{
  _what->attrib("code") = String( errCode );
  _what->attrib("msg") = sysErrStr( errCode );
}

//! \brief Returns the platform-specific error code for the last operation.
//! \sa errno
inline int SystemEx::sysErrCode( void )
{
  return errno;
}

//! \brief Returns the platform-specific error string for the specified error code.
//! \sa strerror()
inline String SystemEx::sysErrStr( int errNum )
{
  return strerror( errNum );
}


}

#endif
