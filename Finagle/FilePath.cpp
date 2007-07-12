/*!
** \file FilePath.cpp
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

#include "FilePath.h"
#include "Dir.h"

using namespace std;
using namespace Finagle;

/*! \class Finagle::FilePath
** \brief Abstracts the filesystem location of a file.
*/

const char FilePath::DirDelim = '/';
const char FilePath::ExtDelim = '.';

/*!
** Initializes the class to point at the file \a FileName, with resides in the
** \a DirName directory, and has extension \a FileExt.  Any of the arguments
** may be \c 0.
*/
FilePath::FilePath( const char *DirName, const char *FileName, const char *FileExt )
{
  if ( !DirName || !DirName[0] )
    assign( FileName ? FileName : "" );
  else {
    assign( DirName );
    if ( *(end() - 1) != DirDelim )
      append( 1, DirDelim );

    if ( FileName )
      append( FileName );
  }

  if ( FileExt ) {
    append( "." );
    append( FileExt );
  }
}

FilePath FilePath::absolute( void ) const
{
  FilePath p( path().beginsWith( DirDelim ) ? path() : (Dir::current() + path()) );
  String::List dirs( p.split( DirDelim ) );

  for ( String::List::Iterator dir = dirs.begin(); dir != dirs.end(); ) {
    if ( *dir == "." )
      dirs.erase( dir++ );
    else
    if ( *dir == ".." ) {
      if ( dir != dirs.begin() ) {
        --dir;
        dirs.erase( dir++ );
      }

      dirs.erase( dir++ );
    } else
      ++dir;
  }

  return FilePath( DirDelim + String::join( dirs, DirDelim ) );
}
