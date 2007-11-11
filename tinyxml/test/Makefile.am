METASOURCES = AUTO
check_PROGRAMS = xmltest

xmltest_SOURCES = xmltest.cpp
xmltest_LDADD = $(top_builddir)/tinyxml/libTinyXML.a \
	$(top_builddir)/Finagle/libFinagle.la

#xmltest_CXXFLAGS = $(libFinagle_CFLAGS) $(CPPUNIT_CFLAGS)
#xmltest_LDFLAGS = $(libFinagle_LIBS) $(CPPUNIT_LIBS)

TESTS = $(check_PROGRAMS)
INCLUDES = -I$(top_srcdir)/.
