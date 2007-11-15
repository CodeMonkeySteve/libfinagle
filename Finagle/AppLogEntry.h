/*!
** \file AppLogEntry.h
** \author Steve Sloan <ssloan@vigilos.com>
** \date Wed Mar 14 2007
** Copyright (C) 2007 by Vigilos, Inc., All Rights Reserved
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose can only be granted in writing by Vigilos, Inc.
*/

#ifndef APPLOGENTRY_H
#define APPLOGENTRY_H

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

}

#endif
