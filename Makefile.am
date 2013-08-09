
SUBDIRS = ManPageEditor/app

remake:
	./remakesourcelist

install-exec-hook:
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
