/*!
** \file AppLog.cpp
** \author Steve Sloan <steve@finagle.org>
** \date Sat Nov 1 2003
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
#include <sstream>

#include "AppLog.h"
#include "Dir.h"
#include "File.h"
#include "Util.h"

using namespace std;
using namespace Finagle;

/*! \class Finagle::AppLog
** \brief Application logging framework
**
** Implements generic application logging through a stream interface (i.e. the Log singleton).  All log messages (LogMsg) are
** derived from XML::Element, although they can be simple strings.  One side effect of this is that exceptions, being XML
** (see Exception) can be sent directly to Log.
**
** Logs are processed and delivered by subclasses of AppLog::Logger class.
*/

//! Add an XML element (e.g. a \a msg) to the log.
AppLog &AppLog::operator +=( XML::Element const &msg )
{
  if ( msg.text().empty() && msg.elements().empty() )
    return *this;

  Lock X( _guard );

  Msg( msg );
  for ( Logger::Iterator l = _loggers.begin(); l != _loggers.end(); ++l )
    l->onMsg( msg );

  return *this;
}

/*! \class Finagle::AppLog::Logger
** \brief Base class for objects that direct log entries to a specific location.
** \sa Finagle::LogToStream, Finagle::LogToFile
*/

/*! \brief Creates an AppLog Logger instance from a specification string (e.g. from the command line or config file)
** Where \a spec is one of:
**  * [xml|text]:stdout
**  * [xml|text]:stderr
**  * [xml|text]:file:[PATH/]BASE
** ... and ...
**  * BASE defaults to the executable name
**  * File has ".log" or ".xlog" appended
*/
AppLog::Logger::Ref AppLog::Logger::fromSpec( String const &spec, bool debug )
{
  String::Array logArgs = spec.split(':');
  NoCase form = pop_front(logArgs);
  NoCase type = pop_front(logArgs);

  bool asXML = false;
  if ( form == "xml" )
    asXML = true;
  else
  if ( form == "text" )
    asXML = false;
  else
    throw Exception( String("Bad log format \"") + form + "\": expecting \"text\" or \"xml\"" );

  if ( type == "stdout" )
    return new LogToStream( cout.rdbuf(), asXML, debug );

  if ( type == "stderr" )
    return new LogToStream( cerr.rdbuf(), asXML, debug );

  if ( type == "file" ) {
    String file( pop_front(logArgs) );
    if ( file.empty() )
      file = execTitle();

    return new LogToFile( file, asXML, debug );
  }

  throw Exception( String("Bad log type \"") + type + "\": expecting \"stdout\", \"stderr\" or \"file\"" );
}


/*! \class Finagle::LogToStream
** \brief Sends log entries to a given \c std::ostream.
*/

void LogToStream::onMsg( XML::Element const &msg )
{
  String const &level( msg.attrib("level") );

  if ( !_debug && (level == "debug") )
    return;

  if ( _asXML ) {
    _stream << msg << endl;
    return;
  }

/*
  time_t t;
  if ( msg["time"].extract( t ) )
    _stream << "[" << DateTime( t ).format( "%H:%M:%S" ) << "] ";
  else
    _stream << string( 11, ' ' );
*/

  if ( !msg.text().empty() ) {
    if ( level == "error" )
      _stream << "ERROR: ";
    else
    if ( level == "warn" )
      _stream << "WARNING: ";
    else
    if ( msg.tag() == "exception" )
      _stream << "EXCEPTION: ";

    _stream << msg.text() << endl;
  }

  for ( XML::Element::List::ConstIterator m = msg.elements().begin(); m != msg.elements().end(); ++m )
    onMsg( *m );
}


/*! \class Finagle::LogToFile
** \brief Sends log entries to an output file.
**
** The filename is determined by taking the #base name and appending ".log" or ".xlog", depending on the value of \a asXML passed
** to the constructor.
*/

/*! Sets the output file base name, reopenning the file if necessary.
*/
FilePath const &LogToFile::base( FilePath const &base )
{
  if ( base == _base )
    return _base;

  if ( _buf.is_open() )
    _buf.close();

  return _base = base;
}

/*! Logs the given log entry
**
** This will open the file, if necessary.  It will also create leading components of the base path, as necessary.
*/
void LogToFile::onMsg( XML::Element const &msg )
{
  if ( !_buf.is_open() ) {
    if ( !_base )
      return;

    Dir(_base.dir()).create();
    FilePath path( _base + (_asXML ? ".xlog" : ".log") );

    if ( !_buf.open( path, ios::out | ios::app | ios::binary ) ) {
      File::OpenEx ex( path, ios::out );
      _base.clear(); // Make sure we don't try to reopen to log the exception
      throw ex;
    }
  }

  LogToStream::onMsg( msg );
}


/* \class Finagle::LogToSysLog
** Log message handler to write to syslog(2)
*/
/*
//#include "Util.h"

#define SYSLOG_NAMES
#include <syslog.h>

LogToSysLog::LogToSysLog( int facility, String const &ident )
: _facility( facility == -1 ? LOG_USER : facility ), _ident( ident ? ident : execFile().name() )
{
  openlog( _ident, LOG_PID, _facility );
}


void LogToSysLog::onMsg( XML::Element const &msg )
{
  syslog( priority(msg["level"]) , "%s", (const char *) msgToText(msg) );
}


int LogToSysLog::facility( String const &fac )
{
  for ( CODE *facName = facilitynames; facName->c_name; ++facName ) {
    if ( fact == facName->c_name )
      return facName->c_val;
  }

  return LOG_USER;
}


int LogToSysLog::priority( String const &pri )
{
  for ( CODE *priName = prioritynames; priName->c_name; ++priName ) {
    if ( pri == priName->c_name )
      return priName->c_val;
  }

  return LOG_INFO;
}
*/
