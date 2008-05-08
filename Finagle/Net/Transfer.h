/*!
** \file Transfer.h
** \author Steve Sloan <steve@finagle.org>
** \date Wed May 7 2008
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

#ifndef FINAGLE_NET_TRANSFER_H
#define FINAGLE_NET_TRANSFER_H

#include <Finagle/Initializer.h>
#include <Finagle/Exception.h>

#include <Finagle/Net/Request.h>
#include <Finagle/Net/Response.h>

namespace Finagle {
namespace Transfer {

typedef Finagle::Exception Exception;

class Processor {
public:
  Processor( void );
 ~Processor( void );

protected:
  void *_reqs;
};

static Finagle::Initializer<Processor> Proc;

// INLINE IMPLEMENTATION ******************************************************

} }

#endif
