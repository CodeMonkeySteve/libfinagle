/*!
** \file PThreadEx.h
** \author Steve Sloan <ssloan@vigilos.com>
** \date Wed Mar 28 2007
** Copyright (C) 2007 by Vigilos, Inc., All Rights Reserved
**
** Permission to use, copy, modify, and distribute this software and its
** documentation for any purpose can only be granted in writing by Vigilos, Inc.
*/

#ifndef FINAGLE_PTHREADEX_H
#define FINAGLE_PTHREADEX_H

#include <pthread.h>
#include <Finagle/Exception.h>

namespace Finagle {

//! Exception thrown on \c pthread errors
class PThreadEx : public SystemEx {
public:
  PThreadEx( String const &str, int errCode );
};

// INLINE IMPLEMENTATION **********************************************************************************************************

inline PThreadEx::PThreadEx( String const &str, int errCode )
: SystemEx( "PThread error: " + str, errCode )
{}

#define PTHREAD_ASSERT( e ) {                 \
  int __pthread_result = (e);                 \
  if ( __pthread_result != 0 ) {              \
    throw PThreadEx( #e, __pthread_result );  \
  }                                           \
}

}

#endif
