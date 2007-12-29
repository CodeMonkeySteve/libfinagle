/*!
** \file AppLoop.cpp
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

#include <algorithm>  // for find()
#include <sys/socket.h>

#include "AppLoop.h"
#include "Timer.h"
#include "AppLog.h"
#include "FileDescWatcher.h"
#include "ThreadFunc.h"
#include "Util.h"

using namespace std;
using namespace Finagle;

/*! \namespace Finagle::AppLoop
** \brief Provides the main application loop.
**
** The application loop provides support for asynchronous file access (via FileDescWatcher and \c select(2)) as well
** as high-precision (i.e. video) timing via Timer.
** \sa FileDescWatcher, Timer
*/

namespace Finagle {
namespace AppLoop {

  //! Maximum time to wait for file descriptors
  Time ProcessTime = 0.03; // Suitable for 30fps video app

  //! Emitted when there are no pending alarms are file descriptors.
  signal0<> idle;

  bool Exit = false;
  int  ExitCode = 0;

  Thread::ID MainThreadId = Thread::self_id();

}};


/*! \fn int Finagle::AppLoop::exec( void )
** Runs the application loop.
**
** Returns the \a ExitCode.
** \sa exit().
*/
int AppLoop::exec( void )
{
  while ( !Exit )
    process( ProcessTime );

  return ExitCode;
}


void AppLoop::wait( Time wait )
{
  if ( Thread::self_id() == MainThreadId )
    process( wait );
  else
    sleep( wait );
}


void AppLoop::exit( int exitCode )
{
  AppLoop::ExitCode = exitCode;
  AppLoop::Exit = true;
}


/*! \fn void Finagle::AppLoop::process( Time )
** Processes a single iteration of the application loop.
**
** This does the grunt work of processing the application loop.  It handles
** alarms and file descriptors, as well as emiting the Idle signal.  Returns
** after \a maxTime seconds.
** \sa Timer and FileDescWatcher.
*/
void AppLoop::process( Time maxTime )
{
  Time startTime = Time::now();

  while ( true ) {
    if ( Exit )
      return;

    Time Now = Time::now();

    Timer::Ptr NextTimer = 0;

    // Make sure any pending alarms get triggered immediately
    while ( !Timers().empty() && (NextTimer = Timers().front()) ) {
      if ( NextTimer->nextAlarm() > Now )
        break;

      NextTimer->trigger();
      if ( Exit )
        return;
    }

    // Is there an alarm within the processing interval?
    if ( NextTimer && ((NextTimer->nextAlarm() - Now) > maxTime) )
      NextTimer = 0;

    // Run select() on file description watchers
    fd_set ReadFDs, WriteFDs, ExceptFDs;
    FD_ZERO( &ReadFDs );
    FD_ZERO( &WriteFDs );
    FD_ZERO( &ExceptFDs );

    int maxFD = -1;
    for ( List<FileDescWatcher *>::const_iterator i = FDWatchers().begin(); i != FDWatchers().end(); ++i ) {
      FileDescWatcher &w = **i;
      if ( !w.readable.connected() && !w.writable.connected() && !w.exception.connected() )
        continue;

      int fd = (*i)->fd();
      maxFD = max( maxFD, fd );

      if ( w.readable.connected() )   FD_SET( fd, &ReadFDs );
      if ( w.writable.connected() )   FD_SET( fd, &WriteFDs );
      if ( w.exception.connected() )  FD_SET( fd, &ExceptFDs );
    }

    // First, poll sockets to see if there's anything waiting (no timeout).
    int res = 0;
    if ( maxFD != -1 ) {
      timeval Timeout = { 0, 0 };
      res = select( (int) maxFD + 1, &ReadFDs, &WriteFDs, &ExceptFDs, &Timeout );

      if ( res == -1 ) {
        LOG_ERROR << "Error in select(2): " << SystemEx::sysErrStr();
        return;
      }
    }

    // No sockets ready, so run idle handlers and select again
    if ( res == 0 ) {
      idle();

      // Now poll/sleep with timeout <= time until next timer alarm
      double WaitTime = NextTimer ? (NextTimer->nextAlarm() - Now) : (double) maxTime;
      if ( maxFD != -1 ) {
        timeval Timeout = { unsigned( WaitTime ), unsigned( (WaitTime - unsigned( WaitTime )) * 1000000.0 ) };
        res = select( (int) maxFD + 1, &ReadFDs, &WriteFDs, &ExceptFDs, &Timeout );

        if ( res == -1 ) {
          LOG_ERROR << "Error in select(2): " << SystemEx::sysErrStr();
          return;
        }
      } else
        sleep( WaitTime );
    }

    if ( res == 0 ) {
      // Still no result.  Check timer and exit flag
      if ( NextTimer ) {
        NextTimer->trigger();
        NextTimer = 0;

        if ( Exit )
          return;
      }
    } else {
      // Examine select() results and notify appropriate FileDescWatchers.
      for ( List<FileDescWatcher *>::const_iterator i = FDWatchers().begin(); i != FDWatchers().end(); ) {
        FileDescWatcher::Ptr w = *(i++);
        int fd = w->fd();
        if ( fd == -1 )
          continue;

        bool isSet = false;
        if ( FD_ISSET( fd, &ExceptFDs ) ) {  w->exception(); isSet = true;  }
        if ( FD_ISSET( fd, &ReadFDs   ) ) {  w->readable();  isSet = true;  }
        if ( FD_ISSET( fd, &WriteFDs  ) ) {  w->writable();  isSet = true;  }

        if ( Exit )
          return;

        if ( isSet && (--res == 0) )
          break;
      }
    }

    // Have we processed enough, or should we quit?
    maxTime -= (Now - startTime);
    if ( (double) maxTime <= 0.0 )
      break;
  }
}


