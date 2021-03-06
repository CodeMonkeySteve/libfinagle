METASOURCES = AUTO
INCLUDES = -I$(top_srcdir) 
SUBDIRS = XML Net test

lib_LTLIBRARIES = libFinagle.la

libFinagle_CPPFLAGS = $(BOOST_BIND) $(PTHREAD_CFLAGS) $(expat_CFLAGS) $(pcre_CFLAGS) $(openssl_CFLAGS) $(uuid_CFLAGS) $(z_CFLAGS)
libFinagle_CXXFLAGS = -Wall

libFinagle_la_SOURCES = AppLog.cpp AppLoop.cpp Compress.cpp DateTime.cpp \
	DateTimeMask.cpp Dir.cpp File.cpp FileDescWatcher.cpp FilePath.cpp MD5.cpp \
	MemTrace.cpp Mutex.cpp OptionParser.cpp PriorityMutex.cpp Rectangle.cpp RegEx.cpp \
	SSL.cpp StreamIO.cpp TextString.cpp Thread.cpp Timer.cpp UUID.cpp \
	Util.cpp Velocimeter.cpp WaitCondition.cpp

libFinagle_la_LDFLAGS = -no-undefined -version-info @LIB_CURRENT@:@LIB_REVISION@:@LIB_AGE@ -release @FINAGLE_VERSION@
libFinagle_la_LIBADD = $(top_builddir)/Finagle/XML/libXML.la \
	$(top_builddir)/Finagle/Net/libNet.la $(BOOST_SIGNALS_LIBS) $(PTHREAD_LIBS) $(expat_LIBS) $(curl_LIBS) $(pcre_LIBS) $(openssl_LIBS) \
	$(uuid_LIBS) $(z_LIBS)

library_includedir=$(includedir)/$(PACKAGE)-$(VERSION)/Finagle
library_include_HEADERS = AppLog.h AppLogEntry.h AppLoop.h Array.h ByteArray.h \
	ByteOrder.h Compress.h DataStream.h DateTime.h DateTimeMask.h Dir.h Exception.h \
	Factory.h File.h FileDescWatcher.h FilePath.h FileSystem.h Finagle.h \
	GarbageCollector.h Initializer.h List.h MD5.h Map.h MapIterator.h \
	MemTrace.h MultiMap.h Mutex.h ObjectPtr.h OptionParser.h OrderedMap.h PThreadEx.h \
	PriorityMutex.h PriorityQueue.h Property.h Queue.h Range.h Rectangle.h ReferenceCount.h \
	RegEx.h SSL.h Set.h Singleton.h SizedQueue.h StreamIO.h \
	TextString.h Thread.h ThreadFunc.h Timer.h UUID.h Util.h Velocimeter.h \
	WaitCondition.h

doc: Doxyfile $(DIST_SOURCES)
	@rm -rf doc
	@doxygen Doxyfile >/dev/null

docdir ?= $(datadir)/doc/$(PACKAGE)
htmldir = $(docdir)/Finagle
html_DATA = $(wildcard doc/*)
EXTRA_DIST = doc

