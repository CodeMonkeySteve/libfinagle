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

#include <boost/signals.hpp>
#include <Finagle/ObjectPtr.h>
#include <Finagle/Set.h>
#include <Finagle/Singleton.h>

namespace Finagle {

namespace AppLoop {
  extern void process( Time );
}

class FileDescWatchable : public ReferenceCount {
public:
  typedef ObjectPtr<FileDescWatchable const> ConstPtr;

public:
  virtual ~FileDescWatchable( void );

  void enable( void ) const;
  void disable( void ) const;

protected:
  FileDescWatchable( void ) {}
  virtual int  fds( fd_set &readFDs, fd_set &writeFDs, fd_set &exceptFDs ) const = 0;
  virtual void onSelect( fd_set &readFDs, fd_set &writeFDs, fd_set &exceptFDs ) const = 0;

  friend void AppLoop::process( Time );

protected:
  static Set<FileDescWatchable const *> Active;
};

class FileDescWatcher : public FileDescWatchable {
public:
  typedef ObjectPtr<FileDescWatcher> Ptr;

public:
  FileDescWatcher( int fileDesc = -1 );
 ~FileDescWatcher( void );

  int fd( void ) const;
  void fd( int fileDesc );

public:
  mutable boost::signal<void ()> readable, writable, exception;

protected:
  int  fds( fd_set &readFDs, fd_set &writeFDs, fd_set &exceptFDs ) const;
  void onSelect( fd_set &readFDs, fd_set &writeFDs, fd_set &exceptFDs ) const;

protected:
  int _fd;
};

// INLINE IMPLEMENTATION ******************************************************

inline void FileDescWatchable::enable( void ) const
{
  Active.insert( this );
}

inline void FileDescWatchable::disable( void ) const
{
  Active.erase( this );
}


inline FileDescWatcher::FileDescWatcher( int fileDesc )
: _fd( -1 )
{
  fd( fileDesc );
}

inline FileDescWatcher::~FileDescWatcher( void )
{
  disable();
  _fd = -1;
}

inline int FileDescWatcher::fd( void ) const
{
  return _fd;
}

};

#endif
