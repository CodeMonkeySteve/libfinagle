/*!
** \file AppLogEntry.h
** \author Steve Sloan <ssloan@vigilos.com>
** \date Wed Mar 14 2007
** Copyright (C) 2007 by Vigilos, Inc., All Rights Reserved
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose can only be granted in writing by Vigilos, Inc.
*/

#ifndef FINAGLE_APPLOGENTRY_H
#define FINAGLE_APPLOGENTRY_H

#include <Finagle/FilePath.h>
#include <Finagle/DateTime.h>
#include <Finagle/XML/Element.h>
#include <Finagle/XML/Text.h>

namespace Finagle {

/*! \brief Generic (XML) application log entry
**
** All log entries have an XML name and level (e.g. "error", "warning", etc.)
** \sa AppLog.
*/
class LogEntry : public XML::Element {
public:
  typedef ObjectRef<LogEntry> Ref;

public:
  LogEntry( String const &name, String const &level = String() );
  LogEntry( String const &name, String const &level, const char *file, unsigned line, String const &func );
  LogEntry( String const &name, String const &level, String const &id, const char *file, unsigned line, String const &func );
};

//! A log message
class LogMsg : public LogEntry {
public:
  LogMsg( String const &level );
  LogMsg( String const &level, const char *file, unsigned line, String const &func );
  LogMsg( String const &level, String const &id, const char *file, unsigned line, String const &func );
};

//! A Debug log message
class LogDebug : public LogMsg {
public:
  LogDebug( const char *file, unsigned line, String const &func, String const &module = String() );
};

//! An Info log message
class LogInfo : public LogMsg {
public:
  LogInfo( void );
};

//! A Warning log message
class LogWarn : public LogMsg {
public:
  LogWarn( const char *file, unsigned line, String const &func );
};

//! An Error log message
class LogErr : public LogMsg {
public:
  LogErr( const char *file, unsigned line, String const &func );
};

//! A (failed) Assertion log message
class LogAssert : public LogErr {
public:
  LogAssert( String const &expr, const char *file, unsigned line, String const &func );
};

// INLINE IMPLEMENTATION **********************************************************************************************************

//! Creates a log entry with the given \a name and \a level.
inline LogEntry::LogEntry( String const &name, String const &level )
: XML::Element( name )
{
  if ( level )  attrib("level") = level;
  attrib("time") = String( (unsigned) DateTime::now().calTime() );
}

//! Creates a log entry with the given \a name and \a level, as well as the source \a file, \a line, and function (\a func)
//! that created it.
inline LogEntry::LogEntry( String const &name, String const &level, const char *file, unsigned line, String const &func )
: XML::Element( name )
{
  if ( level )  attrib("level") = level;
  attrib("time") = String( (unsigned) DateTime::now().calTime() );
  attrib("file") = FilePath( file ).name();
  attrib("line") = String( line );
  attrib("func") = func;
}

//! Creates a log entry with the given \a name and \a level, as well as a unique identifier and the source \a file, \a line, and
//! function (\a func) that created it.
inline LogEntry::LogEntry( String const &name, String const &level, String const &id, const char *file, unsigned line, String const &func )
: XML::Element( name )
{
  if ( level )  attrib("level") = level;
  attrib("time") = String( (unsigned) DateTime::now().calTime() );
  attrib("id") = id;
  attrib("file") = FilePath( file ).name();
  attrib("line") = String( line );
  attrib("func") = func;
}


inline LogMsg::LogMsg( String const &level )
: LogEntry( "Msg", level )
{}

inline LogMsg::LogMsg( String const &level, const char *file, unsigned line, String const &func )
: LogEntry( "Msg", level, file, line, func )
{}

inline LogMsg::LogMsg( String const &level, String const &id, const char *file, unsigned line, String const &func )
: LogEntry( "Msg", level, id, file, line, func )
{}


inline LogDebug::LogDebug( const char *file, unsigned line, String const &func, String const &module )
: LogMsg( "debug", file, line, func )
{
  if ( module )
    attrib("module") = module;
}


inline LogInfo::LogInfo( void )
: LogMsg( "info" )
{}


inline LogWarn::LogWarn( const char *file, unsigned line, String const &func )
: LogMsg( "warn", file, line, func ) {}


inline LogErr::LogErr( const char *file, unsigned line, String const &func )
: LogMsg( "error", file, line, func ) {}


inline LogAssert::LogAssert( String const &expr, const char *file, unsigned line, String const &func )
: LogErr( file, line, func )
{
  attrib("class") = "assert";
  attrib("expr") = expr;
}

}

#endif
