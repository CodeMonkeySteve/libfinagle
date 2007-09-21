/*!
** \file Compress.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Wed Apr. 21 2004
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

#include "Compress.h"
#include "MemTrace.h"

using namespace std;
using namespace Finagle;

static const unsigned BuffSize = 4096;

CompressBuff::CompressBuff( void )
: File( 0 )
{
}


CompressBuff::~CompressBuff( void )
{
  close();

  delete [] eback();
  delete [] pbase();
  setg( 0, 0, 0 );
  setp( 0, 0 );
}


/*!
** \note ios::app and ios::in|ios::out not yet supported
** \note ios::binary  is implied
*/
CompressBuff *CompressBuff::open( FilePath const &Path, std::ios::openmode Mode )
{
  if ( is_open() )
    close();

  if ( (Mode & ios::in) && (Mode & ios::out) )
    return( 0 );

  if ( Mode & ios::in )
    File = gzopen( Path, "rb" );
  else
  if ( Mode & ios::out )
    File = gzopen( Path, "wb" );
  else
    return( 0 );

  if ( !File )
    return( 0 );

  // Allocate read buffer
  if ( Mode & ios::in ) {
    char_type *Buff = new char_type[BuffSize];
    setg( Buff, Buff + BuffSize, Buff + BuffSize );
  }

  // Allocate write buffer
  if ( Mode & ios::out ) {
    char_type *Buff = new char_type[BuffSize];
    setp( Buff, Buff + BuffSize );
  }

  CompressBuff::Mode = Mode;

  return this;
}


CompressBuff *CompressBuff::close( void )
{
  if ( !is_open() )
    return this;

  sync();
  gzclose( File );

  return this;
}


CompressBuff::int_type CompressBuff::sync( void )
{
  streamsize BytesOut = pptr() - pbase();
  if ( !BytesOut )
    return( 0 );

  if ( !is_open() || !(Mode & ios::out) )
    return traits::eof();

  while ( BytesOut ) {
    const streamsize BytesWritten = gzwrite( File, pbase(), BytesOut );
    if ( !BytesWritten || (BytesWritten == -1) )
      return -1;

    if ( BytesWritten < BytesOut )
      memmove( pbase(), pbase() + BytesWritten, BytesOut - BytesWritten );

    pbump( -BytesWritten );

    BytesOut -= BytesWritten;
  }

  return( 0 );
}


CompressBuff::int_type CompressBuff::overflow( int_type Ch )
{
  if ( !is_open() || !(Mode & ios::out) )
    return traits::eof();

  streamsize n = pptr() - pbase();

  if ( n && (sync() == -1) )
    return traits::eof();

  if ( (epptr() - pptr()) == 0 ) {
    char_type c = traits::to_char_type( Ch );
    return (gzwrite( File, &c, sizeof( c ) ) == sizeof( c )) ? 0 : -1;
  }

  *pptr() = traits::to_char_type( Ch );
  pbump( 1 );
  return( 0 );
}


CompressBuff::int_type CompressBuff::underflow( void )
{
  if ( !is_open() || !(Mode & ios::in) )
    return traits::eof();

  if ( gptr() < egptr() )
    return traits::to_int_type( *gptr() );

  streamsize n = gptr() - eback();

  if ( !n ) {
    char_type c;
    if ( gzread( File, &c, sizeof( c ) ) != sizeof( c ) )
      return traits::eof();

    return traits::to_int_type( c );
  }

  streamsize BytesIn = gzread( File, eback(), n );
  if ( BytesIn < 1 )
    return traits::eof();

  if ( BytesIn < n )
    memmove( eback() + n - BytesIn, eback(), BytesIn );

  gbump( -BytesIn );
  return traits::to_int_type( *gptr() );
}


bool Finagle::gzip( FilePath const &ArchFile, FilePath const &SrcFile )
{
  ifstream InFile( SrcFile );
  ozfstream OutFile( ArchFile );

  if ( !InFile.is_open() || !OutFile.is_open() )
    return false;

  OutFile << InFile.rdbuf() << flush;
  if ( !OutFile.good() || InFile.bad() ) {
    OutFile.close();
    File( ArchFile ).erase();
    return false;
  }

  return true;
}
