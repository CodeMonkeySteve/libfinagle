METASOURCES = AUTO
SUBDIRS = test
INCLUDES = -I$(top_srcdir)
AM_CXXFLAGS = -Wall

noinst_LIBRARIES = libTinyXPath.a

libTinyXPath_a_SOURCES = \
   action_store.cpp lex_util.cpp node_set.cpp tokenlist.cpp xml_util.cpp \
   xpath_expression.cpp xpath_processor.cpp xpath_stack.cpp xpath_stream.cpp xpath_syntax.cpp xpath_static.cpp
libTinyXPath_a_LIBADD = $(top_builddir)/tinyxml/libTinyXML.a

library_includedir=$(includedir)/$(PACKAGE)-$(VERSION)/tinyxpath
library_include_HEADERS = action_store.h byte_stream.h lex_token.h lex_util.h node_set.h tinyxpath_conf.h tokenlist.h xml_util.h xpath_expression.h xpath_processor.h xpath_stack.h xpath_static.h xpath_stream.h xpath_syntax.h
