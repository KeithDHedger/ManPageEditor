#!/bin/bash

rm -r autom4te.cache aclocal.m4||true
./remakesourcelist
aclocal
autoheader
touch NEWS README AUTHORS ChangeLog
automake --add-missing --copy
autoconf
./configure $@

