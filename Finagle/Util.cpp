/*!
** \file Util.cpp
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

#include <fstream>
#include <iomanip>
#include <vector>

#include "Util.h"
#include "MemTrace.h"

using namespace std;
using namespace Finagle;

//! Sleeps for \a Secs seconds
void Finagle::sleep( double Secs )
{
  if ( Secs > 0.0 )
    usleep( (unsigned long) ((Secs * 1000000) + 0.5) );
}


//! Returns the path of the currently running executable (or another executable by \a pid).
FilePath Finagle::execFile( pid_t pid )
{
  std::ostringstream Path;
  Path << "/proc/" << pid << "/exe";
  return File(Path.str()).followSymLink();
}


//! Returns the title of the currently running executable
String Finagle::execTitle( pid_t pid )
{
  String t( execFile( pid ).title() );
  if ( t.beginsWith( "lt-" ) )
    t.erase( 0, 3 );
  return t;
}


int Finagle::rand( Range<int> const &r )
{
  unsigned Rand = (::rand() << 16) ^ ::rand();
  return( r.height() ? r.lower() + ((int) Rand % r.height()) : r.lower() );
}


double Finagle::rand( Range<double> const &r )
{
  return( r.lower() + (((double) ::rand() * r.height()) / RAND_MAX) );
}
