/*!
** \file UUID.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Mon Sep 24 2007
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

#include "UUID.h"

using namespace Finagle;

/*!
** \class Finagle::UUID
** \brief Universallyl Unique IDentifier (via \c OSSP \c uuid)
*/

/*!
** \class Finagle::UUID::Exception
** \brief Exception thrown on error from \c uuid_ function.
*/

const UUID UUID::nil;

/*! \brief Returns a string representation of the id
**
** Will be of the form: \code XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX \endcode
** unless the id is \c nil (i.e. #isNil), in which case an empty string is returned.
*/
UUID::operator String( void ) const
{
  if ( isNil() )
    return String();

  char tmp[UUID_LEN_STR + 1];
  char *tmpp = tmp;
  size_t tmpl = sizeof(tmp);
  UUID_ASSERT( uuid_export( _uuid, UUID_FMT_STR, (void **) &tmpp, &tmpl ) );
  return String( tmp, UUID_LEN_STR );
}

