/*!
** \file Dir.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Tue Dec 16 2003
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <limits.h>
#include <dirent.h>

#ifdef HAVE_SYS_VFS_H
#include <sys/vfs.h>
#else
#include <sys/param.h>
#include <sys/mount.h>
#endif


#include "Dir.h"
#include "MemTrace.h"
#include "Util.h"

using namespace std;
using namespace Finagle;

/*! \class Finagle::Dir
** \brief Provides access to the filesystem information for a directory.
*/

//! A platform-specific directory representing the top of the filesystem, e.g. "root" or "C:".
const Dir Dir::root( FilePath::DirDelim );

//! Returns the current working directory.
Dir Dir::current( void )
{
  char dir[PATH_MAX + 1] = "";
  return Dir( getcwd( dir, sizeof( dir ) ) );
}

//! Sets the current working directory
bool Dir::current( Dir const &dir )
{
  return chdir( dir.absolute().path() ) == 0;
}

//! Returns the current user's home directory (\c $HOME, \c /home/$USER, or \c /)
Dir Dir::home( void )
{
  const char *h = 0;
  if ( (h = getenv("HOME")) )
    return Dir(h);

  Dir d;
  if ( (h = getenv("USER")) && (d = (Dir("/home") + h)).exists() )
    return d;

  return Dir::root;
}

//! Returns the number of bytes available on the directory's filesystem.  Returns \c -1 on error.
long long Dir::spaceFree( void ) const
{
  struct statfs stats;
  if ( statfs( path(), &stats ) == -1 )
    return -1;

  return (long long) stats.f_bsize * stats.f_bavail;
}

/*!
** Returns the total size, in bytes, of all the files contained in the directory (and all of its subdirectories).
** \note: this only counts the file contents, not filesystem overhead.
*/
long long Dir::spaceUsed( void ) const
{
  unsigned long long totSize = 0;

  for ( Iterator i = begin(); i != end(); ++i )
    totSize += i->isDir() ? Dir(*i).spaceUsed() : i->size();

  return totSize;
}

//! Returns the number of files in the directory that have extension \a ext.
unsigned Dir::count( const char *ext ) const
{
  unsigned count = 0;
  for ( Iterator f = begin( ext ); f != end() ; ++f )
    count++;

  return count;
}

/*! \brief Creates the directory, if it doesn't exist, and returns \c true if successful.
** Will create any intermidiate directories, as need.
*/
bool Dir::create( unsigned mode ) const
{
  if ( exists() )
    return true;

  if ( !isAbsolute() )
    return Dir( absolute() ).create( mode );

  Dir const parent( dir() );
  if ( !(parent.exists() || parent.create( mode )) )
    return false;

  return (::mkdir( path(), mode ) == 0) && refresh( true );
}

//! \brief Removes the directory.
//! If \a recursive is \c true, will first remove all files and subdirectories.  Returns \c true if successful.
bool Dir::erase( bool recursive ) const
{
  if ( recursive ) {
    // Iterator through all files in directory and delete them
    for ( Iterator f = begin(); f != end(); ++f ) {
      // check if the name is a directory
      if ( !(f->isDir() ? Dir(*f).erase( true ) : f->erase()) )
        return false;
    }
  }

  return ::rmdir( path() ) == 0;
}

//! Copies and deletes files in \a dest such that it matches the files in \a this.
bool Dir::sync( Dir &dest ) const
{
  if ( !dest.exists() )
    dest.create( mode() );
//  else
//    dest.mode( mode() );

  // Remove files in dest which aren't in src (or which have a different type)
  for ( Iterator f = dest.begin(); f != dest.end(); ++f ) {
    File s( *this, f->name() );
    if ( !s.exists() || (s.isRegularFile() != f->isRegularFile()) )
      if ( ! (f->isRegularFile() ? f->erase() : Dir(*f).erase(true)) )
        return false;
  }

  // Sync all source files/directories to dest
  for ( Iterator f = begin(); f != end(); ++f ) {
    if ( f->isDir() ) {
      Dir d( dest, f->name() );
      if ( !Dir(*f).sync(d) )
        return false;
    } else {
      File d( dest, f->name() );
      if ( !f->sync(d) )
        return false;
    }
  }

  return true;
}


/*! \class Finagle::Dir::Iterator
** \brief Iterates through all of the entries of a given subdirectory.
** \note Constructed via call to Dir#begin or Dir#end.
*/

Dir::Iterator::Iterator( Iterator const &that )
: _dir( that._dir ), _ext( that._ext ), _search(0)
{
  _search = opendir( _dir );
  if ( !_search )
    return;

  findNextFile();
}

Dir::Iterator::Iterator( Finagle::Dir const &dir, const char *ext )
: _dir(dir), _ext(ext), _search(0)
{
  _search = opendir( _dir );
  if ( !_search )
    return;

  findNextFile();
}

Dir::Iterator::~Iterator( void )
{
  if ( _search )
    closedir( (DIR *) _search );
}

void Dir::Iterator::findNextFile( void )
{
  dirent *entry;
  while ( (entry = readdir( (DIR *) _search )) != 0 ) {
    File f( _dir.path(), entry->d_name );

    // do we have a . or a .. filename?
    if ( (f.name() == ".") || (f.name() == "..") )
      continue;

    // do we need to check the extension?
    if ( _ext && (NoCase( f.ext() ) != NoCase( _ext )) )
      continue;

    _curFile = f;
    break;
  }
}

/*! \class Finagle::TempDir
** \brief Represents a dynamically-created temporary directory
*/

TempDir::TempDir( unsigned mode )
{
  Dir base;

  const char *t = 0;
  if ( (t = getenv("TEMP")) || (t = getenv("TMP")) )
    base = Dir(t);
  else
    base = home() + "tmp";

  for ( unsigned n = 1; ; ++n ) {
    Dir d( base, execFile().name() + "-" + String(getpid()) + "-" + String(n) );
    if ( d.exists() )
      continue;

    if ( !d.create( mode ) )
      throw SystemEx( "Unable to create directory \"" + d + "\"" );

    assign( d );
    break;
  }
}


TempDir::~TempDir( void )
{
  erase( true );
}
