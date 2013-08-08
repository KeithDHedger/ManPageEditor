
include ../flagsandlibs
include ../sources

manpageeditor_SOURCES = $(COMMONSRC)

manpageeditor_CFLAGS = -O3 -export-dynamic
manpageeditor_CXXFLAGS = -O3 -export-dynamic

pixfilesdir = $(pkgdatadir)/pixmaps
pixfiles_DATA = ../resources/pixmaps/*

bin_PROGRAMS = manpageeditor

man1_MANS = ../resources/man/manpageeditor.1

helpfilesdir = $(pkgdatadir)/help
helpfiles_DATA = ../resources/help/*

install-exec-hook:
	cp ../resources/pixmaps/ManPageEditor.desktop $(DESTDIR)${prefix}/share/applications/ManPageEditor.desktop
	echo Icon=${prefix}/share/ManPageEditor/pixmaps/ManPageEditor.png >> $(DESTDIR)${prefix}/share/applications/ManPageEditor.desktop
	echo Exec=${bindir}/manpageeditor >> $(DESTDIR)${prefix}/share/applications/ManPageEditor.desktop


uninstall-hook:
	rm -f $(DESTDIR)${prefix}/share/applications/ManPageEditor.desktop

