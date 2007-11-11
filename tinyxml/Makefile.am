METASOURCES = AUTO
INCLUDES = -I$(top_srcdir)
AM_CXXFLAGS = -Wall

noinst_LIBRARIES = libTinyXML.a

libTinyXML_a_SOURCES = tinyxml.cpp tinyxmlparser.cpp tinyxmlerror.cpp

library_includedir = $(includedir)/$(PACKAGE)-$(VERSION)/tinyxml
library_include_HEADERS = tinyxml.h


docdir ?= $(datadir)/doc/$(PACKAGE)
htmldir = $(docdir)/TinyXML
html_DATA = $(srcdir)/docs/*
EXTRA_DIST = $(html_DATA)

SUBDIRS = test
