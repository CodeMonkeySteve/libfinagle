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

#include <boost/bind.hpp>
#include <boost/signals.hpp>
#include <Finagle/Initializer.h>
#include <Finagle/Exception.h>
#include <Finagle/FileDescWatcher.h>

namespace Finagle {
namespace Transfer {

class Request;

typedef Finagle::Exception Exception;

class Processor : public FileDescWatchable {
public:
  Processor( void );
 ~Processor( void );

  Request const &add( Request &req );
  Request const &remove( Request &req );

protected:
  void process( void );

  int  fds( fd_set &readFDs, fd_set &writeFDs, fd_set &exceptFDs ) const;
  void onSelect( fd_set &readFDs, fd_set &writeFDs, fd_set &exceptFDs ) const;

protected:
  void *_reqs;
  std::map< void *, Request * > _reqMap;
};

static Finagle::Singleton<Processor> Proc;

// INLINE IMPLEMENTATION **********************************************************************************************************

} }

#endif
