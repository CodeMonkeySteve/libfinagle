// Copyright 2000, Paul Nettle. All rights reserved.
//
// You are free to use this source code in any commercial or non-commercial product.
//
// MemTrace.h - Memory manager & tracking software
//
// The most recent version of this software can be found at: ftp://ftp.GraphicsPapers.com/pub/ProgrammingTools/MemoryManagers/

#ifndef FINAGLE_MEMTRACE_H
#define FINAGLE_MEMTRACE_H

#ifndef __FUNCTION__
#define  __FUNCTION__ "??"
#endif

namespace MemTrace {
  enum AllocType {
    ALLOC_UNKNOWN = 0,
    ALLOC_NEW, ALLOC_NEW_ARRAY, ALLOC_MALLOC, ALLOC_DELETE, ALLOC_DELETE_ARRAY,
    ALLOC_CALLOC, ALLOC_REALLOC, ALLOC_FREE,
  };

  void  setOwner( const char *file, unsigned line, const char *func );
  void *allocator(   const char *SrcFile, unsigned SrcLine, const char *SrcFunc, AllocType Type, size_t reportedSize );
  void *reallocator( const char *SrcFile, unsigned SrcLine, const char *SrcFunc, AllocType Type, size_t reportedSize, void *reportedAddress );
  void  deallocator( const char *SrcFile, unsigned SrcLine, const char *SrcFunc, AllocType Type, void const *reportedAddress );
}

#if defined( FINAGLE_TRACE_MEM ) && defined( __cplusplus )
void  *operator new( size_t reportedSize );
void  *operator new[]( size_t reportedSize );
void  *operator new( size_t reportedSize, const char *SrcFile, int SrcLine );
void  *operator new[]( size_t reportedSize, const char *SrcFile, int SrcLine );
void  operator delete( void *reportedAddress );
void  operator delete[]( void *reportedAddress );
#endif

#endif

#ifdef __cplusplus
#ifdef  new
#undef  new
#endif

#ifdef  delete
#undef  delete
#endif
#endif

#ifdef  malloc
#undef  malloc
#endif

#ifdef  calloc
#undef  calloc
#endif

#ifdef  realloc
#undef  realloc
#endif

#ifdef  free
#undef  free
#endif

#ifdef FINAGLE_TRACE_MEM
#ifdef __cplusplus
#define  new    (MemTrace::setOwner( __FILE__, __LINE__, __FUNCTION__ ), false ) ? 0 : new
#define  delete (MemTrace::setOwner( __FILE__, __LINE__, __FUNCTION__ ), false ) ? MemTrace::setOwner( "", 0, "" ) : delete
#endif

#define  malloc( sz )         MemTrace::allocator  ( __FILE__, __LINE__, __FUNCTION__, AllocType::ALLOC_MALLOC, sz )
#define  calloc( sz )         MemTrace::allocator  ( __FILE__, __LINE__, __FUNCTION__, AllocType::ALLOC_CALLOC, sz )
#define  realloc( ptr, sz )   MemTrace::reallocator( __FILE__, __LINE__, __FUNCTION__, AllocType::ALLOC_REALLOC, sz, ptr )
#define  free( ptr )          MemTrace::deallocator( __FILE__, __LINE__, __FUNCTION__, AllocType::FREE, ptr )
#endif
