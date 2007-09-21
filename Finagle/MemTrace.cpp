// ---------------------------------------------------------------------------------------------------------------------------------
// Copyright 2000, Paul Nettle. All rights reserved.
//
// You are free to use this source code in any commercial or non-commercial product.
//
// MemTrace.cpp - Memory manager & tracking software
//
// The most recent version of this software can be found at: ftp://ftp.GraphicsPapers.com/pub/ProgrammingTools/MemoryManagers/
//
// [NOTE: Best when viewed with 8-character tabs]
//
// ---------------------------------------------------------------------------------------------------------------------------------
//
// !!IMPORTANT!!
//
// This software is self-documented with periodic comments. Before you start using this software, perform a search for the string
// "-DOC-" to locate pertinent information about how to use this software.
//
// You are also encouraged to read the comment blocks throughout this source file. They will help you understand how this memory
// tracking software works, so you can better utilize it within your applications.
//
// NOTES:
//
// 1. This code purposely uses no external routines that allocate RAM (other than the raw allocation routines, such as malloc). We
//    do this because we want this to be as self-contained as possible. As an example, we don't use assert, because when running
//    under WIN32, the assert brings up a dialog box, which allocates RAM. Doing this in the middle of an allocation would be bad.
//
// 2. When trying to override new/delete under MFC (which has its own version of global new/delete) the linker will complain. In
//    order to fix this error, use the compiler option: /FORCE, which will force it to build an executable even with linker errors.
//    Be sure to check those errors each time you compile, otherwise, you may miss a valid linker error.
//
// 3. If you see something that looks odd to you or seems like a strange way of going about doing something, then consider that this
//    code was carefully thought out. If something looks odd, then just assume I've got a good reason for doing it that way (an
//    example is the use of the class MemStaticTimeTracker.)
//
// 4. With MFC applications, you will need to comment out any occurance of "#define new DEBUG_NEW" from all source files.
//
// 5. Include file dependencies are _very_important_ for getting the MMGR to integrate nicely into your application. Be careful if
//    you're including standard includes from within your own project inclues; that will break this very specific dependency order.
//    It should look like this:
//
//    #include <stdio.h>   // Standard includes MUST come first
//    #include <stdlib.h>  //
//    #include <streamio>  //
//
//    #include "mmgr.h"    // mmgr.h MUST come next
//
//    #include "myfile1.h" // Project includes MUST come last
//    #include "myfile2.h" //
//    #include "myfile3.h" //
//
// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef FINAGLE_TRACE_MEM

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <new>
#include <unistd.h>

#include "MemTrace.h"

using std::set_new_handler;
using namespace MemTrace;

struct AllocUnit {
  AllocType Type;
  size_t    actualSize;
  size_t    reportedSize;
  void     *actualAddress;
  void     *reportedAddress;
  char      SrcFile[40];
  char      SrcFunc[40];
  unsigned  SrcLine;
  bool      breakOnDealloc;
  bool      breakOnRealloc;
  unsigned  allocationNumber;
  AllocUnit *prev, *next;
};

struct MemStats {
  unsigned totalReportedMemory;
  unsigned totalActualMemory;
  unsigned peakReportedMemory;
  unsigned peakActualMemory;
  unsigned accumulatedReportedMemory;
  unsigned accumulatedActualMemory;
  unsigned accumulatedAllocUnitCount;
  unsigned totalAllocUnitCount;
  unsigned peakAllocUnitCount;
};

// Allocation breakpoints
bool &breakOnRealloc( void *reportedAddress );
bool &breakOnDealloc( void *reportedAddress );

// Utilitarian functions
bool validateAddress( const void *reportedAddress );
bool validateAllocUnit( const AllocUnit *Alloc );
bool validateAllAllocUnits( void );

// Unused RAM calculations
unsigned calcUnused( const AllocUnit *Alloc );
unsigned calcAllUnused( void );

// Logging and reporting
void    DumpAllocUnit(const AllocUnit *Alloc, const char *prefix = "");
void    dumpMemReport(const char *FileName = "memreport.log", const bool Overwrite = true);
MemStats getMemoryStatistics( void );


// ---------------------------------------------------------------------------------------------------------------------------------
// -DOC- If you're like me, it's hard to gain trust in foreign code. This memory manager will try to INDUCE your code to crash (for
// very good reasons... like making bugs obvious as early as possible.) Some people may be inclined to remove this memory tracking
// software if it causes crashes that didn't exist previously. In reality, these new crashes are the BEST reason for using this
// software!
//
// Whether this software causes your application to crash, or if it reports errors, you need to be able to TRUST this software. To
// this end, you are given some very simple debugging tools.
//
// The quickest way to locate problems is to enable the STRESS_TEST macro (below.) This should catch 95% of the crashes before they
// occur by validating every allocation each time this memory manager performs an allocation function. If that doesn't work, keep
// reading...
//
// Just because this memory manager crashes does not mean that there is a bug here! First, an application could inadvertantly damage
// the heap, causing malloc(), realloc() or free() to crash. Also, an application could inadvertantly damage some of the memory used
// by this memory tracking software, causing it to crash in much the same way that a damaged heap would affect the standard
// allocation routines.
//
// In the event of a crash within this code, the first thing you'll want to do is to locate the actual line of code that is
// crashing. You can do this by adding log() entries throughout the routine that crashes, repeating this process until you narrow
// in on the offending line of code. If the crash happens in a standard C allocation routine (i.e. malloc, realloc or free) don't
// bother contacting me, your application has damaged the heap. You can help find the culprit in your code by enabling the
// STRESS_TEST macro (below.)
//
// If you truely suspect a bug in this memory manager (and you had better be sure about it! :) you can contact me at
// midnight@GraphicsPapers.com. Before you do, however, check for a newer version at:
//
//  ftp://ftp.GraphicsPapers.com/pub/ProgrammingTools/MemoryManagers/
//
// When using this debugging aid, make sure that you are NOT setting the AlwaysLogAll variable on, otherwise the log could be
// cluttered and hard to read.
// ---------------------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------------------
// -DOC- Enable this sucker if you really want to stress-test your app's memory usage, or to help find hard-to-find bugs
// ---------------------------------------------------------------------------------------------------------------------------------

//#define  STRESS_TEST

// ---------------------------------------------------------------------------------------------------------------------------------
// -DOC- Enable this sucker if you want to stress-test your app's error-handling. Set RANDOM_FAIL to the percentage of failures you
//       want to test with (0 = none, >100 = all failures).
// ---------------------------------------------------------------------------------------------------------------------------------

//#define  RANDOM_FAILURE 100.0

