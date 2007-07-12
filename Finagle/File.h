/*!
** \file File.h
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

#ifndef FINAGLE_FILE_H
#define FINAGLE_FILE_H

#ifndef __USE_FILE_OFFSET64
#define __USE_FILE_OFFSET64
#endif

#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <Finagle/FilePath.h>
#include <Finagle/DateTime.h>
#include <Finagle/Exception.h>

namespace Finagle {

class MD5;

class File : public FilePath {
public:
  struct CompareSize {        bool operator()( File const &A, File const &B ) {  return A.size() < B.size();              }  };
  struct CompareAccessTime {  bool operator()( File const &A, File const &B ) {  return A.accessTime() < B.accessTime();  }  };

  //! The size of a file, in bytes.
  typedef unsigned long long Size;
  typedef mode_t Mode;

public:
  File( const char *Path = "" );
  File( String const &Path );
  File( FilePath const &Path );
  File( const char *DirName, const char *FileName, const char *FileExt = 0 );

  File &operator =( FilePath const &NewPath );

  bool refresh( bool Force = false ) const;
  bool exists( void ) const;
  bool isRegularFile( void ) const;
  bool isDir( void ) const;

  bool     createSymLink( FilePath const &toFile, bool force = false );
  FilePath followSymLink( void ) const;

  DateTime createTime( void ) const;
  DateTime accessTime( void ) const;
  DateTime modifyTime( void ) const;

  Size size( void ) const;
  Mode mode( void ) const;

  bool readable( void ) const;
  bool writeable( void ) const;

  bool readable( bool State );
  bool writeable( bool State );

  bool operator ==( File const &That ) const;
  bool operator !=( File const &That ) const;

public:
  bool touch( void );
  bool erase( bool force = true );
  bool rename( File dest, bool force = true );
  bool copy( File &dest, Finagle::MD5 *MD = 0 ) const;
  bool sync( File &dest ) const;

public:
  class Ex : public SystemEx {
  public:
    ~Ex( void ) throw() {}

  protected:
    Ex( FilePath const &path, std::ios::openmode mode = std::ios::in );
  };

  class OpenEx : public Ex {
  public:
    OpenEx( FilePath const &path, std::ios::openmode mode = std::ios::in );
   ~OpenEx( void ) throw() {}
  };

  class IOEx : public Ex {
  public:
    IOEx( FilePath const &path, std::ios::openmode mode );
   ~IOEx( void ) throw() {}
  };

protected:
  mutable struct stat FileInfo;

protected:
  static const int AccessReadBit, AccessWriteBit, ChmodReadBit, ChmodWriteBit;
};

String sizeStr( unsigned long long Bytes );

// INLINE IMPLEMENTATION ******************************************************

//! Initializes the class to point at the file \a Path.
inline File::File( const char *Path )
: FilePath( Path ), FileInfo()
{
  FileInfo.st_size = -1;
}

//! Initializes the class to point at the file \a Path.
inline File::File( String const &Path )
: FilePath( Path ), FileInfo()
{
  FileInfo.st_size = -1;
}

//! Initializes the class to point at the file \a Path.
inline File::File( const FilePath &Path )
: FilePath( Path ), FileInfo()
{
  FileInfo.st_size = -1;
}

/*!
** Initializes the class to point at the file \a FileName, with resides in the
** \a DirName directory, and has extension \a FileExt.  Any of the arguments
** may be \c 0.
*/
inline File::File( const char *DirName, const char *FileName, const char *FileExt )
: FilePath( DirName, FileName, FileExt ), FileInfo()
{
  FileInfo.st_size = -1;
}

//! Changes the class to reference file \a NewPath.
inline File &File::operator =( FilePath const &NewPath )
{
  FilePath::operator =( NewPath );
  FileInfo.st_size = -1;  // Force refresh later
  return *this;
}


/*! \brief Updates the file information.
**
** Normally, the file info is only updated when it's used.  If \a Force is
** \c true, will refresh even if cached values exist.
*/
inline bool File::refresh( bool Force ) const
{
  return( ((FileInfo.st_size != -1) && !Force) ||
          (stat( path(), &FileInfo ) == 0) );
}


/*! \brief Check if the file exists.
** \return \c true, if the file exists.
*/
inline bool File::exists( void ) const
{
  return( refresh( true ) );
}


/*!
** Check if the path is a regular file.
** \return \c true, if the path points to a regular file.
*/
inline bool File::isRegularFile( void ) const
{
  refresh();
  return( (FileInfo.st_mode & S_IFREG) != 0 );
}


/*!
** Check if the path is a directory.
** \return \c true, if the path points to a directory.
*/
inline bool File::isDir( void ) const
{
  refresh();
  return( (FileInfo.st_mode & S_IFDIR) != 0 );
}

/*!
** Returns the contents of the symlink (via readlink(2)).
** \note if the file is inaccesable (or not a symlink), the path returned will
**       be an empty string.
** \todo Test me
*/
inline FilePath File::followSymLink( void ) const
{
  char toFile[PATH_MAX+1];
  int res = readlink( path(), toFile, sizeof(toFile) );
  return res < 0 ? FilePath() : FilePath( String( toFile, res ) );
}

inline bool File::createSymLink( FilePath const &toFile, bool force )
{
  if ( force && exists() )
    erase();

  return ::symlink( toFile, path() ) == 0;
}

//! Returns the file's creation time.
inline DateTime File::createTime( void ) const
{
  refresh();
  return( FileInfo.st_ctime );
}

//! Returns the file's last access time.
inline DateTime File::accessTime( void ) const
{
  refresh();
  return( FileInfo.st_atime );
}

//! Returns the file's last modification time.
inline DateTime File::modifyTime( void ) const
{
  refresh();
  return( FileInfo.st_mtime );
}

//! Returns the file's size (in bytes).
inline File::Size File::size( void ) const
{
  refresh();
  return( FileInfo.st_size );
}

//! Returns the file's permission mode.
inline File::Mode File::mode( void ) const
{
  refresh();
  return( FileInfo.st_mode );
}

//! Returns \c true if the file has read permission.
inline bool File::readable( void ) const
{
  return( access( path(), AccessReadBit ) == 0 );
}

//! Returns \c true if the file has write permission.
inline bool File::writeable( void ) const
{
  return( access( path(), AccessWriteBit ) == 0 );
}

/*! \brief Sets the read permission on the file to \a State
** \return \c true , if the permission was set.
*/
inline bool File::readable( bool State )
{
  return( chmod( path(), (writeable() ? ChmodWriteBit : 0) |
                         (State       ? ChmodReadBit  : 0) ) == 0 );
}

/*! \brief Sets the write permission on the file to \a State
** \return \c true , if the permission was set.
*/
inline bool File::writeable( bool State )
{
  return( chmod( path(), (readable() ? ChmodReadBit  : 0) |
                         (State      ? ChmodWriteBit : 0) ) == 0 );
}

/*! \brief Compares the contents of the file with another file (\a That).
** \return \c true, if the files have the same contents.
*/
inline bool File::operator !=( File const &That ) const
{
  return( !operator==( That ) );
}

/*! \brief Updates the access and modification times to the current time.
**
** If the file doesn't exist, it will be created.
** \return \c true, if successful.
*/
inline bool File::touch( void )
{
  std::ofstream Stream( path(), std::ios::out );
  return( Stream.is_open() );
}

/*! Renames the file to \a dest.
** If \a force is \c true, it will overwrite any existing file.
*/
inline bool File::rename( File dest, bool force )
{
  if ( force && dest.exists() )
    dest.erase();

  return ::rename( path(), dest ) == 0;
}

}

#endif
