METASOURCES = AUTO
SUBDIRS = XML test
INCLUDES = -I$(top_srcdir) $(PTHREAD_CFLAGS) $(expat_CFLAGS) $(libpcre_CFLAGS) $(openssl_CFLAGS) $(uuid_CFLAGS) $(z_CFLAGS)
AM_CXXFLAGS = -Wall

lib_LTLIBRARIES = libFinagle.la

libFinagle_la_SOURCES = AppLog.cpp AppLoop.cpp Compress.cpp DateTime.cpp \
	DateTimeMask.cpp Dir.cpp File.cpp FileDescWatcher.cpp FilePath.cpp IPAddress.cpp MD5.cpp \
	MemTrace.cpp Mutex.cpp OptionParser.cpp PriorityMutex.cpp Rate.cpp Rectangle.cpp \
	RegEx.cpp SSL.cpp Socket.cpp StreamIO.cpp TextString.cpp Thread.cpp Timer.cpp \
	UUID.cpp Util.cpp WaitCondition.cpp

libFinagle_la_LDFLAGS = -no-undefined -version-info @LIB_CURRENT@:@LIB_REVISION@:@LIB_AGE@ -release @FINAGLE_VERSION@
libFinagle_la_LIBADD = $(top_builddir)/Finagle/XML/libXML.la $(PTHREAD_LIBS) \
	$(expat_LIBS) $(libpcre_LIBS) $(openssl_LIBS) $(uuid_LIBS) $(z_LIBS)

library_includedir=$(includedir)/$(PACKAGE)-$(VERSION)/Finagle
library_include_HEADERS = AppLog.h AppLogEntry.h AppLoop.h Array.h ByteArray.h \
	ByteOrder.h Compress.h DataStream.h DateTime.h DateTimeMask.h Dir.h Exception.h \
	Factory.h File.h FileDescWatcher.h FilePath.h FileSystem.h Finagle.h \
	GarbageCollector.h IPAddress.h InetSocket.h Initializer.h List.h MD5.h Map.h MapIterator.h \
	MemTrace.h MultiMap.h Mutex.h ObjectPtr.h OptionParser.h OrderedMap.h PThreadEx.h \
	PriorityMutex.h PriorityQueue.h Property.h Queue.h Range.h Rate.h Rectangle.h \
	ReferenceCount.h RegEx.h SSL.h ServerSocket.h Set.h Singleton.h SizedQueue.h Socket.h \
	StreamIO.h TextString.h Thread.h ThreadFunc.h Timer.h UUID.h UnixSocket.h Util.h \
	WaitCondition.h

doc: Doxyfile $(DIST_SOURCES)
	@rm -rf doc
	@doxygen Doxyfile >/dev/null

docdir ?= $(datadir)/doc/$(PACKAGE)
htmldir = $(docdir)/Finagle
html_DATA = $(wildcard doc/*)
EXTRA_DIST = doc

