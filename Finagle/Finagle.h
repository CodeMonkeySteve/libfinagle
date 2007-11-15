/*!
** \file Finagle.h
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

#ifndef FINAGLE_H
#define FINAGLE_H

#include <iomanip>

#include <sigslot/sigslot.h>

// Basic types, included for convenience
#include <Finagle/Timer.h>
#include <Finagle/AppLog.h>
#include <Finagle/AppLoop.h>
#include <Finagle/Dir.h>
#include <Finagle/Exception.h>
#include <Finagle/Array.h>
#include <Finagle/List.h>
#include <Finagle/Map.h>
#include <Finagle/MultiMap.h>
#include <Finagle/OrderedMap.h>
#include <Finagle/OptionParser.h>
#include <Finagle/Set.h>
#include <Finagle/ReferencedObject.h>
#include <Finagle/StreamIO.h>
#include <Finagle/Util.h>

/*!
** \brief Wraps all classes, types, and functions in the %Finagle library.
**
** Also includes \c sigslot, and \c std::endl (for convenience).
*/
namespace Finagle {
  using namespace sigslot;
  using std::endl;
}

#endif
