
SUBDIRS = ManPageEditor/app

remake:
	./remakesourcelist

install-mimetypes:
	xdg-icon-resource install --context mimetypes --size 256 ManPageEditor/resources/documenticons/256/maneditdoc.png application-x-maneditdoc
	xdg-icon-resource install --context mimetypes --size 128 ManPageEditor/resources/documenticons/128/maneditdoc.png application-x-maneditdoc
	xdg-icon-resource install --context mimetypes --size 48 ManPageEditor/resources/documenticons/48/maneditdoc.png application-x-maneditdoc
	xdg-mime install ManPageEditor/resources/documenticons/maneditdoc-mime.xml
	update-mime-database /usr/share/mime
	gtk-update-icon-cache --force /usr/share/icons/hicolor

uninstall-mimetypes:
	xdg-icon-resource uninstall --context mimetypes --size 256 application-x-maneditdoc
	xdg-icon-resource uninstall --context mimetypes --size 128 application-x-maneditdoc
	xdg-icon-resource uninstall --context mimetypes --size 48 application-x-maneditdoc
	xdg-mime uninstall ManPageEditor/resources/documenticons/maneditdoc-mime.xml
	update-mime-database /usr/share/mime
	gtk-update-icon-cache --force /usr/share/icons/hicolor

install-exec-hook:
	@echo "*******************************************************"
	@echo "* If you have built from source you must now run:     *"
	@echo "* make install-mimetypes as root to install mimetypes *"
	@echo "*******************************************************"

uninstall-hook:
	rm -r $(DESTDIR)${pkgdatadir}
	@echo "*******************************************************"
	@echo "* If you have built from source you should now run:     *"
	@echo "* make uninstall-mimetypes as root to uninstall mimetypes *"
	@echo "*******************************************************"
