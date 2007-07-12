/*!
** \file LogFile.cpp
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

#include <iostream>

#include "AppLog.h"
#include "LogFile.h"
#include "DateTime.h"
#include "Util.h"
#include "Dir.h"

#include "MemTrace.h"

using namespace std;
using namespace Finagle;

/*! \class Finagle::LogToFile
** \brief Provides an XML log file.
**
** Log filenames are generate in the form "BaseName-YYYYMMDD.log".
*/

/*!
** Initializes the log, using \a LogBase as the base filename.  If \a LogBase is
** \c 0, it derives the log filename using findFile().
** \sa execFile().
*/
LogToFile::LogToFile( const char *LogBase, unsigned MaxLogFiles )
: Root( "Log" ), Today( 0 ), MaxFiles( (MaxLogFiles >= 2) ? MaxLogFiles : 2 ), LogBase( LogBase )
{
  Root["base"] = LogToFile::LogBase.title();
  Log().Msg.connect( this, &LogToFile::onMsg );
}


LogToFile::~LogToFile( void )
{
  close();
}


/*! \brief Opens the log.
**
** Returns \c true if successful, or \c false otherwise.
*/
void LogToFile::open( bool Force )
{
  if ( Stream.is_open() && !Force )
    return;

  if ( !LogBase || findFile().empty() ) {
    LOG_ERRL( UUID(0xc9a5b0cc,0x5ea2,0x11d9,0x89,0xa0,0x85,0x38,0x95,0xcd,0xb7,0xb8) )
     << "Unable to find log directory";
    return;
  }

  {
    ofstream NewStream;
    NewStream.open( LogPath, ofstream::out | ofstream::app );
    if ( !NewStream.is_open() ) {
      LOG_ERRL( UUID(0xc9a5b0cc,0x5ea2,0x11d9,0x89,0xa0,0x85,0x38,0x95,0xcd,0xb7,0xb8) )
       << "Unable to open log file: " << LogPath << " (" << SystemEx::sysErrStr() << ")";
      return;
    }
  }

  close();
  Stream.open( LogPath, ofstream::out | ofstream::app );
  Root.openTag( Stream );
  Stream << endl;

  purge();
}


void LogToFile::close( void )
{
  if ( !Stream.is_open() )
    return;

  Root.closeTag( Stream );
  Stream << endl;
  Stream.close();
}


void LogToFile::purge( void )
{
  FilePath Name;
  Dir Path;

  Name = LogBase.name();
  Path = FilePath( LogBase.dir() );

  // Use this application's executable filename (w/o extension).
  if ( Name.empty() )
    Name = exectitle();

  // Use executable's directory
  if ( Path.empty() )
    Path = FilePath( execFile().dir() );

  FileList Logs;
  for ( Dir::Iterator f = Path.begin(); f != Path.end(); ++f ) {
    if ( (NoCase( f->title().substr( 0, Name.length() ) ) != NoCase( Name )) ||
         (NoCase( f->ext() ) != "log") )
      continue;

    Logs.push_back( *f );
  }

  if ( Logs.size() > MaxFiles ) {
    LOG_INFO << "Purging old log files";
    Logs.sort();
    while ( Logs.size() > MaxFiles )
      pop_front( Logs ).erase();
  }
}


void LogToFile::onMsg( XML::Element const &Msg )
{
  time_t t;
  Msg["time"].as(t);
  open( DateTime(t).day() != Today );
  Stream << "  " << Msg << endl;
}


/*!
** \brief Generates the name and path of the current log file.
**
** If no file name or path is specified (i.e. in the constructor), uses the
** application executable name/path.  If no file extension is specified, uses ".LOG".
**
** \sa execFile().
*/

FilePath LogToFile::findFile( void )
{
  FilePath Name, Path;

  Name = LogBase.name();
  Path = LogBase.dir();

  // Use this application's executable filename (w/o extension).
  if ( Name.empty() )
    Name = exectitle();

  // Use executable's directory
  if ( Path.empty() )
    Path = execFile().dir();

  // Append date
  DateTime Now( DateTime::now() );
  Name += "-" + Now.format( "%Y%m%d" );
  Today = Now.day();

  // Append extension
  Name += ".log";

  return( LogPath = File( Path, Name ) );
}
