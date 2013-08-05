
include ../flagsandlibs
include ../sources

manpageeditor_SOURCES = $(COMMONSRC)

manpageeditor_CFLAGS = -O3 -export-dynamic
manpageeditor_CXXFLAGS = -O3 -export-dynamic

pixfilesdir = $(pkgdatadir)/pixmaps
pixfiles_DATA = ../resources/pixmaps/*

scriptfilesdir = $(pkgdatadir)/scripts
scriptfiles_SCRIPTS = ../resources/scripts/README

bin_PROGRAMS = manpageeditor

man1_MANS = ../resources/man/manpageeditor.1

install-exec-hook:
	cp ../resources/pixmaps/ManPageEditor.desktop /usr/share/applications/ManPageEditor.desktop
	echo Icon=${prefix}/share/ManPageEditor/pixmaps/ManPageEditor.png >> /usr/share/applications/ManPageEditor.desktop
	echo Exec=${prefix}/bin/manpageeditor >> /usr/share/applications/ManPageEditor.desktop
#	update-menus

uninstall-hook:
	rm -f /usr/share/applications/ManPageEditor.desktop
#	update-menus
	rm -fr $(pkgdatadir)

