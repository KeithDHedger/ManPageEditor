
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


SUBDIRS = ManPageEditor/app

remake:
	./remakesourcelist

install-exec-hook:
	echo Exec=${bindir}/manpageeditor >> $(DESTDIR)${datarootdir}/applications/ManPageEditor.desktop
	echo Icon=${datarootdir}/pixmaps/ManPageEditor.png >> $(DESTDIR)${datarootdir}/applications/ManPageEditor.desktop
	xdg-icon-resource install --context mimetypes --size 128 ManPageEditor/resources/documenticons/maneditdoc.png application-x-maneditdoc
	xdg-mime install ManPageEditor/resources/documenticons/maneditdoc-mime.xml
	update-mime-database /usr/share/mime
	gtk-update-icon-cache --force /usr/share/icons/hicolor

uninstall-hook:
	xdg-icon-resource uninstall --context mimetypes --size 128 application-x-maneditdoc
	xdg-mime uninstall ManPageEditor/resources/documenticons/maneditdoc-mime.xml
	update-mime-database /usr/share/mime
	gtk-update-icon-cache --force /usr/share/icons/hicolor
	rm -r $(DESTDIR)${pkgdatadir}
man1_MANS = ../resources/man/manpageeditor.1

helpfilesdir = $(pkgdatadir)/help
helpfiles_DATA = ../resources/help/*

examplefilesdir = $(pkgdatadir)/examples
examplefiles_DATA = ../resources/examples/*

#install-data-hook:
#	Icon=${datarootdir}/pixmaps/ManPageEditor.png >> $(DESTDIR)${datarootdir}/applications/ManPageEditor.desktop
#	Exec=${bindir}/manpageeditor >> $(DESTDIR)${datarootdir}/applications/ManPageEditor.desktop


#uninstall-hook:
#	rm -r $(DESTDIR)${pkgdatadir}

