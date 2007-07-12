/*!
** \file FTP.h
** \author Steve Sloan <steve@finagle.org>
** \date Wed Dec. 1 2004
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

#ifndef FINAGLE_FTP_H
#define FINAGLE_FTP_H

#include <Dir.h>
#include <Finagle/Exception.h>
#include <Finagle/IPAddress.h>

#include <ncftp.h>
#undef read
#undef write
#undef open
#undef close

namespace Finagle {

class FTP {
public:
  FTP( void );
 ~FTP( void );

  void connect( IPAddress const &Server, String const &User, String const &Password, unsigned Throttle = 0 );
  void disconnect( bool Force = false );
  bool isConnected( void ) const;
  void abort( void );

  Dir  currentDir( void );
  void currentDir( Dir const &Dir );
  bool isDir( Finagle::Dir const &Dir );
  void createDir( Finagle::Dir const &Dir, bool Recurse = false );

  bool exists( FilePath const &Path );
  File::Size size( FilePath const &Path );
  DateTime modTime( FilePath const &Path );
  void rename( FilePath const &OldName, FilePath const &NewName, bool Force = true );
  void erase( String const &Pattern, bool DoGlob = false, bool Recurse = false );

  FileArray list( Dir const &Dir, String const &Glob = "*" );

  void get( FilePath const &Src, Finagle::Dir const &DestDir, bool Resume = true );
  void put( FilePath const &Src, Finagle::Dir const &DestDir );

  unsigned transferRate( void ) const;  // bytes/sec

public:
  struct Exception : public Finagle::Exception {
    Exception( FTPConnectionInfo const &Session );
    int ErrNum;
  };

  struct ConnectEx : public Exception {
    ConnectEx( FTPConnectionInfo const &Session );
  };

  struct NetworkEx : public Exception {
    NetworkEx( FTPConnectionInfo const &Session );
  };

  struct CommandEx : public Exception {
    CommandEx( FTPConnectionInfo const &Session );
  };


protected:
  static void checkResult( FTPConnectionInfo const &Session, FilePath LocalFile = FilePath() );

protected:
  static FTPLibraryInfo LibInfo;

  FTPConnectionInfo Session;
  bool Connected;
};

// INLINE IMPLEMENTATION ******************************************************

inline bool FTP::isConnected( void ) const
{
  return( Connected );
}

inline void FTP::abort( void )
{
  Session.cancelXfer = true;
}

inline unsigned FTP::transferRate( void ) const
{
  return( unsigned( Session.bytesTransferred / Session.sec ) );
}

inline FTP::Exception::Exception( FTPConnectionInfo const &Session )
: ErrNum( Session.errNo )
{
  Attribs["module"] = "FTP";
}

}

#endif
