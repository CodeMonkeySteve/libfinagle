/*!
** \file DataStream.h
** \author Steve Sloan <steve@finagle.org>
** \date Wed May 19 2004
** Copyright (C) 2004 by Steve Sloan <steve@inagle.org>
**
** This program is free software; you can redistribute it and/or modify it
** under the terms of the GNU General Public License as published by the
** Free Software Foundation; either version 2 of the License, or (at your
** option) any later version.
**
** This program is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this library; if not, you may access it via the web at
** http://www.gnu.org/copyleft/gpl.html
*/

#ifndef FINAGLE_DATASTREAM_H
#define FINAGLE_DATASTREAM_H

#include <fstream>
#include <Finagle/File.h>
#include <ByteOrder.h>

namespace Finagle {

template <typename StreamType, typename ByteSwap = ByteOrder::Host>
class DataStream : public StreamType {
public:
  typedef unsigned char    Byte;
  typedef ByteOrder::Word  Word;
  typedef ByteOrder::DWord DWord;
  typedef ByteOrder::QWord QWord;

public:
  DataStream( void );
  DataStream( Finagle::FilePath const &Path );

  DataStream &operator >>( bool &Bool );
  DataStream &operator >>(          char &Char );
  DataStream &operator >>(   signed char &SChar );
  DataStream &operator >>( unsigned char &UChar );
  DataStream &operator >>(   signed short &SShort );
  DataStream &operator >>( unsigned short &UShort );
  DataStream &operator >>(   signed int &SInt );
  DataStream &operator >>( unsigned int &UInt );
  DataStream &operator >>(   signed long &SLong );
  DataStream &operator >>( unsigned long &ULong );
  DataStream &operator >>(  float &Float );
  DataStream &operator >>( double &Double );
  DataStream &operator >>(   signed long long &LLong );
  DataStream &operator >>( unsigned long long &ULLong );
  DataStream &operator >>( Finagle::String &Str );

  DataStream &operator <<( bool const &Bool );
  DataStream &operator <<(          char const &SChar );
  DataStream &operator <<(   signed char const &SChar );
  DataStream &operator <<( unsigned char const &UChar );
  DataStream &operator <<(   signed short const &SShort );
  DataStream &operator <<( unsigned short const &UShort );
  DataStream &operator <<(   signed int const &SInt );
  DataStream &operator <<( unsigned int const &UInt );
  DataStream &operator <<(   signed long const &SLong );
  DataStream &operator <<( unsigned long const &ULong );
  DataStream &operator <<(  float const &Float );
  DataStream &operator <<( double const &Double );
  DataStream &operator <<(   signed long long const &LLong );
  DataStream &operator <<( unsigned long long const &ULLong );
  DataStream &operator <<( Finagle::String const &Str );

protected:
  DataStream &readByte( Byte *b );
  DataStream &readWord( Word *w );
  DataStream &readDWord( DWord *dw );
  DataStream &readQWord( QWord *qw );

  DataStream &writeByte( Byte const *b );
  DataStream &writeWord( Word const *w );
  DataStream &writeDWord( DWord const *dw );
  DataStream &writeQWord( QWord const *qw );

protected:
  ByteSwap Order;
};

// INLINE/TEMPLATE IMPLEMENTATION *********************************************

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder>::DataStream( void )
{
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder>::DataStream( Finagle::FilePath const &Path )
: StreamType( Path )
{
}


template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::readByte( Byte *b )
{
  StreamType::read( (char *) b, sizeof( Byte ) );
  return *this;
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::readWord( Word *w )
{
  StreamType::read( (char *) w, sizeof( Word ) );
  Order( w );
  return *this;
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::readDWord( DWord *dw )
{
  StreamType::read( (char *) dw, sizeof( DWord ) );
  Order( dw );
  return *this;
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::readQWord( QWord *qw )
{
  StreamType::read( (char *) qw, sizeof( QWord ) );
  Order( qw );
  return *this;
}


template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::writeByte( Byte const *b )
{
  StreamType::write( (const char *) b, sizeof( Byte ) );
  return *this;
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::writeWord( Word const *w )
{
  Word W = *w;  Order( &W );
  StreamType::write( (const char *) &W, sizeof( Word ) );
  return *this;
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::writeDWord( DWord const *dw )
{
  DWord DW = *dw;  Order( &DW );
  StreamType::write( (const char *) &DW, sizeof( DWord ) );
  return *this;
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::writeQWord( QWord const *qw )
{
  QWord QW = *qw;  Order( &QW );
  StreamType::write( (const char *) &QW, sizeof( QWord ) );
  return *this;
}


template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator >>( bool &Bool )
{
  return( readByte( &Bool ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator >>( char &Char )
{
  return( readByte( &Char ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator >>( signed char &SChar )
{
  return( readByte( (Byte *) &SChar ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator >>( unsigned char &UChar )
{
  return( readByte( (Byte *) &UChar ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator >>( signed short &SShort )
{
  return( readWord( (Word *) &SShort ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator >>( unsigned short &UShort )
{
  return( readWord( (Word *) &UShort ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator >>( signed int &SInt )
{
  return( readDWord( (DWord *) &SInt ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator >>( unsigned int &UInt )
{
  return( readDWord( (DWord *) &UInt ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator >>( signed long &SLong )
{
  return( readDWord( (DWord *) &SLong ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator >>( unsigned long &ULong )
{
  return( readDWord( (DWord *) &ULong ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator >>( float &Float )
{
  return( readDWord( (DWord *) &Float ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator >>( double &Double )
{
  return( readQWord( (QWord *) &Double ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator >>( signed long long &SLLong )
{
  return( readQWord( (QWord *) &SLLong ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator >>( unsigned long long &ULLong )
{
  return( readQWord( (QWord *) &ULLong ) );
}

template <typename StreamType, typename ByteOrder>
DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator >>( Finagle::String &Str )
{
  // FIX ME!
  return *this;
}


template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator <<( bool const &Bool )
{
  return( writeByte( (Byte *) &Bool ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator <<( char const &Char )
{
  return( writeByte( (Byte *) &Char ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator <<( signed char const &SChar )
{
  return( writeByte( (Byte *) &SChar ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator <<( unsigned char const &UChar )
{
  return( writeByte( (Byte *) &UChar ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator <<( signed short const &SShort )
{
  return( writeWord( (Word *) &SShort ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator <<( unsigned short const &UShort )
{
  return( writeWord( (Word *) &UShort ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator <<( signed int const &SInt )
{
  return( writeDWord( (DWord *) &SInt ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator <<( unsigned int const &UInt )
{
  return( writeDWord( (DWord *) &UInt ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator <<( signed long const &SLong )
{
  return( writeDWord( (DWord *) &SLong ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator <<( unsigned long const &ULong )
{
  return( writeDWord( (DWord *) &ULong ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator <<( float const &Float )
{
  return( writeDWord( (DWord *) &Float ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator <<( double const &Double )
{
  return( writeQWord( (QWord *) &Double ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator <<( signed long long const &SLLong )
{
  return( writeQWord( (QWord *) &SLLong ) );
}

template <typename StreamType, typename ByteOrder>
inline DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator <<( unsigned long long const &ULLong )
{
  return( writeQWord( (QWord *) &ULLong ) );
}

template <typename StreamType, typename ByteOrder>
DataStream<StreamType, ByteOrder> &DataStream<StreamType, ByteOrder>::operator <<( Finagle::String const &Str )
{
  // FIX ME!
  return *this;
}

}

#endif
