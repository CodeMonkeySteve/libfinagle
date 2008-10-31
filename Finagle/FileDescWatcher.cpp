/*!
** \file FileDescWatcher.cpp
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

#include "FileDescWatcher.h"

using namespace Finagle;

Set<FileDescWatchable const *> FileDescWatchable::Active;


FileDescWatchable::~FileDescWatchable( void )
{
  disable();
}


/*! \class Finagle::FileDescWatcher
** \brief Watches a file descriptor for a change in status (via \c select(2)).
**
** This class watches (using \c select(2)) a particular file descriptor for
** changes in status, i.e. readable, writable, or an exception occured.  It
** is intended to be used for sockets, but could be used for any operating
** system object which provides a file descriptor.
**
** \sa AppLoop, Socket and ServerSocket.
*/

void FileDescWatcher::fd( int fileDesc )
{
  if ( (fileDesc != -1) && (_fd == -1) )
    enable();
  else
  if ( (fileDesc == -1) && (_fd != -1) )
    disable();

  _fd = fileDesc;
}


int FileDescWatcher::fds( fd_set &readFDs, fd_set &writeFDs, fd_set &exceptFDs ) const
{
  if ( _fd == -1 )  return -1;

  bool r = !readable.empty(), w = !writable.empty(), e = !exception.empty();
  if ( !(r || w || e) )  return -1;

  if ( r )  FD_SET( _fd, &readFDs );
  if ( w )  FD_SET( _fd, &writeFDs );
  if ( e )  FD_SET( _fd, &exceptFDs );

  return _fd;
}


void FileDescWatcher::onSelect( fd_set &readFDs, fd_set &writeFDs, fd_set &exceptFDs ) const
{
  if ( FD_ISSET( _fd, &exceptFDs ) )  exception();
  if ( FD_ISSET( _fd, &readFDs   ) )  readable();
  if ( FD_ISSET( _fd, &writeFDs  ) )  writable();
}
