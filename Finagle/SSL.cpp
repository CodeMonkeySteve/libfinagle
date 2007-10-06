/*!
** \file SSL.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Tue Dec 26 2006
** Copyright (C) 2006 by Steve Sloan
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

#include "SSL.h"

using namespace Finagle;
using namespace SSL;

static struct SSLInit {
  SSLInit( void ) {
    SSL_library_init();
    SSL_load_error_strings();
  }
} _SSLInit;

/*! \namespace Finagle::SSL
** \brief Provides Secure %Socket Layer functionality, via \c openssl.
** \note Currently incomplete!
*/

/*!
** \class Finagle::SSL::Context
*/

Context::Context( FilePath const &keyFile )
: _ctx( *SSL_CTX_new( SSLv23_method() ) )
{
  // load the certs (probably not used, but if left out it causes problems)
  if ( SSL_CTX_use_certificate_chain_file( &_ctx, keyFile.path() ) != 1 )
    throw BadKeyFileEx( keyFile );

  // load the keys
  if ( SSL_CTX_use_PrivateKey_file( &_ctx, keyFile, SSL_FILETYPE_PEM ) != 1 )
    throw BadKeyFileEx( keyFile );
}


Context::~Context( void )
{
  SSL_CTX_free( &_ctx );
}