// ---------------------------------------------------------------------------------------------------------------------------------
// -DOC- Locals -- modify these flags to suit your needs
// ---------------------------------------------------------------------------------------------------------------------------------

#ifdef  STRESS_TEST
static  const  unsigned  hashBits               = 12;
static    bool    RandomWipe             = true;
static    bool    AlwaysValidateAll      = true;
static    bool    AlwaysLogAll           = true;
static    bool    AlwaysWipeAll          = true;
static    bool    cleanupLogOnFirstRun   = true;
static  const  unsigned  paddingSize            = 1024; // An extra 8K per allocation!
#else
static  const  unsigned  hashBits               = 12;
static    bool    RandomWipe             = false;
static    bool    AlwaysValidateAll      = false;
static    bool    AlwaysLogAll           = false;
static    bool    AlwaysWipeAll          = true;
static    bool    cleanupLogOnFirstRun   = true;
static  const  unsigned  paddingSize            = 4;
#endif

// ---------------------------------------------------------------------------------------------------------------------------------
// We define our own assert, because we don't want to bring up an assertion dialog, since that allocates RAM. Our new assert
// simply declares a forced breakpoint.
// ---------------------------------------------------------------------------------------------------------------------------------

#define  MEM_ASSERT assert

// ---------------------------------------------------------------------------------------------------------------------------------
// Here, we turn off our macros because any place in this source file where the word 'new' or the word 'delete' (etc.)
// appear will be expanded by the macro. So to avoid problems using them within this source file, we'll just #undef them.
// ---------------------------------------------------------------------------------------------------------------------------------

#undef  new
#undef  delete
#undef  malloc
#undef  calloc
#undef  realloc
#undef  free

// ---------------------------------------------------------------------------------------------------------------------------------
// -DOC- Get to know these values. They represent the values that will be used to fill unused and deallocated RAM.
// ---------------------------------------------------------------------------------------------------------------------------------

static    unsigned  prefixPattern          = 0xbaadf00d; // Fill pattern for bytes preceeding allocated blocks
static    unsigned  postfixPattern         = 0xdeadc0de; // Fill pattern for bytes following allocated blocks
static    unsigned  unusedPattern          = 0xfeedface; // Fill pattern for freshly allocated blocks
static    unsigned  releasedPattern        = 0xdeadbeef; // Fill pattern for deallocated blocks

// ---------------------------------------------------------------------------------------------------------------------------------
// Other locals
// ---------------------------------------------------------------------------------------------------------------------------------

static const unsigned HashSize = 1 << hashBits;
static    AllocUnit  *HashTable[HashSize];
static    AllocUnit  *reservoir;
static    unsigned  currentAllocationCount = 0;
static    unsigned  breakOnAllocationCount = 0;
static    MemStats    stats;
static  const  char    *SrcFile            = "??";
static  const  char    *SrcFunc            = "??";
static    unsigned  SrcLine             = 0;
static    bool    staticDeinitTime       = false;
static    AllocUnit  **reservoirBuffer      = 0;
static    unsigned  reservoirBufferSize    = 0;
static const char *AllocTypeStrs[] = {
  "Unknown",
  "new", "new[]", "delete", "delete[]",
  "malloc", "calloc", "realloc", "free",
};

// ---------------------------------------------------------------------------------------------------------------------------------
// Local functions only
// ---------------------------------------------------------------------------------------------------------------------------------

static  void  doCleanupLogOnFirstRun()
{
  if ( cleanupLogOnFirstRun)
  {
    unlink("memalloc.log");
    cleanupLogOnFirstRun = false;
  }
}

// ---------------------------------------------------------------------------------------------------------------------------------

static  const char  *sourceFileStripper(const char *SrcFile)
{
  char  *ptr = strrchr(SrcFile, '\\');
  if ( ptr) return ptr + 1;
  ptr = strrchr(SrcFile, '/');
  if ( ptr) return ptr + 1;
  return SrcFile;
}

// ---------------------------------------------------------------------------------------------------------------------------------

static  const char  *ownerString(const char *SrcFile, unsigned SrcLine, const char *SrcFunc)
{
  static  char  str[90];
  memset(str, 0, sizeof(str));
  sprintf(str, "%s(%05d)::%s", sourceFileStripper(SrcFile), SrcLine, SrcFunc);
  return str;
}

// ---------------------------------------------------------------------------------------------------------------------------------

static  const char  *insertCommas(unsigned value)
{
  static  char  str[30];
  memset(str, 0, sizeof(str));

  sprintf(str, "%u", value);
  if ( strlen(str) > 3)
  {
    memmove(&str[strlen(str)-3], &str[strlen(str)-4], 4);
    str[strlen(str) - 4] = ',';
  }
  if ( strlen(str) > 7)
  {
    memmove(&str[strlen(str)-7], &str[strlen(str)-8], 8);
    str[strlen(str) - 8] = ',';
  }
  if ( strlen(str) > 11)
  {
    memmove(&str[strlen(str)-11], &str[strlen(str)-12], 12);
    str[strlen(str) - 12] = ',';
  }

  return str;
}

// ---------------------------------------------------------------------------------------------------------------------------------

static  const char  *MemSizeStr(unsigned long size)
{
  static  char  str[90];
       if ( size > (1024*1024))  sprintf(str, "%10s (%7.2fM)", insertCommas(size), (float) size / (1024.0f * 1024.0f));
  else if ( size > 1024)    sprintf(str, "%10s (%7.2fK)", insertCommas(size), (float) size / 1024.0f);
  else        sprintf(str, "%10s bytes     ", insertCommas(size));
  return str;
}

// ---------------------------------------------------------------------------------------------------------------------------------

static  AllocUnit  *findAllocUnit(const void *reportedAddress)
{
  // Just in case...
  MEM_ASSERT( reportedAddress );

  // Use the address to locate the hash index. Note that we shift off the lower four bits. This is because most allocated
  // addresses will be on four-, eight- or even sixteen-byte boundaries. If we didn't do this, the hash index would not have
  // very good coverage.

  unsigned  HashIndex = ((unsigned) reportedAddress >> 4) & (HashSize - 1);
  AllocUnit *ptr = HashTable[HashIndex];
  while(ptr) {
    if ( ptr->reportedAddress == reportedAddress)
      return ptr;

    ptr = ptr->next;
  }

  return 0;
}

// ---------------------------------------------------------------------------------------------------------------------------------

static  size_t  calculateActualSize(const size_t reportedSize)
{
  // We use DWORDS as our padding, and a long is guaranteed to be 4 bytes, but an int is not (ANSI defines an int as
  // being the standard word size for a processor; on a 32-bit machine, that's 4 bytes, but on a 64-bit machine, it's
  // 8 bytes, which means an int can actually be larger than a long.)

  return reportedSize + paddingSize * sizeof(long) * 2;
}

