/*!
** \file AppLoop.h
** \author Steve Sloan <steve@finagle.org>
** \date Tue Sep 7 2004
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

#ifndef FINAGLE_APPLOOP_H
#define FINAGLE_APPLOOP_H

#include <boost/signals.hpp>
#include <Finagle/DateTime.h>

namespace Finagle {
namespace AppLoop {

  extern boost::signal< void() > idle;

  int  exec( void );
  void process( Time MaxTime = 0.0 );
  void wait( Time Wait = 0.1 );
  void exit( int ExitCode = 0 );

} };

#endif
