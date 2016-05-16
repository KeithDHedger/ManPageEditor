#
#
AC_INIT([ManPageEditor],[0.1.1],[PROJ],[ManPageEditor])

AC_CONFIG_HEADER([config.h])
AC_PROG_CXX

AM_INIT_AUTOMAKE

AC_PROG_CC
AC_HEADER_STDC

AM_PROG_CC_C_O

AC_MSG_CHECKING(whether to use gtk3)
AC_ARG_ENABLE(gtk3,
    [  --enable-gtk3           Use GTK3.],
    [enable_gtk3=$enableval],
    [enable_gtk3="no"])
AC_MSG_RESULT($enable_gtk3)
if test "$enable_gtk3" = "yes"; then
	AC_DEFINE([_USEGTK3_], [], [Use GTK3])
	GTK_MODULES="gtk+-3.0 >= 3.10.0"
	GTKSRC_MODULES="gtksourceview-3.0 >= 3.10.0"
else
	GTK_MODULES="gtk+-2.0 >= 2.24.0"
	GTKSRC_MODULES="gtksourceview-2.0 >= 2.9.9"
fi

PKG_CHECK_MODULES(GTK, $GTK_MODULES)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

PKG_CHECK_MODULES(GTKSRC, $GTKSRC_MODULES)
AC_SUBST(GTKSRC_CFLAGS)
AC_SUBST(GTKSRC_LIBS)

AC_MSG_CHECKING(whether to include spell checker (aspell))
AC_ARG_ENABLE(aspell,
    [  --enable-aspell         Include spellchecker.],
    [enable_aspell=$enableval],
    [enable_aspell="no"])
AC_MSG_RESULT($enable_aspell)
if test "$enable_aspell" = "yes"; then
	ASPELL_LIBS=-laspell
	AC_SUBST(ASPELL_LIBS)
	AC_DEFINE([_ASPELL_], [], [Include spellchecker])
fi

AC_CHECK_PROG(GROFF, groff ,true,false)
if test $GROFF = false; then
	AC_MSG_ERROR([No groff program found!])
fi

AC_MSG_CHECKING(whether to format using man or nroff)
AC_ARG_ENABLE(nroff,AS_HELP_STRING([--disable-nroff],[Format using nroff instead of man]),
    [enable_nroff=$enableval],
    [enable_nroff="no"])
AC_MSG_RESULT($enable_nroff)
if test "$enable_nroff" = "yes"; then
	AC_DEFINE([_USENROFF_], [], [Use nroff])
fi


AC_OUTPUT(Makefile ManPageEditor/app/Makefile)

echo
echo "You may now run:"
echo "make"
echo "sudo make install"
echo
echo "If you are installing from source you also need to run:"
echo "sudo make install-mimetypes"
echo "To install the mimetypes, icons, etc"
echo "When uninstalling from source use:"
echo "sudo make uninstall-mimetypes"
echo
echo "Packagers should include commands from install-mimetypes/uninstall-mimetypes in their post install/uninstall"
echo