// ---------------------------------------------------------------------------------------------------------------------------------

static  size_t  calculateReportedSize(const size_t actualSize)
{
  // We use DWORDS as our padding, and a long is guaranteed to be 4 bytes, but an int is not (ANSI defines an int as
  // being the standard word size for a processor; on a 32-bit machine, that's 4 bytes, but on a 64-bit machine, it's
  // 8 bytes, which means an int can actually be larger than a long.)

  return actualSize - paddingSize * sizeof(long) * 2;
}

// ---------------------------------------------------------------------------------------------------------------------------------

static  void  *calculateReportedAddress(const void *actualAddress)
{
  // We allow this...

  if ( !actualAddress) return 0;

  // JUst account for the padding

  return (void *) ((char *) actualAddress + sizeof(long) * paddingSize);
}

// ---------------------------------------------------------------------------------------------------------------------------------

static  void  wipeWithPattern(AllocUnit *Alloc, unsigned long pattern, unsigned originalReportedSize = 0)
{
  // For a serious test run, we use wipes of random a random value. However, if this causes a crash, we don't want it to
  // crash in a differnt place each time, so we specifically DO NOT call srand. If, by chance your program calls srand(),
  // you may wish to disable that when running with a random wipe test. This will make any crashes more consistent so they
  // can be tracked down easier.

  if ( RandomWipe)
  {
    pattern = ((rand() & 0xff) << 24) | ((rand() & 0xff) << 16) | ((rand() & 0xff) << 8) | (rand() & 0xff);
  }

  // -DOC- We should wipe with 0's if we're not in debug mode, so we can help hide bugs if possible when we release the
  // product. So uncomment the following line for releases.
  //
  // Note that the "AlwaysWipeAll" should be turned on for this to have effect, otherwise it won't do much good. But we'll
  // leave it this way (as an option) because this does slow things down.
//  pattern = 0;

  // This part of the operation is optional

  if ( AlwaysWipeAll && Alloc->reportedSize > originalReportedSize)
  {
    // Fill the bulk

    long  *lptr = (long *) ((char *)Alloc->reportedAddress + originalReportedSize);
    int  length = Alloc->reportedSize - originalReportedSize;
    int  i;
    for (i = 0; i < (length >> 2); i++, lptr++)
    {
      *lptr = pattern;
    }

    // Fill the remainder

    unsigned  shiftCount = 0;
    char    *cptr = (char *) lptr;
    for (i = 0; i < (length & 0x3); i++, cptr++, shiftCount += 8)
      *cptr = (char) (pattern & (0xff << shiftCount)) >> shiftCount;
  }

  // Write in the prefix/postfix bytes

  long    *pre = (long *) Alloc->actualAddress;
  long    *post = (long *) ((char *)Alloc->actualAddress + Alloc->actualSize - paddingSize * sizeof(long));
  for (unsigned i = 0; i < paddingSize; i++, pre++, post++)
  {
    *pre = prefixPattern;
    *post = postfixPattern;
  }
}

// ---------------------------------------------------------------------------------------------------------------------------------

static  void  resetGlobals()
{
  SrcFile = "??";
  SrcLine = 0;
  SrcFunc = "??";
}

// ---------------------------------------------------------------------------------------------------------------------------------

static  void  log(const char *format, ...)
{
  // Build the buffer

  static char buffer[2048];
  va_list  ap;
  va_start(ap, format);
  vsprintf(buffer, format, ap);
  va_end(ap);

  // Cleanup the log?

  if ( cleanupLogOnFirstRun) doCleanupLogOnFirstRun();

  // Open the log file

  FILE  *FP = fopen("memalloc.log", "ab");

  // If you hit this assert, then the memory logger is unable to log information to a file (can't open the file for some
  // reason.) You can interrogate the variable 'buffer' to see what was supposed to be logged (but won't be.)
  MEM_ASSERT(FP);

  if ( !FP) return;

  // Spit out the data to the log

  fprintf(FP, "%s\r\n", buffer);
  fclose(FP);
}

// ---------------------------------------------------------------------------------------------------------------------------------

static  void  dumpAllocations(FILE *FP)
{
  fprintf(FP, "Alloc.   Addr       Size       Addr       Size                        BreakOn BreakOn              \r\n");
  fprintf(FP, "Number Reported   Reported    Actual     Actual     Unused    Method  Dealloc Realloc Allocated by \r\n");
  fprintf(FP, "------ ---------- ---------- ---------- ---------- ---------- -------- ------- ------- --------------------------------------------------- \r\n");


  for (unsigned i = 0; i < HashSize; i++)
  {
    AllocUnit *ptr = HashTable[i];
    while(ptr)
    {
      fprintf(FP, "%06d 0x%08X 0x%08X 0x%08X 0x%08X 0x%08X %-8s    %c       %c    %s\r\n",
        ptr->allocationNumber,
        (unsigned) ptr->reportedAddress, ptr->reportedSize,
        (unsigned) ptr->actualAddress, ptr->actualSize,
        calcUnused(ptr),
        AllocTypeStrs[ptr->Type],
        ptr->breakOnDealloc ? 'Y':'N',
        ptr->breakOnRealloc ? 'Y':'N',
        ownerString(ptr->SrcFile, ptr->SrcLine, ptr->SrcFunc));
      ptr = ptr->next;
    }
  }
}

// ---------------------------------------------------------------------------------------------------------------------------------

void  dumpLeakReport()
{
  // Open the report file

  FILE  *FP = fopen("memleaks.log", "w+b");

  // If you hit this assert, then the memory report generator is unable to log information to a file (can't open the file for
  // some reason.)
  MEM_ASSERT(FP);
  if ( !FP) return;

  // Any leaks?

  // Header

  static  char    TimeStr[25];
  memset(TimeStr, 0, sizeof(TimeStr));
  time_t  t = time(0);
  struct  tm *tme = localtime(&t);
  fprintf(FP, " ---------------------------------------------------------------------------------------------------------------------------------- \r\n");
  fprintf(FP, "|                                          Memory leak report for:  %02d/%02d/%04d %02d:%02d:%02d                                            |\r\n", tme->tm_mon + 1, tme->tm_mday, tme->tm_year + 1900, tme->tm_hour, tme->tm_min, tme->tm_sec);
  fprintf(FP, " ---------------------------------------------------------------------------------------------------------------------------------- \r\n");
  fprintf(FP, "\r\n");
  fprintf(FP, "\r\n");
  if ( stats.totalAllocUnitCount)
  {
    fprintf(FP, "%d memory leak%s found:\r\n", stats.totalAllocUnitCount, stats.totalAllocUnitCount == 1 ? "":"s");
  }
  else
  {
    fprintf(FP, "Congratulations! No memory leaks found!\r\n");

    // We can finally free up our own memory allocations

    if ( reservoirBuffer)
    {
      for (unsigned i = 0; i < reservoirBufferSize; i++)
      {
        free(reservoirBuffer[i]);
      }
      free(reservoirBuffer);
      reservoirBuffer = 0;
      reservoirBufferSize = 0;
      reservoir = 0;
    }
  }
  fprintf(FP, "\r\n");

  if ( stats.totalAllocUnitCount)
  {
    dumpAllocations(FP);
  }

  fclose(FP);
}


