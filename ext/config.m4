dnl config.m4 for extension pbtojson

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use 'with':

dnl PHP_ARG_WITH([pbtojson],
dnl   [for pbtojson support],
dnl   [AS_HELP_STRING([--with-pbtojson],
dnl     [Include pbtojson support])])

dnl Otherwise use 'enable':

PHP_ARG_ENABLE([pbtojson],
  [whether to enable pbtojson support],
  [AS_HELP_STRING([--enable-pbtojson],
    [Enable pbtojson support])],
  [no])

if test "$PHP_PBTOJSON" != "no"; then
  dnl Write more examples of tests here...

  dnl Remove this code block if the library does not support pkg-config.
  dnl PKG_CHECK_MODULES([LIBFOO], [foo])
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBS, PBTOJSON_SHARED_LIBADD)

  dnl If you need to check for a particular library version using PKG_CHECK_MODULES,
  dnl you can use comparison operators. For example:
  dnl PKG_CHECK_MODULES([LIBFOO], [foo >= 1.2.3])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo < 3.4])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo = 1.2.3])

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-pbtojson -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/pbtojson.h"  # you most likely want to change this
  dnl if test -r $PHP_PBTOJSON/$SEARCH_FOR; then # path given as parameter
  dnl   PBTOJSON_DIR=$PHP_PBTOJSON
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for pbtojson files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       PBTOJSON_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$PBTOJSON_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the pbtojson distribution])
  dnl fi

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-pbtojson -> add include path
  dnl PHP_ADD_INCLUDE($PBTOJSON_DIR/include)

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-pbtojson -> check for lib and symbol presence
  dnl LIBNAME=PBTOJSON # you may want to change this
  dnl LIBSYMBOL=PBTOJSON # you most likely want to change this

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   AC_DEFINE(HAVE_PBTOJSON_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your pbtojson library.])
  dnl ], [
  dnl   $LIBFOO_LIBS
  dnl ])

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are not using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $PBTOJSON_DIR/$PHP_LIBDIR, PBTOJSON_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_PBTOJSON_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your pbtojson library.])
  dnl ],[
  dnl   -L$PBTOJSON_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(PBTOJSON_SHARED_LIBADD)

  dnl In case of no dependencies
  AC_DEFINE(HAVE_PBTOJSON, 1, [ Have pbtojson support ])
  PHP_ADD_INCLUDE(.)
  PHP_ADD_LIBRARY(stdc++, 1, PBTOJSON_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(gopbtojson, ./, PBTOJSON_SHARED_LIBADD)
  PHP_SUBST(PBTOJSON_SHARED_LIBADD)
  PHP_NEW_EXTENSION(pbtojson, pbtojson.c, $ext_shared)
fi
