/*!
** \file LogFile.h
** \author Steve Sloan <steve@finagle.org>
** \date Thu Nov 4 2004
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

#ifndef FINAGLE_LOGFILE_H
#define FINAGLE_LOGFILE_H

#include <fstream>
#include <AppLog.h>

namespace Finagle {

class LogToFile : public has_slots<> {
public:
  LogToFile( const char *FileBase = 0, unsigned MaxLogFiles = 30 );
 ~LogToFile( void );

  void open( bool Force = false );
  void close( void );
  FilePath const &path( void );

  LogEntry Root;

protected:
  void onMsg( XML::Element const &Msg );

  FilePath findFile( void );
  void purge( void );

protected:
  unsigned Today, MaxFiles;
  FilePath LogBase;
  File LogPath;
  std::ofstream Stream;
};

// INLINE IMPLEMENTATION ******************************************************

inline FilePath const &LogToFile::path( void )
{
  if ( LogPath.empty() )
    findFile();

  return( LogPath );
}


}

#endif
