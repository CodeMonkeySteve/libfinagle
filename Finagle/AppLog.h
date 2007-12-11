/*!
** \file AppLog.h
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

#ifndef FINAGLE_APPLOG_H
#define FINAGLE_APPLOG_H

#include <iostream>
#include <fstream>
#include <Finagle/AppLogEntry.h>
#include <Finagle/FilePath.h>
#include <Finagle/Map.h>
#include <Finagle/Mutex.h>
#include <Finagle/Singleton.h>
#include <sigslot/sigslot.h>

namespace Finagle {

using namespace sigslot;
using std::endl;

class AppLog {
public:
  AppLog( void ) : _guard() {}
  AppLog &operator <<( XML::Element const &msg );
  AppLog &operator +=( XML::Element const &msg );

  static String msgToText( XML::Element const &msg );

public:
  signal1<XML::Element const &> Msg;

public:
  class Logger : public has_slots<>, public ReferenceCount {
  public:
    typedef ObjectRef<Logger> Ref;
    typedef ObjectRefIterator<List<Logger::Ref>::Iterator> Iterator;
    typedef ObjectRefConstIterator<List<Logger::Ref>::Iterator> ConstIterator;

    Logger( void );
    virtual ~Logger( void ) {}
    static Logger::Ref fromSpec( String const &spec, bool debug = false );
    virtual void onMsg( XML::Element const &msg ) = 0;
  };
  friend class Logger;

protected:
  Mutex _guard;
  List<Logger::Ref> _loggers;
};

//! The application log singleton
static Singleton<AppLog> Log;

//! A log message
class LogMsg : public LogEntry {
public:
  LogMsg( String const &level )
  : LogEntry( "Msg", level ) {}

  LogMsg( String const &level, const char *file, unsigned line, String const &func )
  : LogEntry( "Msg", level, file, line, func ) {}

  LogMsg( String const &level, String const &id, const char *file, unsigned line, String const &func )
  : LogEntry( "Msg", level, id, file, line, func ) {}
};

//! A Debug log message
class LogDebug : public LogMsg {
public:
  LogDebug( const char *file, unsigned line, String const &func, String const &module = String() )
  : LogMsg( "debug", file, line, func )
  {
    if ( module )
      attribs()["module"] = module;
  }
};

//! An Info log message
class LogInfo : public LogMsg {
public:
  LogInfo( void )
  : LogMsg( "info" ) {}
};

//! A Warning log message
class LogWarn : public LogMsg {
public:
  LogWarn( const char *file, unsigned line, String const &func )
  : LogMsg( "warn", file, line, func ) {}
};

//! An Error log message
class LogErr : public LogMsg {
public:
  LogErr( const char *file, unsigned line, String const &func )
  : LogMsg( "error", file, line, func ) {}
};

//! A (failed) Assertion log message
class LogAssert : public LogErr {
public:
  LogAssert( String const &expr, const char *file, unsigned line, String const &func )
  : LogErr( file, line, func )
  {
    attribs()["class"] = "assert";
    attribs()["expr"] = expr;
  }
};


class LogToStream : public AppLog::Logger {
public:
  LogToStream( std::streambuf *buf, bool asXML = false, bool debug = false );
  void onMsg( XML::Element const &msg );

protected:
  std::ostream _stream;
  bool _asXML, _debug;
};

class LogToFile : public LogToStream {
public:
  LogToFile( String const &base, bool asXML = false, bool debug = false );
  void onMsg( XML::Element const &msg );

  String const &base( void ) const;
  String const &base( String const &base );

protected:
  String _base;
  std::filebuf _buf;
};

// INLINE/TEMPLATE IMPLEMENTATION *************************************************************************************************

inline AppLog &AppLog::operator +=( XML::Element const &msg )
{
  return operator <<( msg );
}

inline AppLog::Logger::Logger( void )
{
  Log()._loggers.push_back( this );
}

/*! Creates a Logger to send log entries to the given output stream buffer (\a buf).
**
** If \a asXML is \c false, XML entries will be converted to a plain text form.  If \a debug is \c false, entries with a level
** of \c debug will be silently dropped.
*/
inline LogToStream::LogToStream( std::streambuf *buf, bool asXML, bool debug )
: _stream(buf), _asXML(asXML), _debug(debug)
{}

/*! Creates a Logger to send log entries to an output file.
**
** If \a asXML is \c false, XML entries will be converted to a plain text form.  If \a debug is \c false, entries with a level
** of \c debug will be silently dropped.
*/
inline LogToFile::LogToFile( String const &base, bool asXML, bool debug )
: LogToStream( &_buf, asXML, debug ), _base(base)
{}

//! Returns the output file base name.
inline String const &LogToFile::base( void ) const
{
  return _base;
}

#define LOG_DEBUG        Finagle::Log() += Finagle::LogDebug(  __FILE__, __LINE__, __FUNCTION__ )
#define LOG_DEBUGM( m )  Finagle::Log() += Finagle::LogDebug(  __FILE__, __LINE__, __FUNCTION__, m )
#define LOG_INFO         Finagle::Log() += Finagle::LogInfo()
#define LOG_WARN         Finagle::Log() += Finagle::LogWarn( __FILE__, __LINE__, __FUNCTION__ )
#define LOG_WARNL( l )   Finagle::Log() += Finagle::LogWarn( l, __FILE__, __LINE__, __FUNCTION__ )
#define LOG_ERROR        Finagle::Log() += Finagle::LogErr( __FILE__, __LINE__, __FUNCTION__ )
#define LOG_ERRORL( l )  Finagle::Log() += Finagle::LogErr( l, __FILE__, __LINE__, __FUNCTION__ )

#define FINAGLE_ASSERT( e ) \
  if ( !(e) ) {  Finagle::Log() += Finagle::LogAssert( #e, __FILE__, __LINE__, __FUNCTION__ );  }

}

#endif
