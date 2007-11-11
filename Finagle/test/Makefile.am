METASOURCES = AUTO
INCLUDES = -I$(top_srcdir)/.
AM_CXXFLAGS = -Wall -DTESTDIR="\"$(srcdir)\""
TESTS = $(check_PROGRAMS)

check_PROGRAMS = testFinagle

testFinagle_SOURCES = FactoryTest.cpp FilePathTest.cpp MutexTest.cpp \
	ObjectRefTest.cpp PriorityQueueTest.cpp QueueTest.cpp SizedQueueTest.cpp StringTest.cpp \
	TestFinagle.cpp ThreadTest.cpp UUIDTest.cpp UtilTest.cpp WaitConditionTest.cpp
testFinagle_LDADD = $(top_builddir)/Finagle/libFinagle.la

testFinagle_CXXFLAGS = $(PTHREAD_CFLAGS) $(z_CFLAGS) $(libpcre_CFLAGS) $(expat_CFLAGS) $(openssl_CFLAGS) $(CPPUNIT_CFLAGS) \
                       $(uuid_CFLAGS)
testFinagle_LDFLAGS = $(PTHREAD_LIBS) $(z_LIBS) $(libpcre_LIBS) $(expat_LIBS) $(openssl_LIBS) $(CPPUNIT_LIBS) $(uuid_LIBS)
