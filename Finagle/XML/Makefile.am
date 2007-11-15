SUBDIRS = test
METASOURCES = AUTO
INCLUDES = -I$(top_srcdir)
AM_CXXFLAGS = -Wall

noinst_LTLIBRARIES = libXML.la
libXML_la_SOURCES = Document.cpp Element.cpp Node.cpp

library_includedir=$(includedir)/$(PACKAGE)-$(VERSION)/Finagle/XML
library_include_HEADERS = Document.h Element.h Iterator.h Node.h Text.h