// We use a static class to let us know when we're in the midst of static deinitialization
class  MemStaticTimeTracker {
public:
  MemStaticTimeTracker() {doCleanupLogOnFirstRun();}
  ~MemStaticTimeTracker() {staticDeinitTime = true; dumpLeakReport();}
};
static  MemStaticTimeTracker  mstt;


// -DOC- Simply call this routine with the address of an allocated block of RAM, to cause it to force a breakpoint when it is
// reallocated.
bool &breakOnRealloc( void *reportedAddress )
{
  // Locate the existing allocation unit
  AllocUnit *au = findAllocUnit( reportedAddress );

  // If you hit this assert, you tried to set a breakpoint on reallocation for an address that doesn't exist. Interrogate the
  // stack frame or the variable 'au' to see which allocation this is.
  MEM_ASSERT( au );

  // If you hit this assert, you tried to set a breakpoint on reallocation for an address that wasn't allocated in a way that
  // is compatible with reallocation.
  MEM_ASSERT( (au->Type == ALLOC_MALLOC) ||
              (au->Type == ALLOC_CALLOC) ||
              (au->Type == ALLOC_REALLOC) );

  return au->breakOnRealloc;
}


//! Simply call this routine with the address of an allocated block of RAM, to cause it to force a breakpoint when it is
//! deallocated.
bool &breakOnDealloc( void *reportedAddress )
{
  // Locate the existing allocation unit

  AllocUnit  *au = findAllocUnit(reportedAddress);

  // If you hit this assert, you tried to set a breakpoint on deallocation for an address that doesn't exist. Interrogate the
  // stack frame or the variable 'au' to see which allocation this is.
  MEM_ASSERT(au != 0);

  return au->breakOnDealloc;
}


//! When tracking down a difficult bug, use this routine to force a breakpoint on a specific allocation count
void breakOnAllocation( unsigned Count )
{
  breakOnAllocationCount = Count;
}


void MemTrace::setOwner( const char *File, unsigned Line, const char *Func )
{
  SrcFile = File;
  SrcLine = Line;
  SrcFunc = Func;
}

// ---------------------------------------------------------------------------------------------------------------------------------
// Global new/new[]
//
// These are the standard new/new[] operators. They are merely interface functions that operate like normal new/new[], but use our
// memory tracking routines.
// ---------------------------------------------------------------------------------------------------------------------------------

void *operator new( size_t reportedSize )
{
  // ANSI says: allocation requests of 0 bytes will still return a valid value
  if ( !reportedSize )
    reportedSize = 1;

  // ANSI says: loop continuously because the error handler could possibly free up some memory
  for( ; ; ) {
    // Try the allocation
    void *ptr = allocator( SrcFile, SrcLine, SrcFunc, ALLOC_NEW, reportedSize );
    if ( ptr )
      return ptr;

    // There isn't a way to determine the new handler, except through setting it. So we'll just set it to NULL, then
    // set it back again.
    new_handler nh = set_new_handler( 0 );
    set_new_handler( nh );

    // If there is an error handler, call it.  Otherwise, throw the exception
    if ( nh )
      (*nh)();
    else
      throw std::bad_alloc();
  }
}


void *operator new[]( size_t reportedSize )
{
  // The ANSI standard says that allocation requests of 0 bytes will still return a valid value
  if ( !reportedSize )
    reportedSize = 1;

  // ANSI says: loop continuously because the error handler could possibly free up some memory
  for ( ; ; ) {
    // Try the allocation
    void  *ptr = allocator( SrcFile, SrcLine, SrcFunc, ALLOC_NEW_ARRAY, reportedSize );
    if ( ptr )
      return ptr;

    // There isn't a way to determine the new handler, except through setting it. So we'll just set it to NULL, then
    // set it back again.
    new_handler nh = set_new_handler( 0 );
    set_new_handler( nh );

    // If there is an error handler, call it.  Otherwise, throw the exception
    if ( nh)
      (*nh)();
    else
      throw std::bad_alloc();
  }
}


// ---------------------------------------------------------------------------------------------------------------------------------
// Other global new/new[]
//
// These are the standard new/new[] operators as used by Microsoft's memory tracker. We don't want them interfering with our memory
// tracking efforts. Like the previous versions, these are merely interface functions that operate like normal new/new[], but use
// our memory tracking routines.
// ---------------------------------------------------------------------------------------------------------------------------------

void *operator new( size_t reportedSize, const char *SrcFile, int SrcLine )
{
  // The ANSI standard says that allocation requests of 0 bytes will still return a valid value
  if ( !reportedSize )
    reportedSize = 1;

  // ANSI says: loop continuously because the error handler could possibly free up some memory
  for( ; ; ) {
    // Try the allocation
    void  *ptr = allocator( SrcFile, SrcLine, "??", ALLOC_NEW, reportedSize );
    if ( ptr )
      return ptr;

    // There isn't a way to determine the new handler, except through setting it. So we'll just set it to NULL, then
    // set it back again.
    new_handler nh = set_new_handler( 0 );
    set_new_handler( nh );

    // If there is an error handler, call it.  Otherwise, throw the exception
    if ( nh )
      (*nh)();
    else
      throw std::bad_alloc();
  }
}


void *operator new[]( size_t reportedSize, const char *SrcFile, int SrcLine )
{
  // The ANSI standard says that allocation requests of 0 bytes will still return a valid value
  if ( !reportedSize )
    reportedSize = 1;

  // ANSI says: loop continuously because the error handler could possibly free up some memory
  for ( ; ; ) {
    // Try the allocation
    void  *ptr = allocator( SrcFile, SrcLine, "??", ALLOC_NEW_ARRAY, reportedSize );
    if ( ptr )
      return ptr;

    // There isn't a way to determine the new handler, except through setting it. So we'll just set it to NULL, then
    // set it back again.
    new_handler  nh = set_new_handler( 0 );
    set_new_handler( nh );

    // If there is an error handler, call it.  Otherwise, throw the exception
    if ( nh )
      (*nh)();
    else
      throw std::bad_alloc();
  }
}


