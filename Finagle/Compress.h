/*!
** \file Compress.h
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

#ifndef FINAGLE_COMPRESS_H
#define FINAGLE_COMPRESS_H

#include <fstream>
#include <zlib.h>
#include <Finagle/File.h>

namespace Finagle {

class CompressBuff : public std::streambuf {
public:
  typedef std::streambuf::traits_type traits;
  typedef std::streambuf::char_type   char_type;
  typedef std::streambuf::int_type    int_type;

public:
  CompressBuff( void );
 ~CompressBuff( void );

  CompressBuff *open( FilePath const &gzFile, std::ios::openmode mode );
  CompressBuff *close( void );

  bool is_open( void ) const;

  int_type sync( void );
  int_type overflow( int_type Ch );
  int_type underflow( void );

protected:
  gzFile _gzFile;
  std::ios::openmode _mode;
};

//! Convenience template for accessing a compressed iostream
template <class Base, std::ios_base::openmode DefMode>
class CompressTempl : public Base {
public:
  CompressTempl( void );
  CompressTempl( FilePath gzFile, std::ios_base::openmode mode = DefMode )
    : Base( &_buff ) {  open( gzFile, mode );  }

  void open( FilePath gzFile, std::ios_base::openmode mode = DefMode );
  bool is_open( void ) const;
  void close( void );

protected:
  CompressBuff _buff;
};

//! Compressed input stream
typedef CompressTempl<std::istream,  std::ios::in>  izfstream;

//! Compressed output stream
typedef CompressTempl<std::ostream,  std::ios::out> ozfstream;

//! Compressed input/output stream
typedef CompressTempl<std::iostream, std::ios::openmode( (int) std::ios::in | std::ios::out )> zfstream;

bool gzip( FilePath const &ArchFile, FilePath const &SrcFile );

// INLINE IMPLEMENTATION ******************************************************

CompressBuff::CompressBuff( void )
: _gzFile(0)
{}

inline bool CompressBuff::is_open( void ) const
{
  return _gzFile != 0;
}

// TEMPLATE IMPLEMENTATION ****************************************************

template <class Base, std::ios_base::openmode DefMode>
inline CompressTempl<Base, DefMode>::CompressTempl( void )
: Base( &_buff )
{}

template <class Base, std::ios_base::openmode DefMode>
inline void CompressTempl<Base, DefMode>::open( FilePath gzFile, std::ios_base::openmode mode )
{
  if ( !_buff.open( gzFile, mode ) )
    Base::setstate( Base::rdstate() | std::ios_base::badbit );
}

template <class Base, std::ios_base::openmode DefMode>
inline bool CompressTempl<Base, DefMode>::is_open( void ) const
{
  return _buff.is_open();
}

template <class Base, std::ios_base::openmode DefMode>
inline void CompressTempl<Base, DefMode>::close( void )
{
  _buff.close();
}

}

#endif
