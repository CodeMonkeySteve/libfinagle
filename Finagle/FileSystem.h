/*!
** \file FileSystem.h
** \author Steve Sloan <steve@finagle.org>
** \date Thu Dec 7 2006
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

#ifndef FINAGLE_FILESYSTEM_H
#define FINAGLE_FILESYSTEM_H

#include <sys/statvfs.h>
#include <Dir.h>

namespace Finagle {

class FileSystem  {
public:
  FileSystem( const char *path );

  bool refresh( bool force = false ) const;

  Finagle::FilePath const &mountPoint( void ) const;

  unsigned long long bytesTotal( void ) const;
  unsigned long long bytesAvail( void ) const;
  unsigned long long bytesUsed( void ) const;

  unsigned long inodesTotal( void ) const;
  unsigned long inodesAvail( void ) const;
  unsigned long inodesUsed( void ) const;

protected:
  Finagle::FilePath path;
  mutable struct statvfs info;
};

// INLINE IMPLEMENTATION ******************************************************

//! Initializes the class to point at the filesystem on which \a path resides.
inline FileSystem::FileSystem( const char *path )
: path( path )
{
  info.f_bsize = 0;
}

/*! Updates the filesystem information.
**
** Normally, the filesystem info is only updated when it's used.  If \a Force is \c true, will refresh even if cached values exist.
*/
inline bool FileSystem::refresh( bool force ) const
{
  return( (info.f_bsize && !force) || (statvfs( path, &info ) == 0) );
}


inline Finagle::FilePath const &FileSystem::mountPoint( void ) const
{
  return path;
}


inline unsigned long long FileSystem::bytesTotal( void ) const
{
  refresh();
  return (unsigned long long) info.f_blocks * (unsigned long long) info.f_frsize;
}

inline unsigned long long FileSystem::bytesAvail( void ) const
{
  refresh();
  return (unsigned long long) info.f_bavail * (unsigned long long) info.f_frsize;
}

inline unsigned long long FileSystem::bytesUsed( void ) const
{
  return bytesTotal() - ((unsigned long long) info.f_bfree * (unsigned long long) info.f_frsize);
}


inline unsigned long FileSystem::inodesTotal( void ) const
{
  refresh();
  return info.f_files;
}

inline unsigned long FileSystem::inodesAvail( void ) const
{
  refresh();
  return info.f_favail;
}

inline unsigned long FileSystem::inodesUsed( void ) const
{
  return inodesTotal() - info.f_ffree;
}


};

#endif
