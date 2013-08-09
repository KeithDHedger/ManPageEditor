
include ../flagsandlibs
include ../sources

manpageeditor_SOURCES = $(COMMONSRC)

manpageeditor_CFLAGS = -O3 -export-dynamic
manpageeditor_CXXFLAGS = -O3 -export-dynamic

appfilesdir = ${datarootdir}/applications
appfiles_DATA = ../resources/applications/*

pixfilesdir = ${datarootdir}/pixmaps
pixfiles_DATA = ../resources/pixmaps/*

bin_PROGRAMS = manpageeditor

man1_MANS = ../resources/man/manpageeditor.1

helpfilesdir = $(pkgdatadir)/help
helpfiles_DATA = ../resources/help/*

examplefilesdir = $(pkgdatadir)/examples
examplefiles_DATA = ../resources/examples/*

install-data-hook:
	echo Icon=${datarootdir}/pixmaps/ManPageEditor.png >> $(DESTDIR)${datarootdir}/applications/ManPageEditor.desktop
	echo Exec=${bindir}/manpageeditor >> $(DESTDIR)${datarootdir}/applications/ManPageEditor.desktop


#uninstall-hook:
#	rm -r $(DESTDIR)${pkgdatadir}

