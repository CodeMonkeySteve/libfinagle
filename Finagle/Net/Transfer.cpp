/*!
** \file Transfer.cpp
** \date Wed May 7 2008
** \author Steve Sloan <steve@finagle.org>
** Copyright (C) 2008 by Steve Sloan
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

#include <curl/curl.h>

#include "Transfer.h"

using namespace Finagle;
using namespace Transfer;

Transfer::Processor::Processor( void )
{
  curl_global_init( CURL_GLOBAL_ALL );
}

Transfer::Processor::~Processor( void )
{
  curl_global_cleanup();
}
