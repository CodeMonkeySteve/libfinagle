/*!
** \file ByteOrder.h
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

#ifndef FINAGLE_BYTEORDER_H
#define FINAGLE_BYTEORDER_H

#include <sys/param.h>

namespace Finagle {

#ifndef LITTLE_ENDIAN
  #define LITTLE_ENDIAN 1234
#endif
#ifndef BIG_ENDIAN
  #define BIG_ENDIAN 4321
#endif

#if !defined(BYTE_ORDER)
  #error No host byte order defined!
#endif

//! Provides architecture-dependant byte ordering at compile-time
namespace ByteOrder {
  typedef unsigned short      Word;
  typedef unsigned long      DWord;
  typedef unsigned long long QWord;

  //! Provides a functor to byte-swap to host-architecture order (i.e. does nothing)
  struct Host {
    //! Byte-swaps the (16-bit) word at \a w.
    void operator ()(  Word *w )  {}

    //! Byte-swaps the (32-bit) double-word at \a dw.
    void operator ()( DWord *dw ) {}

    //! Byte-swaps the (64-bit) quad-word at \a dw.
    void operator ()( QWord *qw ) {}
  };

  //! Provides a functor to byte-swap to the reverse of the host-architecture order.
  struct Reverse {
    //! Byte-swaps the (16-bit) word at \a w.
    void operator ()(  Word *w );

    //! Byte-swaps the (32-bit) double-word at \a dw.
    void operator ()( DWord *dw );

    //! Byte-swaps the (64-bit) quad-word at \a dw.
    void operator ()( QWord *qw );
  };

#if BYTE_ORDER == LITTLE_ENDIAN
  typedef Host    LittleEndian;
  typedef Reverse BigEndian;
  typedef Reverse Network;
#elif BYTE_ORDER == BIG_ENDIAN
  typedef Host    BigEndian;
  typedef Reverse LittleEndian;
  typedef Host    Network;
#else
  #error Unknown byte order!
#endif

//! \typedef LittleEndian
//! Provides a functor to byte-swap to little-endian (i.e. Intel) byte order.

//! \typedef BigEndian
//! Provides a functor to byte-swap to big-endian (i.e. Motorola et al.) byte order.

//! \typedef Network
//! Provides a functor to byte-swap to network byte order.

// INLINE IMPLEMENTATION ******************************************************

inline void Reverse::operator ()( Word *w )
{
  Word W = *w;
  *w = ((W & 0x00FFU) << 8) |
       ((W & 0xFF00U) >> 8);
}

inline void Reverse::operator ()( DWord *dw )
{
  DWord DW = *dw;
  *dw = ((DW & 0x000000FFUL) << 24) |
        ((DW & 0x0000FF00UL) << 8) |
        ((DW & 0x00FF0000UL) >>  8) |
        ((DW & 0xFF000000UL) << 24);
}

inline void Reverse::operator ()( QWord *qw )
{
  QWord QW = *qw;
  *qw = ((QW & 0x00000000000000FFULL) << 56) |
        ((QW & 0x000000000000FF00ULL) << 40) |
        ((QW & 0x0000000000FF0000ULL) << 24) |
        ((QW & 0x00000000FF000000ULL) << 8) |
        ((QW & 0x000000FF00000000ULL) >>  8) |
        ((QW & 0x0000FF0000000000ULL) >> 24) |
        ((QW & 0x00FF000000000000ULL) >> 40) |
        ((QW & 0xFF00000000000000ULL) >> 56);
}

}

}

#endif
