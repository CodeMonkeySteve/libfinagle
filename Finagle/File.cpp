/*!
** \file File.cpp
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
#include <sstream>
#include "File.h"
#include "MD5.h"
#include "MemTrace.h"

using namespace std;
using namespace Finagle;

/*! \class Finagle::File
** \brief Provides access to the filesystem information for file.
**
** This class inherits #FilePath to store the path, and adds functions for
** reading and modifying filesystem meta-data.
*/

const int File::AccessReadBit  = R_OK;
const int File::AccessWriteBit = W_OK;
const int File::ChmodReadBit   = S_IRUSR;
const int File::ChmodWriteBit  = S_IWUSR;

//! Compares the contents of the file with another file (\a that).
bool File::operator ==( File const &that ) const
{
  // Check size first
  if ( refresh() && refresh() && (size() != that.size()) )
    return false;

  ifstream StreamA( path() ), StreamB( that );
  if ( !StreamA.is_open() || !StreamB.is_open() )
    return false;

  streamsize CompareBytes = min( streamsize( size() ), streamsize( 32768 ) );
  Finagle::Array<char> BuffA( CompareBytes, char() ), BuffB( CompareBytes, char() );
  while ( StreamA && StreamB ) {
    StreamA.read( &BuffA.front(), CompareBytes );
    StreamB.read( &BuffB.front(), CompareBytes );

    if ( (StreamA.gcount() != StreamB.gcount()) || (BuffA != BuffB) )
      return false;
  }

  if ( StreamA.bad() || StreamB.bad() )
    return false;

  return true;
}


/*!
** \brief Erases the file, if it exists.
** \param force If true, and if necessary, change the file permissions to make it deletable.
** \return \c true, if successful.
*/
bool File::erase( bool force )
{
  if ( (::remove( path() ) == 0) || (SystemEx::sysErrCode() == ENOENT) )
    return true;

  if ( force && (SystemEx::sysErrCode() == EACCES) ) {
    writeable( true );
    return ::remove( path() ) == 0;
  }

  return false;
}


/*!
** \brief Copies the file to \a dest.
** If \a digest is non-\c NULL , calculates an MD5 digest on the file contents as it copies.
**/
bool File::copy( File &dest, MD5 *digest ) const
{
  if ( !isRegularFile() )
    return false;

  if ( dest.isDir() ) {
    File f( dest, name() );
    return copy( f, digest );
  }

  ifstream srcStream( path() );
  if ( !srcStream.is_open() )
    return false;

  File tempDest( 0, dest, "tmp" );
  ofstream destStream( tempDest, ios::out | ios::trunc | ios::binary );
  if ( !destStream.is_open() )
    return false;

  // Copy the file
  char buff[32768];
  while ( !srcStream.eof() ) {
    srcStream.read( buff, sizeof(buff) );
    destStream.write( buff, srcStream.gcount() );

    if ( digest )
      digest->fromMem( buff, srcStream.gcount() );

    if ( srcStream.bad() || destStream.bad() ) {
      destStream.close();
      tempDest.erase();
      return false;
    }
  }

  // Rename destination file
  destStream.close();
  return tempDest.rename(dest);
}


bool File::sync( File &dest ) const
{
  if ( !dest.exists() || (dest.size() != size()) || (dest.modifyTime() < modifyTime()) )
    return copy( dest );

// FIX ME: compare file contents

  return true;
}


//! Formats a size, in bytes, as a human-readable string using S.I. prefixes.
String Finagle::sizeStr( unsigned long long Bytes )
{
  static const char SIPrefix[] = {
    'K', 'M', 'G', 'T', 'P', 'E', 'Z', 'Y',
  };

  if ( Bytes < 1024 )
    return String::format( "%dB", (int) Bytes );

  double b = (double) Bytes;
  unsigned Mag = 0;
  while ( b >= 1024.0 ) {
    b /= 1024.0;
    Mag++;
  }

  if ( !Mag || (Mag >= sizeof( SIPrefix )) ) {
    // Unknown SI Prefix!
    return String::format( "%dB", (int) Bytes );
  }

  return String::format( "%.1f%cB", b, SIPrefix[Mag - 1] );
}


File::Ex::Ex( FilePath const &path, std::ios::openmode mode )
{
  String modeStr;
  if ( mode & ios::in )     modeStr += "r";
  if ( mode & ios::out )    modeStr += "w";
  if ( mode & ios::app )    modeStr += "a";
  if ( mode & ios::trunc )  modeStr += "t";

  attrib("path") = path;
  attrib("mode") = modeStr;
}

File::OpenEx::OpenEx( FilePath const &path, std::ios::openmode mode )
: Ex( path, mode )
{
  stringstream err;

  err << "Unable to open file " << path << " for ";

  if ( (mode & ios::in) && (mode & ios::out) )
    err << "read/write";
  else
  if ( mode & ios::in )
    err << "reading";
  else
  if ( mode & ios::out )
    err << "writing";

  *this << err.str();
}

File::IOEx::IOEx( FilePath const &path, std::ios::openmode mode )
: Ex( path, mode )
{
  stringstream err;

  err << "Unable to ";

  if ( mode & ios::in )
    err << "read from";
  else
  if ( mode & ios::out )
    err << "write to";
  else
    err << "access";

  err << "  file " << path;

  *this << err.str();
}
