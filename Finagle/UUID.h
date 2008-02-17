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
#include <uuid/uuid.h>
#include <Finagle/Exception.h>
#include <Finagle/TextString.h>

namespace Finagle {

class UUID {
public:
  static const UUID nil;

public:
  UUID( bool generate = false );
  UUID( String const &str );
  UUID( UUID const &id );
  UUID &operator =( UUID const &id );

  bool operator ==( UUID const &id ) const;
  bool operator !=( UUID const &id ) const;
  bool operator <( UUID const &id ) const;

  bool isNil( void ) const;
  operator String( void ) const;

  UUID &generate( void );

protected:
  uuid_t _uuid;
};

std::ostream &operator <<( std::ostream &out, UUID const &ID );

// INLINE IMPLEMENTATION ******************************************************

//! Creates a \c nil id.  if \a generate is \c true, creates a new UUID.
inline UUID::UUID( bool generate )
{
  if ( generate )
    UUID::generate();
  else
    uuid_clear( _uuid );
}

//! Creates an id by parsing string \a str.
inline UUID::UUID( String const &str )
{
  if ( uuid_parse( str, _uuid ) != 0 )
    throw Exception( "Invalid UUID: \"" + str + "\"" );
}

inline UUID::UUID( UUID const &id )
{
  uuid_copy( _uuid, id._uuid );
}

inline UUID &UUID::operator =( UUID const &id )
{
  uuid_copy( _uuid, id._uuid );
  return *this;
}


inline bool UUID::operator ==( UUID const &id ) const
{
  return uuid_compare( _uuid, id._uuid ) == 0;
}

inline bool UUID::operator !=( UUID const &id ) const
{
  return ! operator==(id);
}

inline bool UUID::operator <( UUID const &id ) const
{
  return uuid_compare( _uuid, id._uuid ) < 0;
}

//! Returns \c true if the id is invalid.
inline bool UUID::isNil( void ) const
{
  return uuid_is_null( _uuid );
}

/*! \brief Generates a new universally unique identifier.
**
** The id generated is of the "classical" type, that is, computed using the ethernet MAC address and clock
** time of the running system.  It is globally-unique, but may leak some information about the system used to
** generate it.
*/
inline UUID &UUID::generate( void )
{
  uuid_generate_time( _uuid );
  return *this;
}

//! Writes a string representation of the \a id to stream \a out.
inline std::ostream &operator <<( std::ostream &out, UUID const &id )
{
  return out << (String) id;
}

};

#endif