// Global delete/delete[]
//
// These are the standard delete/delete[] operators. They are merely interface functions that operate like normal delete/delete[],
// but use our memory tracking routines.
void operator delete( void *reportedAddress )
{
  // ANSI says: delete & delete[] allow NULL pointers (they do nothing)
  if ( !reportedAddress )
    return;

  deallocator( SrcFile, SrcLine, SrcFunc, ALLOC_DELETE, reportedAddress );
}


void operator delete[]( void *reportedAddress )
{
  // ANSI says: delete & delete[] allow NULL pointers (they do nothing)
  if ( !reportedAddress )
    return;

  deallocator( SrcFile, SrcLine, SrcFunc, ALLOC_DELETE_ARRAY, reportedAddress );
}


// Allocate memory and track it
void *MemTrace::allocator( const char *SrcFile, unsigned SrcLine, const char *SrcFunc, AllocType Type, size_t reportedSize )
{
  try {
    // Increase our allocation count
    currentAllocationCount++;

    // Log the request
    if ( AlwaysLogAll )
      log( "%05d %-40s %8s            : %s", currentAllocationCount, ownerString( SrcFile, SrcLine, SrcFunc ), AllocTypeStrs[Type], MemSizeStr( reportedSize ) );

    // If you hit this assert, you requested a breakpoint on a specific allocation count
    MEM_ASSERT( currentAllocationCount != breakOnAllocationCount );

    // If necessary, grow the reservoir of unused allocation units
    if ( !reservoir ) {
      // Allocate 256 reservoir elements
      reservoir = (AllocUnit *) malloc( sizeof( AllocUnit ) * 256 );

      // If you hit this assert, then the memory manager failed to allocate internal memory for tracking the
      // allocations
      MEM_ASSERT( reservoir );

      // Danger Will Robinson!
      if ( !reservoir )
        throw "Unable to allocate RAM for internal memory tracking data";

      // Build a linked-list of the elements in our reservoir
      memset( reservoir, 0, sizeof( AllocUnit ) * 256 );
      for ( unsigned i = 0; i < 256 - 1; ++i )
        reservoir[i].next = &reservoir[i + 1];

      // Add this address to our reservoirBuffer so we can free it later
      AllocUnit **temp = (AllocUnit **) realloc( reservoirBuffer, (reservoirBufferSize + 1) * sizeof( AllocUnit * ) );
      MEM_ASSERT( temp );
      if ( temp ) {
        reservoirBuffer = temp;
        reservoirBuffer[reservoirBufferSize++] = reservoir;
      }
    }

    // Logical flow says this should never happen...
    MEM_ASSERT( reservoir );

    // Grab a new allocaton unit from the front of the reservoir
    AllocUnit *au = reservoir;
    reservoir = au->next;

    // Populate it with some real data
    memset( au, 0, sizeof( AllocUnit ) );
    au->actualSize = calculateActualSize( reportedSize );

#ifdef RANDOM_FAILURE
    double  a = rand();
    double  b = RAND_MAX / 100.0 * RANDOM_FAILURE;
    if ( a > b )
      au->actualAddress = malloc( au->actualSize );
    else {
      log( "!Random faiure!" );
      au->actualAddress = 0;
    }
#else
    au->actualAddress = malloc( au->actualSize );
#endif

    au->reportedSize = reportedSize;
    au->reportedAddress = calculateReportedAddress( au->actualAddress );
    au->Type = Type;
    au->SrcLine = SrcLine;
    au->allocationNumber = currentAllocationCount;
    if ( SrcFile )
      strncpy( au->SrcFile, sourceFileStripper( SrcFile ), sizeof( au->SrcFile ) - 1 );
    else
      strcpy( au->SrcFile, "??" );

    if ( SrcFunc )
      strncpy( au->SrcFunc, SrcFunc, sizeof( au->SrcFunc ) - 1 );
    else
      strcpy( au->SrcFunc, "??" );

    // We don't want to assert with random failures, because we want the application to deal with them.
#ifndef RANDOM_FAILURE
    // If you hit this assert, then the requested allocation simply failed (you're out of memory.) Interrogate the
    // variable 'au' or the stack frame to see what you were trying to do.
    MEM_ASSERT( au->actualAddress );
#endif

    if ( !au->actualAddress )
      throw "Request for allocation failed. Out of memory.";

    // If you hit this assert, then this allocation was made from a source that isn't setup to use this memory tracking
    // software, use the stack frame to locate the source and include our H file.
    MEM_ASSERT( Type != ALLOC_UNKNOWN );

    // Insert the new allocation into the hash table
    unsigned HashIndex = ((unsigned) au->reportedAddress >> 4) & (HashSize - 1);
    if ( HashTable[HashIndex] )
      HashTable[HashIndex]->prev = au;

    au->next = HashTable[HashIndex];
    au->prev = 0;
    HashTable[HashIndex] = au;

    // Account for the new allocatin unit in our stats
    stats.totalReportedMemory += au->reportedSize;
    stats.totalActualMemory   += au->actualSize;
    stats.totalAllocUnitCount++;
    if ( stats.totalReportedMemory > stats.peakReportedMemory ) stats.peakReportedMemory = stats.totalReportedMemory;
    if ( stats.totalActualMemory   > stats.peakActualMemory )   stats.peakActualMemory   = stats.totalActualMemory;
    if ( stats.totalAllocUnitCount > stats.peakAllocUnitCount ) stats.peakAllocUnitCount = stats.totalAllocUnitCount;
    stats.accumulatedReportedMemory += au->reportedSize;
    stats.accumulatedActualMemory += au->actualSize;
    stats.accumulatedAllocUnitCount++;

    // Prepare the allocation unit for use (wipe it with recognizable garbage)
    wipeWithPattern( au, unusedPattern );

    // calloc() expects the reported memory address range to be filled with 0's
    if ( Type == ALLOC_CALLOC )
      memset( au->reportedAddress, 0, au->reportedSize );

    // Validate every single allocated unit in memory
    if ( AlwaysValidateAll )
      validateAllAllocUnits();

    // Log the result
    if ( AlwaysLogAll )
      log( "                                                                 OK: %010p (hash: %d)", au->reportedAddress, HashIndex );

    // Resetting the globals insures that if at some later time, somebody calls our memory manager from an unknown
    // source (i.e. they didn't include our H file) then we won't think it was the last allocation.
    resetGlobals();

    // Return the (reported) address of the new allocation unit

    return au->reportedAddress;
  }

  catch ( const char *err ) {
    // Deal with the errors
    log( err );
    resetGlobals();
    return( 0 );
  }
}


