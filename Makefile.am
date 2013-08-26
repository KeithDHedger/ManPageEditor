
SUBDIRS = ManPageEditor/app

remake:
	./remakesourcelist

install-exec-hook:
	xdg-icon-resource install --context mimetypes --size 256 ManPageEditor/resources/documenticons/256/maneditdoc.png application-x-maneditdoc
	xdg-icon-resource install --context mimetypes --size 128 ManPageEditor/resources/documenticons/128/maneditdoc.png application-x-maneditdoc
	xdg-icon-resource install --context mimetypes --size 48 ManPageEditor/resources/documenticons/48/maneditdoc.png application-x-maneditdoc
	xdg-mime install ManPageEditor/resources/documenticons/maneditdoc-mime.xml
	update-mime-database /usr/share/mime
	gtk-update-icon-cache --force /usr/share/icons/hicolor

uninstall-hook:
	xdg-icon-resource uninstall --context mimetypes --size 256 application-x-maneditdoc
	xdg-icon-resource uninstall --context mimetypes --size 128 application-x-maneditdoc
	xdg-icon-resource uninstall --context mimetypes --size 48 application-x-maneditdoc
	xdg-mime uninstall ManPageEditor/resources/documenticons/maneditdoc-mime.xml
	update-mime-database /usr/share/mime
	gtk-update-icon-cache --force /usr/share/icons/hicolor
	rm -r $(DESTDIR)${pkgdatadir}
