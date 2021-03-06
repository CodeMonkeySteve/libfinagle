/*!
** \file MD5.h
** \date Tue Apr 20 2004
** Copyright (C) 1999, 2002 Aladdin Enterprises.  All rights reserved.
**
** Permission is granted to anyone to use this software for any purpose,
** including commercial applications, and to alter it and redistribute it
** freely, subject to the following restrictions:
**
** 1. The origin of this software must not be misrepresented; you must not
**    claim that you wrote the original software. If you use this software
**    in a product, an acknowledgment in the product documentation would be
**    appreciated but is not required.
** 2. Altered source versions must be plainly marked as such, and must not be
**    misrepresented as being the original software.
** 3. This notice may not be removed or altered from any source distribution.
**
**  L. Peter Deutsch
**  ghost@aladdin.com
**
** Independent implementation of MD5 (RFC 1321).
**
** This code implements the MD5 Algorithm defined in RFC 1321, whose
** text is available at http://www.ietf.org/rfc/rfc1321.txt
** The code is derived from the text of the RFC, including the test suite
** (section A.5) but excluding the rest of Appendix A.  It does not include
** any code or documentation that is identified in the RFC as being
** copyrighted.
**
** The original and principal author of md5.h is L. Peter Deutsch
** <ghost@aladdin.com>.  Other authors are noted in the change history
** that follows (in reverse chronological order):
**
** 2004-04-20 sts Imported into libFinagle and C++-ified
** 2002-04-13 lpd Removed support for non-ANSI compilers; removed references
**                to Ghostscript; clarified derivation from RFC 1321; now
**                handles byte order either statically or dynamically.
** 1999-11-04 lpd Edited comments slightly for automatic TOC extraction.
** 1999-10-18 lpd Fixed typo in header comment (ansi2knr rather than md5);
**                added conditionalization for C++ compilation from Martin
**                Purschke <purschke@bnl.gov>.
** 1999-05-03 lpd Original version.
*/

#ifndef FINAGLE_MD5_H
#define FINAGLE_MD5_H

#include <istream>
#include <ByteArray.h>
#include <Finagle/File.h>

namespace Finagle {

class MD5 {
public:
  typedef unsigned char Byte; // 8-bit byte
  typedef unsigned int  Word; // 32-bit word

  typedef ByteArray<16> Digest;

public:
  MD5( void );
  MD5( Digest const &digest );
  MD5( void const *bytes, unsigned numBytes );
  MD5( FilePath const &File );

  void fromMem( void const *bytes, unsigned numBytes );
  bool fromFile( FilePath const &file );
  bool fromStream( std::istream &in );
  bool fromStream( std::istream &in, unsigned numBytes );

  bool isValid( void ) const;

  Digest const &operator()( void );

  void reset( void );

protected:
  void processBlock( Byte const *Data );
  void finish( void );

protected:
  bool _valid;
  Digest _digest;

  Word _count[2];   // message length in bits, LSW first
  Word _buff[4];    // digest buffer
  Byte _accum[64];  // accumulator block
};

// INLINE IMPLEMENTATION ******************************************************

//! Constructs an empty message digest.
inline MD5::MD5( void )
{
  reset();
}

inline MD5::MD5( Digest const &digest )
: _digest( _digest )
{
  reset();
  _valid = true;
}

//! Generates a digest from \a numBytes data bytes read from \a bytes.
inline MD5::MD5( void const *bytes, unsigned numBytes )
{
  reset();
  fromMem( bytes, numBytes );
  finish();
}

//! Generates a digest from the contents of \a file.
inline MD5::MD5( FilePath const &file )
{
  reset();
  if ( fromFile( file ) )
    finish();
}

//! Returns \c true if the digest is valid.
inline bool MD5::isValid( void ) const
{
  return _valid;
}

//! Returns the message digest.
inline MD5::Digest const &MD5::operator()( void )
{
  finish();
  return _digest;
}


}

#endif
