#!/bin/sh
rm -rf configure *.spec autom4te.cache libFinagle-*.tar.bz2
aclocal
autoheader
automake
autoconf

