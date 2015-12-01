#
#
AC_INIT([ManPageEditor],[0.0.18],[PROJ],[ManPageEditor])

AC_CONFIG_HEADER([config.h])
AC_PROG_CXX

AM_INIT_AUTOMAKE

AC_PROG_CC
AC_HEADER_STDC

AM_PROG_CC_C_O

GTK_MODULES="gtk+-2.0 >= 2.24.0"
PKG_CHECK_MODULES(GTK, $GTK_MODULES)
AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

GTKSRC_MODULES="gtksourceview-2.0 >= 2.9.9"
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

