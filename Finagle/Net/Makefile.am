SUBDIRS = test
METASOURCES = AUTO
INCLUDES = -I$(top_srcdir) $(curl_CFLAGS)
AM_CXXFLAGS = -Wall

noinst_LTLIBRARIES = libNet.la
libNet_la_SOURCES = IPAddress.cpp Request.cpp Response.cpp Socket.cpp Transfer.cpp URI.cpp
libNet_la_LIBADD = $(curl_LIBS) 

library_includedir=$(includedir)/$(PACKAGE)-$(VERSION)/Finagle/Net
library_include_HEADERS = IPAddress.h InetSocket.h Request.h ServerSocket.h Socket.h \
	URI.h UnixSocket.h Transfer.h Response.h
