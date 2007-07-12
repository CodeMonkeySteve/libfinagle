/*!
** \file XMLParse.h
** \author Steve Sloan <steve@finagle.org>
** \date Fri Dec 16 2004
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

#ifndef FINAGLE_XMLPARSE_H
#define FINAGLE_XMLPARSE_H

#include <istream>
#include <Finagle/FilePath.h>
#include <Finagle/XML.h>
#include <Finagle/Exception.h>

namespace Finagle {  namespace XML {

  struct ParseEx : public Exception {
    ParseEx( String const &Src, unsigned LineNum, String const &Err );
  };

  Element::Ref parse( std::istream &in, String SourceName = "stream" );
  Element::Ref parse( FilePath const &file );

} }

#endif
