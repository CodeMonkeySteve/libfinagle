METASOURCES = AUTO
INCLUDES = -I$(top_srcdir)
AM_CXXFLAGS = -Wall -DTESTDIR="\"$(srcdir)\""
TESTS = $(check_PROGRAMS)

check_PROGRAMS = testXML

testXML_SOURCES = CollectionTest.cpp DocumentTest.cpp ElementTest.cpp \
	NodeTest.cpp TestXML.cpp
testXML_LDADD = $(top_builddir)/Finagle/libFinagle.la

testXML_CXXFLAGS = $(CPPUNIT_CFLAGS) 
testXML_LDFLAGS = $(CPPUNIT_LIBS)
