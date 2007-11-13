/*!
** \file UUID.h
** \author Steve Sloan <steve@finagle.org>
** \date Tue Jul 31 2007
** Copyright (C) 2007 by Steve Sloan
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

#ifndef FINAGLE_UUID_H
#define FINAGLE_UUID_H

#include <ostream>
#include <uuid.h>
#include <Finagle/Exception.h>
#include <Finagle/TextString.h>

namespace Finagle {

class UUID {
public:
  class Exception : public Finagle::Exception {
  public:
    Exception( uuid_rc_t res );
  };
  static const UUID nil;

public:
  UUID( void );
  UUID( String const &str );
  UUID( UUID const &id );
  UUID &operator =( UUID const &id );
 ~UUID( void );

  bool operator ==( UUID const &id );
  bool operator !=( UUID const &id );
  bool operator <( UUID const &id );

  bool isNil( void ) const;
  operator String( void ) const;

  UUID &generate( void );

protected:
  uuid_t *_uuid;
};

std::ostream &operator <<( std::ostream &out, UUID const &ID );

// INLINE IMPLEMENTATION ******************************************************

#define UUID_ASSERT( e ) {                   \
  uuid_rc_t __uuid_result = (e);                   \
  if ( __uuid_result != 0 ) {                \
    throw UUID::Exception( __uuid_result );  \
  }                                          \
}

//! Creates a \c nil id.
inline UUID::UUID( void )
: _uuid(0)
{
  UUID_ASSERT( uuid_create( &_uuid ) );
}

//! Creates an id by parsing string \a str.
inline UUID::UUID( String const &str )
: _uuid(0)
{
  UUID_ASSERT( uuid_create( &_uuid ) );
  UUID_ASSERT( uuid_import( _uuid, UUID_FMT_STR, str, str.length() ) );
}

inline UUID::UUID( UUID const &id )
: _uuid(0)
{
  UUID_ASSERT( uuid_clone( id._uuid, &_uuid ) );
}

inline UUID &UUID::operator =( UUID const &id )
{
  UUID_ASSERT( uuid_destroy( _uuid ) );
  UUID_ASSERT( uuid_clone( id._uuid, &_uuid ) );
  return *this;
}

inline UUID::~UUID( void )
{
  UUID_ASSERT( uuid_destroy( _uuid ) );
}


inline bool UUID::operator ==( UUID const &id )
{
  int res = 0;
  UUID_ASSERT( uuid_compare( _uuid, id._uuid, &res ) );
  return res == 0;
}

inline bool UUID::operator !=( UUID const &id )
{
  return ! operator==(id);
}

inline bool UUID::operator <( UUID const &id )
{
  int res = 0;
  UUID_ASSERT( uuid_compare( _uuid, id._uuid, &res ) );
  return res < 0;
}

//! Returns \c true if the id is invalid.
inline bool UUID::isNil( void ) const
{
  int res = 0;
  UUID_ASSERT( uuid_isnil( _uuid, &res ) );
  return res;
}

/*! \brief Generates a new universally unique identifier.
**
** The id generated is of the "classical" type, that is, computed using the ethernet MAC address and clock
** time of the running system.  It is globally-unique, but may leak some information about the system used to
** generate it.
*/
inline UUID &UUID::generate( void )
{
  UUID_ASSERT( uuid_make( _uuid, UUID_MAKE_V1 ) );
  return *this;
}

//! Writes a string representation of the \a id to stream \a out.
inline std::ostream &operator <<( std::ostream &out, UUID const &id )
{
  return out << (String) id;
}


inline UUID::Exception::Exception( uuid_rc_t res )
: Finagle::Exception( "UUID: " + String(uuid_error(res)) )
{}


};

#endif
