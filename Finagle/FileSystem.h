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

//! Provides information about a filesystem (via \c statvfs)
class FileSystem  {
public:
  FileSystem( Dir const &dir );

  bool refresh( bool force = false ) const;

  Dir const &mountPoint( void ) const;

  unsigned long long bytesTotal( void ) const;
  unsigned long long bytesAvail( void ) const;
  unsigned long long bytesUsed( void ) const;

  unsigned long inodesTotal( void ) const;
  unsigned long inodesAvail( void ) const;
  unsigned long inodesUsed( void ) const;

protected:
  Dir _dir;
  mutable struct statvfs _info;
};

// INLINE IMPLEMENTATION ******************************************************

//! Initializes the class to point at the filesystem on which \a dir resides.
inline FileSystem::FileSystem( Dir const &dir )
: _dir( dir )
{
  info.f_bsize = 0;
}

/*! \brief Updates the filesystem information.
**
** Normally, the filesystem info is only updated when it's used.  If \a Force is \c true, will refresh even if cached values exist.
*/
inline bool FileSystem::refresh( bool force ) const
{
  return (info.f_bsize && !force) || (statvfs( path, &info ) == 0);
}


//! Returns the filesystem directory
inline Finagle::FilePath const &FileSystem::mountPoint( void ) const
{
  return _dir;
}

//! Returns the filesystem's total size, in bytes.
inline unsigned long long FileSystem::bytesTotal( void ) const
{
  refresh();
  return (unsigned long long) info.f_blocks * (unsigned long long) info.f_frsize;
}

//! Returns the filesystem's free size, in bytes.
inline unsigned long long FileSystem::bytesAvail( void ) const
{
  refresh();
  return (unsigned long long) info.f_bavail * (unsigned long long) info.f_frsize;
}

//! Returns the filesystem's used size, in bytes.
inline unsigned long long FileSystem::bytesUsed( void ) const
{
  return bytesTotal() - ((unsigned long long) info.f_bfree * (unsigned long long) info.f_frsize);
}

//! Returns the total number of inodes on the filesystem.
inline unsigned long FileSystem::inodesTotal( void ) const
{
  refresh();
  return info.f_files;
}

//! Returns the number of free inodes on the filesystem.
inline unsigned long FileSystem::inodesAvail( void ) const
{
  refresh();
  return info.f_favail;
}

//! Returns the number of used inodes on the filesystem.
inline unsigned long FileSystem::inodesUsed( void ) const
{
  return inodesTotal() - info.f_ffree;
}


};

#endif
