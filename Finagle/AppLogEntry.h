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
#include <Finagle/XML.h>

namespace Finagle {

class LogEntry : public XML::Element {
public:
  LogEntry( String const &tag, String const &level = String() );
  LogEntry( String const &tag, String const &level, const char *file, unsigned line, String const &func );
  LogEntry( String const &tag, String const &level, String const &id, const char *file, unsigned line, String const &func );
};

// INLINE IMPLEMENTATION **********************************************************************************************************

inline LogEntry::LogEntry( String const &tag, String const &level )
: XML::Element( tag )
{
  if ( level )  attrib("level") = level;
  attrib("time") = String( (unsigned) DateTime::now().calTime() );
}

inline LogEntry::LogEntry( String const &tag, String const &level, const char *file, unsigned line, String const &func )
: XML::Element( tag )
{
  if ( level )  attrib("level") = level;
  attrib("time") = String( (unsigned) DateTime::now().calTime() );
  attrib("file") = FilePath( file ).name();
  attrib("line") = String( line );
  attrib("func") = func;
}

inline LogEntry::LogEntry( String const &tag, String const &level, String const &id, const char *file, unsigned line, String const &func )
: XML::Element( tag )
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
