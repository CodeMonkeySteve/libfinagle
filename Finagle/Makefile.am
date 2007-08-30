METASOURCES = AUTO
lib_LTLIBRARIES = libFinagle.la

libFinagle_la_SOURCES = AppLog.cpp AppLog.h AppLogEntry.h AppLoop.cpp AppLoop.h \
	Array.h ByteArray.h ByteOrder.h Collection.h Compress.cpp Compress.h \
	Configure.cpp Configure.h Constructable.h DataStream.h DateTime.cpp DateTime.h \
	DateTimeMask.cpp DateTimeMask.h Dir.cpp Dir.h Exception.h File.cpp File.h \
	FileDescWatcher.cpp FileDescWatcher.h FilePath.cpp FilePath.h FileSystem.h Finagle.h \
	GarbageCollector.h IPAddress.cpp IPAddress.h InetSocket.h List.h MD5.cpp MD5.h Map.h \
	MapIterator.h MemTrace.cpp MemTrace.h MultiMap.h Mutex.cpp Mutex.h OptionParser.cpp \
	OptionParser.h OrderedMap.h PThreadEx.h PriorityMutex.cpp PriorityMutex.h Property.h \
	Queue.h Range.h Rate.cpp Rate.h Rectangle.cpp Rectangle.h ReferencedObject.h \
	RegEx.h SSL.cpp SSL.h Schedule.cpp Schedule.h ServerSocket.h Set.h Singleton.h \
	Socket.cpp Socket.h Spool.cpp StreamIO.cpp StreamIO.h TextString.cpp TextString.h \
	Thread.cpp Thread.h ThreadFunc.h Timer.cpp Timer.h UUID.h UnixSocket.h Util.cpp \
	Util.h WaitCondition.cpp WaitCondition.h XML.cpp XML.h XMLParse.cpp XMLParse.h \
	sigslot.h

libFinagle_la_LDFLAGS = -no-undefined -version-info @LIB_CURRENT@:@LIB_REVISION@:@LIB_AGE@ -release @FINAGLE_VERSION@
libFinagle_la_LIBADD = $(PTHREAD_LIBS) $(z_LIBS) $(libpcre_LIBS) $(expat_LIBS) $(openssl_LIBS) $(uuid_LIBS)
INCLUDES = -I$(top_srcdir)/. $(PTHREAD_CFLAGS) $(expat_CFLAGS) \
	$(libpcre_CFLAGS) $(openssl_CFLAGS) $(uuid_CFLAGS) $(z_CFLAGS)

library_includedir=$(includedir)/$(PACKAGE)-$(VERSION)/Finagle
library_include_HEADERS = AppLog.h AppLogEntry.h AppLoop.h Array.h ByteArray.h \
	ByteOrder.h Collection.h Compress.h Configure.h Constructable.h DataStream.h \
	DateTime.h DateTimeMask.h Dir.h Exception.h File.h FileDescWatcher.h FilePath.h \
	FileSystem.h Finagle.h GarbageCollector.h IPAddress.h InetSocket.h List.h MD5.h \
	Map.h MapIterator.h MemTrace.h MultiMap.h Mutex.h OptionParser.h OrderedMap.h \
	PThreadEx.h PriorityMutex.h Property.h Queue.h Range.h Rate.h Rectangle.h \
	ReferencedObject.h RegEx.h SSL.h Schedule.h ServerSocket.h Set.h Singleton.h SizedQueue.h \
	Socket.h SortedQueue.h Spool.h StreamIO.h TextString.h Thread.h ThreadFunc.h \
	Timer.h UUID.h UnixSocket.h Util.h WaitCondition.h XML.h XMLParse.h sigslot.h

