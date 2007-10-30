#!/bin/sh
rm -rf configure *.spec autom4te.cache doc/html libFinagle-*.tar.bz2
aclocal
autoheader
automake
autoconf

