/*!
** \file Request.h
** \author Steve Sloan <steve@finagle.org>
** \date Wed May 7 2008
** Copyright (C) 2008 by Steve Sloan
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

#ifndef FINAGLE_NET_REQUEST_H
#define FINAGLE_NET_REQUEST_H

#include <Finagle/Exception.h>
#include <Finagle/Net/URL.h>
#include <Finagle/Net/Transfer.h>
#include <sigslot/sigslot.h>

namespace Finagle {
namespace Transfer {

class Response;

typedef Finagle::Exception Exception;

class Request : public ReferenceCount {
public:
  typedef ObjectPtr<Request> Ptr;

public:
  Request( URL const &url );
 ~Request( void );

  unsigned result( void ) const;
  bool succeeded( void ) const;
  bool failed( void ) const;

  void perform( void );

public:
  sigslot::signal2<String const &, size_t> recvBodyStart; //!< content type, size
  sigslot::signal2<const char *, size_t>   recvBodyFrag;  //!< data, size

protected:
  static size_t onBodyFrag( const char *data, size_t membSize, size_t membNum, Request *req );

protected:
  URL _url;
  void *_req;
  mutable unsigned _res;
  bool _firstFrag;

  friend class Processor;
};

// INLINE IMPLEMENTATION **********************************************************************************************************

inline bool Request::succeeded( void ) const
{
  unsigned res = result();
  return (res >= 200) && (res < 300);
}

inline bool Request::failed( void ) const
{
  unsigned res = result();
  return (res >= 400) && (res < 600);
}

} }

#endif