//! Reallocate memory and track it
void *MemTrace::reallocator( const char *SrcFile, unsigned SrcLine, const char *SrcFunc, AllocType Type, const size_t reportedSize, void *reportedAddress )
{
  try {
    // Calling realloc with a NULL should force same operations as a malloc
    if ( !reportedAddress )
      return MemTrace::allocator( SrcFile, SrcLine, SrcFunc, Type, reportedSize );

    // Increase our allocation count
    currentAllocationCount++;

    // If you hit this assert, you requested a breakpoint on a specific allocation count
    MEM_ASSERT( currentAllocationCount != breakOnAllocationCount );

    // Log the request
    if ( AlwaysLogAll )
      log( "%05d %-40s %8s(%010p): %s", currentAllocationCount, ownerString( SrcFile, SrcLine, SrcFunc ), AllocTypeStrs[Type], reportedAddress, MemSizeStr( reportedSize ) );

    // Locate the existing allocation unit
    AllocUnit *au = findAllocUnit( reportedAddress );

    // If you hit this assert, you tried to reallocate RAM that wasn't allocated by this memory manager.
    MEM_ASSERT( au );
    if ( !au )
      throw "Request to reallocate RAM that was never allocated";

    // If you hit this assert, then the allocation unit that is about to be reallocated is damaged. But you probably
    // already know that from a previous assert you should have seen in validateAllocUnit() :)
    MEM_ASSERT( validateAllocUnit( au ) );

    // If you hit this assert, then this reallocation was made from a source that isn't setup to use this memory
    // tracking software, use the stack frame to locate the source and include our H file.
    MEM_ASSERT( Type != ALLOC_UNKNOWN );

    // If you hit this assert, you were trying to reallocate RAM that was not allocated in a way that is compatible with
    // realloc. In other words, you have a allocation/reallocation mismatch.
    MEM_ASSERT( (au->Type == ALLOC_MALLOC) || (au->Type == ALLOC_CALLOC) || (au->Type == ALLOC_REALLOC) );

    // If you hit this assert, then the "break on realloc" flag for this allocation unit is set (and will continue to be
    // set until you specifically shut it off. Interrogate the 'au' variable to determine information about this
    // allocation unit.
    MEM_ASSERT( !au->breakOnRealloc );

    // Keep track of the original size
    unsigned originalReportedSize = au->reportedSize;

    // Do the reallocation
    void  *oldReportedAddress = reportedAddress;
    size_t newActualSize = calculateActualSize( reportedSize );
    void  *newActualAddress = 0;
#ifdef RANDOM_FAILURE
    double  a = rand();
    double  b = RAND_MAX / 100.0 * RANDOM_FAILURE;
    if ( a > b)
      newActualAddress = realloc( au->actualAddress, newActualSize );
    else
      log( "!Random faiure!" );
#else
    newActualAddress = realloc( au->actualAddress, newActualSize );
#endif

    // We don't want to assert with random failures, because we want the application to deal with them.
#ifndef RANDOM_FAILURE
    // If you hit this assert, then the requested allocation simply failed (you're out of memory) Interrogate the
    // variable 'au' to see the original allocation. You can also query 'newActualSize' to see the amount of memory
    // trying to be allocated. Finally, you can query 'reportedSize' to see how much memory was requested by the caller.
    MEM_ASSERT( newActualAddress );
#endif

    if ( !newActualAddress )
      throw "Request for reallocation failed. Out of memory.";

    // Remove this allocation from our stats (we'll add the new reallocation again later)
    stats.totalReportedMemory -= au->reportedSize;
    stats.totalActualMemory   -= au->actualSize;

    // Update the allocation with the new information
    au->actualSize        = newActualSize;
    au->actualAddress     = newActualAddress;
    au->reportedSize      = calculateReportedSize( newActualSize );
    au->reportedAddress   = calculateReportedAddress( newActualAddress );
    au->Type = Type;
    au->SrcLine = SrcLine;
    au->allocationNumber  = currentAllocationCount;
    if ( SrcFile )
      strncpy( au->SrcFile, sourceFileStripper( SrcFile ), sizeof( au->SrcFile ) - 1 );
    else
      strcpy( au->SrcFile, "??" );

    if ( SrcFunc )
      strncpy( au->SrcFunc, SrcFunc, sizeof( au->SrcFunc ) - 1 );
    else
      strcpy( au->SrcFunc, "??" );

    // The reallocation may cause the address to change, so we should relocate our allocation unit within the hash table
    unsigned HashIndex = (unsigned) -1;
    if ( oldReportedAddress != au->reportedAddress ) {
      // Remove this allocation unit from the hash table

      {
        unsigned HashIndex = ((unsigned) oldReportedAddress >> 4) & (HashSize - 1);
        if ( HashTable[HashIndex] == au )
          HashTable[HashIndex] = HashTable[HashIndex]->next;
        else {
          if ( au->prev )
            au->prev->next = au->next;

          if ( au->next )
            au->next->prev = au->prev;
        }
      }

      // Re-insert it back into the hash table
      HashIndex = ((unsigned) au->reportedAddress >> 4) & (HashSize - 1);
      if ( HashTable[HashIndex] )
        HashTable[HashIndex]->prev = au;

      au->next = HashTable[HashIndex];
      au->prev = 0;
      HashTable[HashIndex] = au;
    }

    // Account for the new allocation unit in our stats
    stats.totalReportedMemory += au->reportedSize;
    stats.totalActualMemory   += au->actualSize;
    if ( stats.totalReportedMemory > stats.peakReportedMemory ) stats.peakReportedMemory = stats.totalReportedMemory;
    if ( stats.totalActualMemory   > stats.peakActualMemory )   stats.peakActualMemory   = stats.totalActualMemory;

    int  deltaReportedSize = reportedSize - originalReportedSize;
    if ( deltaReportedSize > 0 ) {
      stats.accumulatedReportedMemory += deltaReportedSize;
      stats.accumulatedActualMemory += deltaReportedSize;
    }

    // Prepare the allocation unit for use (wipe it with recognizable garbage)
    wipeWithPattern( au, unusedPattern, originalReportedSize );

    // If you hit this assert, then something went wrong, because the allocation unit was properly validated PRIOR to
    // the reallocation. This should not happen.
    MEM_ASSERT( validateAllocUnit( au ) );

    // Validate every single allocated unit in memory
    if ( AlwaysValidateAll )
      validateAllAllocUnits();

    // Log the result
    if ( AlwaysLogAll )
      log( "                                                                 OK: %010p (hash: %d)", au->reportedAddress, HashIndex );

    // Resetting the globals insures that if at some later time, somebody calls our memory manager from an unknown
    // source (i.e. they didn't include our H file) then we won't think it was the last allocation.
    resetGlobals();

    // Return the (reported) address of the new allocation unit
    return au->reportedAddress;
  }

  catch ( const char *err ) {
    // Deal with the errors
    log( err );
    resetGlobals();
    return( 0 );
  }
}


