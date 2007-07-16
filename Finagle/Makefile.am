METASOURCES = AUTO
lib_LTLIBRARIES = libFinagle.la

libFinagle_la_SOURCES = AppLog.cpp AppLoop.cpp Array.h Finagle.h Compress.cpp \
	Compress.h Configure.cpp Configure.h Constructable.h DataStream.h DateTime.cpp \
	DateTime.h DateTimeMask.cpp DateTimeMask.h Dir.cpp Dir.h Exception.h File.cpp \
	File.h FileDescWatcher.cpp FileDescWatcher.h FilePath.cpp FilePath.h \
	FileSystem.h GarbageCollector.h IPAddress.cpp IPAddress.h InetSocket.h List.h \
	MD5.cpp MD5.h Map.h MapIterator.h MemTrace.cpp MemTrace.h MultiMap.h Mutex.cpp \
	Mutex.h OptionParser.cpp OptionParser.h OrderedMap.h \
	PriorityMutex.cpp PriorityMutex.h Property.h PThreadEx.h Queue.h Range.h Rate.cpp Rate.h \
	Rectangle.cpp Rectangle.h ReferencedObject.h RegEx.h SSL.cpp SSL.h Schedule.cpp \
	Schedule.h ServerSocket.h Set.h Singleton.h Socket.cpp Socket.h StreamIO.cpp \
	StreamIO.h TextString.cpp TextString.h Thread.cpp Thread.h ThreadFunc.h Timer.cpp \
	Timer.h UUID.h UnixSocket.h Util.cpp Util.h WaitCondition.cpp WaitCondition.h \
	XML.cpp XML.h XMLParse.cpp XMLParse.h sigslot.h \
	AppLog.h AppLogEntry.h AppLoop.h Array.h ByteArray.h \
	ByteOrder.h Collection.h

libFinagle_la_LDFLAGS = -no-undefined -version-info @LIB_CURRENT@:@LIB_REVISION@:@LIB_AGE@
#libFinagle_la_LIBADD = $(PTHREAD_LIBS) $(z_LIBS) $(libpcre_LIBS) $(expat_LIBS) $(openssl_LIBS)
INCLUDES = -I$(top_srcdir) $(PTHREAD_CFLAGS) $(expat_CFLAGS) $(libpcre_CFLAGS) $(openssl_CFLAGS) $(z_CFLAGS)
