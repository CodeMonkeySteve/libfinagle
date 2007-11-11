METASOURCES = AUTO
INCLUDES = -I$(top_srcdir)
AM_CXXFLAGS = -DTESTDIR="\"$(srcdir)\""
TESTS = $(check_PROGRAMS)

EXTRA_DIST = $(srcdir)/*.xml

check_PROGRAMS = testxpath

testxpath_SOURCES = htmlutil.cpp main.cpp htmlutil.h
testxpath_LDADD = $(top_builddir)/tinyxpath/libTinyXPath.a \
	$(top_builddir)/tinyxml/libTinyXML.a
