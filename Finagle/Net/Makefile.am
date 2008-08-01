SUBDIRS = test
METASOURCES = AUTO
INCLUDES = -I$(top_srcdir) $(curl_CFLAGS)
AM_CXXFLAGS = -Wall

noinst_LTLIBRARIES = libNet.la
libNet_la_SOURCES = IPAddress.cpp InetSocket.cpp MultipartResponse.cpp \
	Request.cpp Response.cpp Socket.cpp Transfer.cpp URL.cpp
libNet_la_LIBADD = $(curl_LIBS) 

library_includedir=$(includedir)/$(PACKAGE)-$(VERSION)/Finagle/Net
library_include_HEADERS = IPAddress.h InetSocket.h MultipartResponse.h \
	Request.h Response.h ServerSocket.h Socket.h Transfer.h URL.h UnixSocket.h
