#!/bin/bash

rm -r autom4te.cache aclocal.m4
./remakesourcelist
aclocal
autoheader
touch NEWS README AUTHORS ChangeLog
automake --add-missing --copy
autoconf
./configure $@

