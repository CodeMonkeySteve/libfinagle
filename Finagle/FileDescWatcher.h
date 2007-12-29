/*!
** \file FileDescWatcher.h
** \author Steve Sloan <steve@finagle.org>
** \date Tue Sep 7 2004
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

#ifndef FINAGLE_FILEDESCWATCHER_H
#define FINAGLE_FILEDESCWATCHER_H

#include <Finagle/ObjectPtr.h>
#include <Finagle/List.h>
#include <Finagle/Singleton.h>
#include <sigslot/sigslot.h>

namespace Finagle {

using namespace sigslot;

class FileDescWatcher : public ReferenceCount {
public:
  typedef ObjectPtr<FileDescWatcher> Ptr;

public:
  FileDescWatcher( int fileDesc = -1 );
  virtual ~FileDescWatcher( void );

  int fd( void ) const;
  void fd( int fileDesc );

public:
  signal0<> readable;
  signal0<> writable;
  signal0<> exception;

protected:
  int _fd;
};

// INLINE IMPLEMENTATION ******************************************************

inline FileDescWatcher::FileDescWatcher( int fileDesc )
: _fd(-1)
{
  fd( fileDesc );
}

inline FileDescWatcher::~FileDescWatcher( void )
{
  fd(-1);
}

inline int FileDescWatcher::fd( void ) const
{
  return _fd;
}

namespace AppLoop {
  static Singleton<List<FileDescWatcher *> > FDWatchers;
}

};

#endif
