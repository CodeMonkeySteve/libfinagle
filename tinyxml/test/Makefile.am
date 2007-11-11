METASOURCES = AUTO
INCLUDES = -I$(top_srcdir)
AM_CXXFLAGS = -DTESTDIR="\"$(srcdir)\""
TESTS = $(check_PROGRAMS)

EXTRA_DIST = $(srcdir)/*.xml

check_PROGRAMS = xmltest

xmltest_SOURCES = xmltest.cpp
xmltest_LDADD = $(top_builddir)/tinyxml/libTinyXML.a \
	$(top_builddir)/Finagle/libFinagle.la

