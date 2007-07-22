/*!
** \file FilePath.h
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

#ifndef FINAGLE_FILEPATH_H
#define FINAGLE_FILEPATH_H

#include <Finagle/TextString.h>
#include <Finagle/Array.h>
#include <Finagle/List.h>

namespace Finagle {

class FilePath : public String {
public:
  FilePath( const char *Path = "" );
  FilePath( String const &Path );
  FilePath( const char *DirName, const char *FileName, const char *FileExt = 0 );

public:
  String path( void ) const;
  String dir( void ) const;
  String name( void ) const;
  String ext( void ) const;
  String title( void ) const;

  FilePath absolute( void ) const;
  bool isAbsolute( void ) const;

  FilePath &operator +=( String const &str );

public:
  static const char DirDelim, ExtDelim;
};

typedef Array<FilePath> FileArray;
typedef List<FilePath>  FileList;

FilePath operator +( FilePath const &path, String const &str );
FilePath operator +( FilePath const &path, const char *str );

// INLINE IMPLEMENTATION ******************************************************

//! Initializes the class to point at the file \a Path.
inline FilePath::FilePath( const char *Path )
: String( Path )
{}


//! Initializes the class to point at the file \a Path.
inline FilePath::FilePath( String const &Path )
: String( Path )
{}


//! Returns the full path of the file.  Same as \c (String).
inline String FilePath::path( void ) const
{
  return *this;
}

/*!
** Returns the directory portion of the path, without trailing slash.  May
** return an empty string if there is no directory specified in the path.
*/
inline String FilePath::dir( void ) const
{
  const size_type i = rfind( DirDelim );
  return( (i == npos) ? "" : substr( 0, i ) );
}

//! Returns the filename portion of the path, including extension.
inline String FilePath::name( void ) const
{
  const size_type i = rfind( DirDelim );
  return( (i != npos) ? String( substr( i + 1 ) ) : String( *this ) );
}

/*!
** Returns the file extension.  This is portion of the path after the last
** period.  May return an empty string, if there is no extension.
*/
inline String FilePath::ext( void ) const
{
  const size_type i = rfind( ExtDelim );
  return( (i == npos) ? "" : substr( i + 1 ) );
}

//! Returns the title of the file, i.e. the filename without extension.
inline String FilePath::title( void ) const
{
  String Name( name() );
  const size_type i = Name.rfind( ExtDelim );
  return( (i != npos) ? String( Name, 0, i ) : Name );
}

//! Returns \c true iff the path is absolute (i.e. begins with DirDelim).
inline bool FilePath::isAbsolute( void ) const
{
  return operator[](0) == DirDelim;
}

inline FilePath &FilePath::operator +=( String const &str )
{
  if ( *(end() - 1) != DirDelim )
    append( 1, DirDelim );

  append( str );
  return *this;
}

inline FilePath operator +( FilePath const &path, String const &str )
{
  return FilePath(path) += str;
}


inline FilePath operator +( FilePath const &path, const char *str )
{
  return FilePath(path) += str;
}


}

#endif