//! Deallocate memory and track it
void MemTrace::deallocator( const char *SrcFile, unsigned SrcLine, const char *SrcFunc, AllocType Type, const void *reportedAddress)
{
  try {
    // Log the request

    if ( AlwaysLogAll )
      log( "      %-40s %8s(%010p)", ownerString( SrcFile, SrcLine, SrcFunc ), AllocTypeStrs[Type], reportedAddress );

    // Go get the allocation unit
    AllocUnit *au = findAllocUnit( reportedAddress );

    // If you hit this assert, you tried to deallocate RAM that wasn't allocated by this memory manager.
    MEM_ASSERT( au );
    if ( !au )
      throw "Request to deallocate RAM that was never allocated";

    // If you hit this assert, then the allocation unit that is about to be deallocated is damaged. But you probably
    // already know that from a previous assert you should have seen in validateAllocUnit() :)
    MEM_ASSERT( validateAllocUnit( au ) );

    // If you hit this assert, then this deallocation was made from a source that isn't setup to use this memory
    // tracking software, use the stack frame to locate the source and include our H file.
    MEM_ASSERT( Type != ALLOC_UNKNOWN );

    // If you hit this assert, you were trying to deallocate RAM that was not allocated in a way that is compatible with
    // the deallocation method requested. In other words, you have a allocation/deallocation mismatch.
    MEM_ASSERT( ((au->Type == ALLOC_NEW)       && (Type == ALLOC_DELETE)) ||
                ((au->Type == ALLOC_NEW_ARRAY) && (Type == ALLOC_DELETE_ARRAY)) ||
                ((au->Type == ALLOC_MALLOC)    && (Type == ALLOC_FREE)) ||
                ((au->Type == ALLOC_CALLOC)    && (Type == ALLOC_FREE)) ||
                ((au->Type == ALLOC_REALLOC)   && (Type == ALLOC_FREE)) ||
                (Type == ALLOC_UNKNOWN) );

    // If you hit this assert, then the "break on dealloc" flag for this allocation unit is set. Interrogate the 'au'
    // variable to determine information about this allocation unit.
    MEM_ASSERT( !au->breakOnDealloc );

    // Wipe the deallocated RAM with a new pattern. This doen't actually do us much good in debug mode under WIN32,
    // because Microsoft's memory debugging & tracking utilities will wipe it right after we do. Oh well.
    wipeWithPattern( au, releasedPattern );

    // Do the deallocation
    free( au->actualAddress );

    // Remove this allocation unit from the hash table
    unsigned HashIndex = ((unsigned) au->reportedAddress >> 4) & (HashSize - 1);
    if ( HashTable[HashIndex] == au )
      HashTable[HashIndex] = au->next;
    else {
      if ( au->prev )
        au->prev->next = au->next;

      if ( au->next )
        au->next->prev = au->prev;
    }

    // Remove this allocation from our stats
    stats.totalReportedMemory -= au->reportedSize;
    stats.totalActualMemory   -= au->actualSize;
    stats.totalAllocUnitCount--;

    // Add this allocation unit to the front of our reservoir of unused allocation units
    memset( au, 0, sizeof( AllocUnit ) );
    au->next = reservoir;
    reservoir = au;

    // Resetting the globals insures that if at some later time, somebody calls our memory manager from an unknown
    // source (i.e. they didn't include our H file) then we won't think it was the last allocation.
    resetGlobals();

    // Validate every single allocated unit in memory
    if ( AlwaysValidateAll )
      validateAllAllocUnits();

    // If we're in the midst of static deinitialization time, track any pending memory leaks
    if ( staticDeinitTime )
      dumpLeakReport();
  }

  catch( const char *err ) {
    // Deal with errors
    log( err );
    resetGlobals();
  }
}


// The following utilitarian allow you to become proactive in tracking your own memory, or help you narrow in on those tough bugs.
inline bool validateAddress( const void *reportedAddress )
{
  // Just see if the address exists in our allocation routines
  return findAllocUnit( reportedAddress ) != 0;
}


bool validateAllocUnit( const AllocUnit *Alloc )
{
  // Make sure the padding is untouched
  long *pre = (long *) Alloc->actualAddress;
  long *post = (long *) ((char *) Alloc->actualAddress + Alloc->actualSize - paddingSize * sizeof( long ));
  bool Err = false;

  for ( unsigned i = 0; i < paddingSize; ++i, ++pre, ++post )
  {
    if ( *pre != (long) prefixPattern ) {
      log( "A memory allocation unit was corrupt because of an underrun:" );
      DumpAllocUnit( Alloc, "  " );
      Err = true;
    }

    // If you hit this assert, then you should know that this allocation unit has been damaged. Something (possibly the
    // owner?) has underrun the allocation unit (modified a few bytes prior to the start). You can interrogate the
    // variable 'Alloc' to see statistics and information about this damaged allocation unit.
    MEM_ASSERT( *pre == (long) prefixPattern );

    if ( *post != (long) postfixPattern ) {
      log( "A memory allocation unit was corrupt because of an overrun:" );
      DumpAllocUnit( Alloc, "  " );
      Err = true;
    }

    // If you hit this assert, then you should know that this allocation unit has been damaged. Something (possibly the
    // owner?) has overrun the allocation unit (modified a few bytes after the end). You can interrogate the variable
    // 'Alloc' to see statistics and information about this damaged allocation unit.
    MEM_ASSERT( *post == (long) postfixPattern );
  }

  // Return the error status (we invert it, because a return of 'false' means error)
  return !Err;
}


