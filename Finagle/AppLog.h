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
#include <Finagle/sigslot.h>
#include <Finagle/Singleton.h>

namespace Finagle {

using namespace sigslot;
using std::endl;

class AppLog {
public:
  AppLog( void ) : _guard() {}
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

static Singleton<AppLog> Log;


class LogMsg : public LogEntry {
public:
  LogMsg( String const &level )
  : LogEntry( "Msg", level ) {}

  LogMsg( String const &level, const char *file, unsigned line, String const &func )
  : LogEntry( "Msg", level, file, line, func ) {}

  LogMsg( String const &level, String const &id, const char *file, unsigned line, String const &func )
  : LogEntry( "Msg", level, id, file, line, func ) {}
};

class LogDebug : public LogMsg {
public:
  LogDebug( const char *file, unsigned line, String const &func, String const &module = String() )
  : LogMsg( "debug", file, line, func )
  {
    if ( module )
      attribs()["module"] = module;
  }
};

class LogInfo : public LogMsg {
public:
  LogInfo( void )
  : LogMsg( "info" ) {}
};

class LogWarn : public LogMsg {
public:
  LogWarn( const char *file, unsigned line, String const &func )
  : LogMsg( "warn", file, line, func ) {}
};

class LogErr : public LogMsg {
public:
  LogErr( const char *file, unsigned line, String const &func )
  : LogMsg( "error", file, line, func ) {}
};

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
  LogToFile( FilePath const &base, bool asXML = false, bool debug = false );
  void onMsg( XML::Element const &msg );

  FilePath const &base( void ) const;
  FilePath const &base( FilePath const &base );

protected:
  FilePath _base;
  std::filebuf _buf;
};

// INLINE/TEMPLATE IMPLEMENTATION *************************************************************************************************

inline AppLog::Logger::Logger( void )
{
  Log()._loggers.push_back( this );
}

inline LogToStream::LogToStream( std::streambuf *buf, bool asXML, bool debug )
: _stream(buf), _asXML(asXML), _debug(debug)
{}

inline LogToFile::LogToFile( FilePath const &base, bool asXML, bool debug )
: LogToStream( &_buf, asXML, debug ), _base(base)
{}

inline FilePath const &LogToFile::base( void ) const
{
  return _base;
}

inline FilePath const &LogToFile::base( FilePath const &base )
{
  return _base = base;
}

#define LOG_DEBUG       Finagle::Log() += Finagle::LogDebug(  __FILE__, __LINE__, __FUNCTION__ )
#define LOG_DEBUGM( m ) Finagle::Log() += Finagle::LogDebug(  __FILE__, __LINE__, __FUNCTION__, m )
#define LOG_INFO        Finagle::Log() += Finagle::LogInfo()
#define LOG_WARN        Finagle::Log() += Finagle::LogWarn( __FILE__, __LINE__, __FUNCTION__ )
#define LOG_WARNL( l )  Finagle::Log() += Finagle::LogWarn( l, __FILE__, __LINE__, __FUNCTION__ )
#define LOG_ERR         Finagle::Log() += Finagle::LogErr( __FILE__, __LINE__, __FUNCTION__ )
#define LOG_ERRL( l )   Finagle::Log() += Finagle::LogErr( l, __FILE__, __LINE__, __FUNCTION__ )

#define FINAGLE_ASSERT( e ) \
  if ( !(e) ) {  Finagle::Log() += Finagle::LogAssert( #e, __FILE__, __LINE__, __FUNCTION__ );  }

}

#endif
