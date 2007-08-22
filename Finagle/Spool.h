/*!
** \file Spool.h
** \author Steve Sloan <steve@finagle.org>
** \date Tue Aug 21 2007
** Copyright (C) 2007 by Steve Sloan
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

#ifndef FINAGLE_SPOOL_H
#define FINAGLE_SPOOL_H

#include <algorithm>
#include <fstream>
#include <Finagle/Dir.h>
#include <Finagle/SizedQueue.h>
#include <Finagle/ThreadFunc.h>

namespace Finagle {

/*! \class Spool
** A specialized Queue that conserves memory by writing (i.e. spooling) entries to disk
** when the size becomes too large, and automatically reads them in again as needed.
** \note Requires that Type implement the stream redirection operators (i.e. << and >> ).
** \note
*/
template <typename Type, typename PtrType = ObjectRef<Type> >
class Spool : public Finagle::SizedQueue<PtrType> {
public:
  Spool( FilePath const &spoolDir, unsigned numCached );

  unsigned size( void ) const;

protected:
  void loadSpool( void );
  void saveSpool( void );

  void despool( void );

protected:
  Dir _dir;
  unsigned _readIdx, _writeIdx;
  ClassFuncThread<Spool> _despoolThread;
};

// INLINE/TEMPLATE IMPLEMENTATION *************************************************************************************************

template <typename Type, typename PtrType>
Spool<Type, PtrType>::Spool( Finagle::FilePath const &spoolDir, unsigned numCached )
: SizedQueue<Type>( numCached ), _dir( spoolDir ), _readIdx(0), _writeIdx(0), _despoolThread( this, &despool )
{
  loadSpool();
  _despoolThread.start();
}

template <typename Type, typename PtrType>
void Spool<Type, PtrType>::loadSpool( void )
{
  unsigned size = 0;
  for ( Dir::Iterator f = _dir.begin(); f != _dir.end(); ++f, ++size ) {
    unsigned x = f->as<unsigned>();
    if ( x < _readIdx )  _readIdx = x;
    else if ( x > _writeIdx )  _writeIdx = x;
  }
  _writeIdx++;

  if ( _writeIdx - _readIdx != size ) {
    throw Exception( "Spool directory (" + _dir + ") corrupted: " +
                     String(_readIdx) + "..." + String(_writeIdx) + " != " + String(size) );
  }
}

template <typename Type, typename PtrType>
void Spool<Type, PtrType>::saveSpool( void )
{
}

//! Thread function to keep the SizedQueue full, loading messages from the spool as necessary (and available).
template <typename Type, typename PtrType>
void Spool<Type, PtrType>::despool( void )
{
return;
  while ( true ) {
//    FilePath path = _spool.pop();
//    PtrType
//    std::ifstream f( String(_writeIdx) );
//    if ( !
//      if ( !(out << *msg) )
  }
}

}

#endif
