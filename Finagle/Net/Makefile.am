SUBDIRS =
METASOURCES = AUTO
INCLUDES = -I$(top_srcdir)
AM_CXXFLAGS = -Wall

noinst_LTLIBRARIES = libNet.la
libNet_la_SOURCES = IPAddress.cpp Socket.cpp

library_includedir=$(includedir)/$(PACKAGE)-$(VERSION)/Finagle/Net
library_include_HEADERS = InetSocket.h IPAddress.h Socket.h UnixSocket.h ServerSocket.h
