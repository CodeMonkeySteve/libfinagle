/*!
** \file FTP.cpp
** Created: Wed Dec. 1 2004
** Copyright (C) 2004 by Steve Sloan <steve@finagle.org>
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

#include "FTP.h"

#include "AppLog.h"
#include "Socket.h"
#include "Util.h"

#include <ncftp_errno.h>

#include "MemTrace.h"

using namespace Finagle;
using Finagle::UUID;

FTPLibraryInfo FTP::LibInfo;


FTP::FTP( void )
{
  if ( !LibInfo.init && (FTPInitLibrary( &LibInfo ) != kNoErr) ) {
    LOG_ERRL( UUID(0x8436fe06,0x2dec,0x11d9,0xbe,0xb6,0xb1,0x17,0xd8,0x61,0x18,0x91) )
      << "Cannot initialize FTP library!";
    return;
  }

  if ( FTPInitConnectionInfo( &LibInfo, &Session, kDefaultFTPBufSize ) < 0 ) {
    LOG_ERRL( UUID(0x923eb142,0x2dec,0x11d9,0xaa,0xfe,0xc2,0xf6,0x2f,0x61,0xb9,0xc5) )
      << "Cannot initialize FTP Connection Info";
    return;
  }
}


FTP::~FTP( void )
{
  disconnect();
}


void FTP::connect( IPAddress const &Server, String const &User, String const &Password, unsigned /*Throttle*/ )
{
  if ( isConnected() )
    disconnect();

  strncpy( Session.user, User, sizeof( Session.user ) );
  strncpy( Session.pass, Password, sizeof( Session.pass ) );
  strncpy( Session.host, Server.host(), sizeof( Session.host ) );
  Session.dataPortMode = kFallBackToSendPortMode;
  Session.maxDials = 1;
  Session.connTimeout = 5;
  Session.ctrlTimeout = 30;

// FIX ME: bandwidth throttling through libNcFTP
//  Session.Throttle = Throttle;

  if ( FTPOpenHost( &Session ) < 0 ) {
    Connected = false;
    checkResult( Session );
  }

  Connected = true;
}


void FTP::disconnect( bool Force )
{
  if ( Force )
    FTPShutdownHost( &Session );
  else
  if ( FTPCloseHost( &Session ) < 0 )
    FTPShutdownHost( &Session );

  Connected = false;
}


//! Returns the current working directory on the FTP server
Dir FTP::currentDir( void )
{
  char Temp[1025];
  if ( FTPGetCWD( &Session, Temp, sizeof( Temp ) ) < 0 )
    checkResult( Session );

  return( Dir( Temp ) );
}


//! Change the current working directory on the FTP server
void FTP::currentDir( Finagle::Dir const &Dir )
{
  if ( FTPChdir( &Session, Dir ) < 0 )
    checkResult( Session );
}


bool FTP::isDir( Finagle::Dir const &Dir )
{
  int Res = FTPIsDir( &Session, Dir );
  if ( Res >= 0 )
    return( Res == 1 );

  checkResult( Session );
  return( false );
}


void FTP::createDir( Finagle::Dir const &Dir, bool Recurse )
{
  if ( FTPMkdir( &Session, Dir, Recurse ? kRecursiveYes : kRecursiveNo ) != kNoErr )
    checkResult( Session );
}


bool FTP::exists( FilePath const &Path )
{
  if ( FTPFileExists( &Session, Path ) >= 0 )
    return( true );

  if ( Session.errNo != kErrNoSuchFileOrDirectory )
    checkResult( Session );

  return( false );
}


File::Size FTP::size( FilePath const &Path )
{
  long long Temp;
  if ( FTPFileSize( &Session, Path, &Temp, kTypeBinary ) != kNoErr )
    checkResult( Session );

  return( Temp );
}


DateTime FTP::modTime( FilePath const &Path )
{
  time_t Time;
  if ( FTPFileModificationTime( &Session, Path, &Time ) < 0 )
    checkResult( Session );

  return( DateTime( Time ) );
}


void FTP::rename( FilePath const &OldName, FilePath const &NewName, bool Force )
{
  if ( FTPRename( &Session, OldName, NewName ) < 0 ) {
    if ( (Session.errNo == kErrRenameFailed) && Force && exists( NewName ) ) {
      erase( NewName );
      rename( OldName, NewName, false );
      return;
    }

    checkResult( Session );
  }
}


void FTP::erase( String const &Pattern, bool DoGlob, bool Recurse )
{
  if ( FTPDelete( &Session, Pattern, Recurse ? kRecursiveYes : kRecursiveNo, DoGlob ? kGlobYes : kGlobNo ) < 0 )
    checkResult( Session );
}


FileArray FTP::list( Finagle::Dir const &Dir, String const &Glob )
{
  FileArray Files;
  LineList FileList;

  if ( FTPRemoteGlob( &Session, &FileList, FilePath( Dir, Glob ), kGlobYes ) < 0 )
    checkResult( Session );

  Files.clear();
  for ( LinePtr Iterator = FileList.first; Iterator; Iterator = Iterator->next )
    Files.push_back( Iterator->line );

  DisposeLineListContents( &FileList );
  return( Files );
}


