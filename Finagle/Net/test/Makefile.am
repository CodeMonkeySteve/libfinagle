METASOURCES = AUTO
INCLUDES = -I$(top_srcdir)
AM_CXXFLAGS = -Wall -DTESTDIR="\"$(srcdir)\""
TESTS = $(check_PROGRAMS)

check_PROGRAMS = testNet

testNet_SOURCES = TestNet.cpp TransferTest.cpp
testNet_LDADD = $(top_builddir)/Finagle/libFinagle.la

testNet_CXXFLAGS = $(CPPUNIT_CFLAGS) 
testNet_LDFLAGS = $(CPPUNIT_LIBS)
