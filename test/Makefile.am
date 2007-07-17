#INCLUDES = -I$(top_srcdir)
METASOURCES = AUTO
check_PROGRAMS = testFinagle
testFinagle_SOURCES = MutexTest.cpp QueueTest.cpp StringTest.cpp TestFinagle.cpp ThreadTest.cpp WaitConditionTest.cpp
testFinagle_LDADD = $(top_srcdir)/Finagle/libFinagle.la

testFinagle_CXXFLAGS = $(PTHREAD_CFLAGS) $(z_CFLAGS) $(libpcre_CFLAGS) $(expat_CFLAGS) $(openssl_CFLAGS) $(CPPUNIT_CFLAGS)
testFinagle_LDFLAGS = $(PTHREAD_LIBS) $(z_LIBS) $(libpcre_LIBS) $(expat_LIBS) $(openssl_LIBS) $(CPPUNIT_LIBS)

TESTS = $(check_PROGRAMS)
