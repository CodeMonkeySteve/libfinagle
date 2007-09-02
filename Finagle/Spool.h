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
*/
template <typename Type, typename PtrType = ObjectRef<Type>, typename Base = Finagle::Queue<PtrType> >
class Spool : public Base {
public:
  Spool( FilePath const &spoolDir, unsigned minCached );
  virtual ~Spool( void );

  unsigned size( void ) const;

protected:
  void loadSpool( void );
  void saveSpool( void );

  void spooler( void );

protected:
  Dir _dir;
  unsigned _readIdx, _writeIdx;
  ClassFuncThread<Spool> _spoolThread;
};

// INLINE/TEMPLATE IMPLEMENTATION *************************************************************************************************

template <typename Type, typename PtrType, typename Base>
Spool<Type, PtrType, Base>::Spool( Finagle::FilePath const &spoolDir, unsigned numCached )
: _dir( spoolDir ), _readIdx(0), _writeIdx(0),
  _spoolThread( this, &Spool<Type, PtrType, Base>::spooler )
{
  loadSpool();
  _spoolThread.start();
}


template <typename Type, typename PtrType, typename Base>
Spool<Type, PtrType, Base>::~Spool( void )
{}

template <typename Type, typename PtrType, typename Base>
unsigned Spool<Type, PtrType, Base>::size( void ) const
{
  return Base::size() + (_writeIdx - _readIdx - 1);
}


template <typename Type, typename PtrType, typename Base>
void Spool<Type, PtrType, Base>::loadSpool( void )
{
  unsigned size = 0;
  for ( Dir::Iterator f = _dir.begin(); f != _dir.end(); ++f, ++size ) {
    unsigned x = f->as<unsigned>();
    if ( x < _readIdx )  _readIdx = x;
    else if ( x > _writeIdx )  _writeIdx = x;
  }
  if ( _writeIdx - _readIdx != size ) {
    throw Exception( "Spool directory (" + _dir + ") corrupted: " +
                     String(_readIdx) + "..." + String(_writeIdx) + " != " + String(size) );
  }

  _writeIdx++;
}

template <typename Type, typename PtrType, typename Base>
void Spool<Type, PtrType, Base>::saveSpool( void )
{
}

//! Thread function to keep the SizedQueue full, loading messages from the spool as necessary (and available).
template <typename Type, typename PtrType, typename Base>
void Spool<Type, PtrType, Base>::spooler( void )
{
  while ( _spoolThread.running() ) {

sleep(0.01);
//    FilePath path = _spool.pop();
//    PtrType
//    std::ifstream f( String(_writeIdx) );
//    if ( !
//      if ( !(out << *msg) )
  }
}

}

#endif
