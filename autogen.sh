#!/bin/sh
rm -rf configure *.spec autom4te.cache libFinagle-*.tar.bz2
libtoolize --copy --automake
aclocal -Ibuild-aux
autoheader
automake --add-missing --copy
autoconf

