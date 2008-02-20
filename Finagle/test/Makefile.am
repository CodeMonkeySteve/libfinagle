METASOURCES = AUTO
INCLUDES = -I$(top_srcdir)/.
AM_CXXFLAGS = -Wall -DTESTDIR="\"$(srcdir)\""
TESTS = $(check_PROGRAMS)

check_PROGRAMS = testFinagle

testFinagle_SOURCES = AppLogTest.cpp ExceptionTest.cpp FactoryTest.cpp \
	FilePathTest.cpp GarbageCollectorTest.cpp InitializerTest.cpp MutexTest.cpp \
	ObjectRefTest.cpp PriorityQueueTest.cpp QueueTest.cpp SizedQueueTest.cpp StringTest.cpp \
	TestFinagle.cpp ThreadTest.cpp UUIDTest.cpp UtilTest.cpp VelocimeterTest.cpp \
	WaitConditionTest.cpp

testFinagle_CXXFLAGS = $(PTHREAD_CFLAGS) $(z_CFLAGS) $(libpcre_CFLAGS) $(expat_CFLAGS) $(openssl_CFLAGS) $(CPPUNIT_CFLAGS) \
                       $(uuid_CFLAGS)
testFinagle_LDFLAGS = $(CPPUNIT_LIBS) 
testFinagle_LDADD = $(top_builddir)/Finagle/libFinagle.la