bool validateAllAllocUnits( void )
{
  // Just go through each allocation unit in the hash table and count the ones that have errors
  unsigned Errors = 0;
  unsigned AllocCount = 0;

  for ( unsigned i = 0; i < HashSize; ++i ) {
    AllocUnit *Alloc = HashTable[i];
    while( Alloc ) {
      AllocCount++;
      if ( !validateAllocUnit( Alloc ) )
        Errors++;

      Alloc = Alloc->next;
    }
  }

  // Test for hash-table correctness
  if ( AllocCount != stats.totalAllocUnitCount ) {
    log( "Memory tracking hash table corrupt!" );
    Errors++;
  }

  // If you hit this assert, then the internal memory (hash table) used by this memory tracking software is damaged! The
  // best way to track this down is to use the AlwaysLogAll flag in conjunction with STRESS_TEST macro to narrow in on the
  // offending code. After running the application with these settings (and hitting this assert again), interrogate the
  // memory.log file to find the previous successful operation. The corruption will have occurred between that point and this
  // assertion.
  MEM_ASSERT( allocCount == stats.totalAllocUnitCount );

  // If you hit this assert, then you've probably already been notified that there was a problem with a allocation unit in a
  // prior call to validateAllocUnit(), but this assert is here just to make sure you know about it. :)
  MEM_ASSERT( !Errors );

  // Log any errors
  if ( Errors )
    log( "While validting all allocation units, %d allocation unit(s) were found to have problems", Errors );

  // Return the error status
  return Errors != 0;
}


// Unused RAM calculation routines. Use these to determine how much of your RAM is unused (in bytes)
unsigned calcUnused( AllocUnit const *Alloc )
{
  const unsigned long *ptr = (const unsigned long *) Alloc->reportedAddress;
  unsigned Count = 0;

  for ( unsigned i = 0; i < Alloc->reportedSize; i += sizeof( long ), ++ptr )
    if ( *ptr == unusedPattern )
      Count += sizeof( long );

  return Count;
}


unsigned calcAllUnused( void )
{
  // Just go through each allocation unit in the hash table and count the unused RAM
  unsigned Total = 0;

  for ( unsigned i = 0; i < HashSize; ++i ) {
    for ( AllocUnit *ptr = HashTable[i]; ptr; ptr = ptr->next )
      Total += calcUnused( ptr );
  }

  return Total;
}


void DumpAllocUnit( AllocUnit const *Unit, const char *Prefix )
{
  log( "%sAddress (reported): %010p",       Prefix, Unit->reportedAddress );
  log( "%sAddress (actual)  : %010p",       Prefix, Unit->actualAddress );
  log( "%sSize (reported)   : 0x%08X (%s)", Prefix, Unit->reportedSize, MemSizeStr( Unit->reportedSize ) );
  log( "%sSize (actual)     : 0x%08X (%s)", Prefix, Unit->actualSize, MemSizeStr( Unit->actualSize ) );
  log( "%sOwner             : %s(%d)::%s",  Prefix, Unit->SrcFile, Unit->SrcLine, Unit->SrcFunc );
  log( "%sAllocation type   : %s",          Prefix, AllocTypeStrs[Unit->Type] );
  log( "%sAllocation number : %d",          Prefix, Unit->allocationNumber );
}


void dumpMemReport( const char *FileName, const bool Overwrite )
{
  // Open the report file
  FILE *FP = fopen( FileName, Overwrite ? "w+b" : "ab" );

  // If you hit this assert, then the memory report generator is unable to log information to a file (can't open the file for
  // some reason.)
  MEM_ASSERT( FP );
  if ( !FP )
    return;

  // Header
  static char TimeStr[25];
  memset( TimeStr, 0, sizeof( TimeStr ) );
  time_t t = time(0);
  struct tm *tme = localtime(&t);

  fprintf( FP, " ---------------------------------------------------------------------------------------------------------------------------------- \r\n");
  fprintf( FP, "|                                             Memory report for: %02d/%02d/%04d %02d:%02d:%02d                                               |\r\n", tme->tm_mon + 1, tme->tm_mday, tme->tm_year + 1900, tme->tm_hour, tme->tm_min, tme->tm_sec);
  fprintf( FP, " ---------------------------------------------------------------------------------------------------------------------------------- \r\n");
  fprintf( FP, "\r\n" );
  fprintf( FP, "\r\n" );

  // Report summary
  fprintf( FP, " ---------------------------------------------------------------------------------------------------------------------------------- \r\n");
  fprintf( FP, "|                                                           T O T A L S                                                            |\r\n");
  fprintf( FP, " ---------------------------------------------------------------------------------------------------------------------------------- \r\n");
  fprintf( FP, "              Allocation unit count: %10s\r\n", insertCommas(stats.totalAllocUnitCount));
  fprintf( FP, "            Reported to application: %s\r\n", MemSizeStr(stats.totalReportedMemory));
  fprintf( FP, "         Actual total memory in use: %s\r\n", MemSizeStr(stats.totalActualMemory));
  fprintf( FP, "           Memory tracking overhead: %s\r\n", MemSizeStr(stats.totalActualMemory - stats.totalReportedMemory));
  fprintf( FP, "\r\n");

  fprintf( FP, " ---------------------------------------------------------------------------------------------------------------------------------- \r\n");
  fprintf( FP, "|                                                            P E A K S                                                             |\r\n");
  fprintf( FP, " ---------------------------------------------------------------------------------------------------------------------------------- \r\n");
  fprintf( FP, "              Allocation unit count: %10s\r\n", insertCommas(stats.peakAllocUnitCount));
  fprintf( FP, "            Reported to application: %s\r\n", MemSizeStr(stats.peakReportedMemory));
  fprintf( FP, "                             Actual: %s\r\n", MemSizeStr(stats.peakActualMemory));
  fprintf( FP, "           Memory tracking overhead: %s\r\n", MemSizeStr(stats.peakActualMemory - stats.peakReportedMemory));
  fprintf( FP, "\r\n");

  fprintf( FP, " ---------------------------------------------------------------------------------------------------------------------------------- \r\n");
  fprintf( FP, "|                                                      A C C U M U L A T E D                                                       |\r\n");
  fprintf( FP, " ---------------------------------------------------------------------------------------------------------------------------------- \r\n");
  fprintf( FP, "              Allocation unit count: %s\r\n", MemSizeStr(stats.accumulatedAllocUnitCount));
  fprintf( FP, "            Reported to application: %s\r\n", MemSizeStr(stats.accumulatedReportedMemory));
  fprintf( FP, "                             Actual: %s\r\n", MemSizeStr(stats.accumulatedActualMemory));
  fprintf( FP, "\r\n");

  fprintf( FP, " ---------------------------------------------------------------------------------------------------------------------------------- \r\n");
  fprintf( FP, "|                                                           U N U S E D                                                            |\r\n");
  fprintf( FP, " ---------------------------------------------------------------------------------------------------------------------------------- \r\n");
  fprintf( FP, "    Memory allocated but not in use: %s\r\n", MemSizeStr( calcAllUnused() ) );
  fprintf( FP, "\r\n");

  dumpAllocations( FP );

  fclose( FP );
}


MemStats getMemStats( void )
{
  return stats;
}

#endif

