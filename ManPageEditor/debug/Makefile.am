

include ../flagsandlibs
include ../sources

manpageeditordebug_SOURCES = $(COMMONSRC)

manpageeditordebug_CFLAGS = -ggdb -O0 -export-dynamic
manpageeditordebug_CXXFLAGS = -ggdb -O0 -export-dynamic

bin_PROGRAMS = manpageeditordebug

install-exec-local:
	cd $(DESTDIR)$(bindir) && \
	rm manpageeditordebug