void FTP::get( FilePath const &Src, Finagle::Dir const &DestDir, bool Resume )
{
  Session.cancelXfer = false;

  FilePath Dest( DestDir, Src.name() );
  if ( FTPGetOneFile3( &Session, Src, Dest, kTypeBinary, -1,
                       Resume ? kResumeYes : kResumeNo, kAppendNo, kDeleteNo, 0, 0 ) < 0 ) {

    if ( Session.errNo == kErrOpenFailed )
      throw File::OpenEx( Dest, std::ios::out );

    checkResult( Session, Dest );
  }
}


void FTP::put( FilePath const &Src, Finagle::Dir const &DestDir )
{
  Session.cancelXfer = false;

  FilePath Dest( DestDir, Src.name() );
  if ( FTPPutOneFile3( &Session, Src, Dest, kTypeBinary, -1, kAppendNo, 0, ".tmp", kResumeNo, kDeleteNo, 0, 0) < 0 ) {
    if ( Session.errNo == kErrOpenFailed )
      throw File::OpenEx( Dest, std::ios::in );

    checkResult( Session, Src );
  }
}


void FTP::checkResult( FTPConnectionInfo const &Session, FilePath LocalFile )
{
  switch ( Session.errNo ) {
    case kErrMallocFailed:
      throw std::bad_alloc();

    case kErrWriteFailed:
      throw File::IOEx( LocalFile, std::ios::out );

    case kErrReadFailed:
      throw File::IOEx( LocalFile, std::ios::in );

    case kErrSocketWriteFailed:
      throw Socket::IOEx( std::ios::out );

    case kErrSocketReadFailed:
      throw Socket::IOEx( std::ios::in );

    case kErrHostUnknown:
    case kErrNewStreamSocket:
    case kErrDupSocket:
    case kErrConnectMiscErr:
    case kErrConnectRetryableErr:
    case kErrConnectRefused:
    case kErrBadRemoteUser:
    case kErrBadRemoteUserOrPassword:
    case kErrLoginFailed:
      throw ConnectEx( Session );

    case kErrFdopenR:
    case kErrFdopenW:
    case kErrGetSockName:
    case kErrBindDataSocket:
    case kErrListenDataSocket:
    case kErrPassiveModeFailed:
    case kErrServerSentBogusPortNumber:
    case kErrConnectDataSocket:
    case kErrAcceptDataSocket:
    case kErrSetStartPoint:
    case kErrBadTransferType:
    case kErrHostDisconnectedDuringLogin:
    case kErrInvalidReplyFromServer:
    case kErrRemoteHostClosedConnection:
    case kErrNotConnected:
    case kErrCouldNotStartDataTransfer:
    case kErrDataTransferFailed:
    case kErrPORTFailed:
    case kErrPASVFailed:
    case kErrControlTimedOut:
    case kErrDataTimedOut:
      throw NetworkEx( Session );

    case kErrInvalidDirParam:
    case kErrPWDFailed:
    case kErrCWDFailed:
    case kErrRMDFailed:
    case kErrBadLineList:
    case kErrLISTFailed:
    case kErrRETRFailed:
    case kErrSTORFailed:
    case kErrMKDFailed:
    case kErrCannotGoToPrevDir:
    case kErrChmodFailed:
    case kErrUmaskFailed:
    case kErrDELEFailed:
    case kErrSIZEFailed:
    case kErrMDTMFailed:
    case kErrTYPEFailed:
    case kErrSIZENotAvailable:
    case kErrMDTMNotAvailable:
    case kErrRenameFailed:
    case kErrGlobFailed:
    case kErrUTIMEFailed:
    case kErrNoValidFilesSpecified:
    case kErrLocalFileNewer:
    case kErrRemoteFileNewer:
    case kErrLocalSameAsRemote:
    case kErrMLSTFailed:
    case kErrInvalidMLSTResponse:
    case kErrMLSTNotAvailable:
    case kErrSTATFailed:
    case kErrSTATwithFileNotAvailable:
    case kErrNLSTFailed:
    case kErrNLSTwithFileNotAvailable:
    case kErrNoSuchFileOrDirectory:
    case kErrCantTellIfFileExists:
      throw CommandEx( Session );

    default:
      break;
  }
}


#include <sstream>

FTP::ConnectEx::ConnectEx( FTPConnectionInfo const &Session )
: Exception( Session )
{
  String Host = IPAddress( Session.host ).host();
  Attribs["host"] = Host;
  Attribs["user"] = Session.user;

  std::ostringstream Str;
  if ( (ErrNum == kErrBadRemoteUser) || (ErrNum == kErrBadRemoteUserOrPassword) )
    Str << "Failed to login to " << Host << " as user " << Session.user << " (" << FTPStrError( ErrNum ) << ")";
  else
    Str << "Failed to connect to " << Host << " (" << FTPStrError( ErrNum ) << ")";

  append( Str.str() );
}


FTP::NetworkEx::NetworkEx( FTPConnectionInfo const &Session )
: Exception( Session )
{
  append( String( FTPStrError( ErrNum ) ) );
}


FTP::CommandEx::CommandEx( FTPConnectionInfo const &Session )
: Exception( Session )
{
  Attribs["res"] = Session.lastFTPCmdResultStr;

  std::ostringstream Str;
  Str << FTPStrError( ErrNum ) << " (" << Session.lastFTPCmdResultStr << ")";
  append( Str.str() );
}
