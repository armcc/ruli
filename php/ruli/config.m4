dnl $Id: config.m4,v 1.1 2004/03/17 23:32:34 evertonm Exp $
dnl config.m4 for extension ruli

PHP_ARG_WITH(ruli, for ruli support,
Make sure that the comment is aligned:
[  --with-ruli             Include ruli support])

if test "$PHP_RULI" != "no"; then

  # --with-ruli -> check with-path
  SEARCH_PATH="/usr/local/ruli /usr/local /usr"
  SEARCH_FOR="/include/ruli.h"
  if test -r $PHP_RULI/; then # path given as parameter
    RULI_DIR=$PHP_RULI
    if test -r $RULI_DIR/$SEARCH_FOR; then
      dnl AC_MSG_RESULT(found in $RULI_DIR)
      :
    else
      AC_MSG_RESULT(not found in $RULI_DIR)
      AC_MSG_ERROR([Please reinstall the ruli distribution])
    fi
  else # search default path list
    AC_MSG_CHECKING([for ruli files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        RULI_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi
  
  if test -z "$RULI_DIR"; then
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the ruli distribution])
  fi

  # --with-ruli -> add include path
  PHP_ADD_INCLUDE($RULI_DIR/include)

  # --with-ruli -> check for lib and symbol presence
  LIBNAME=ruli
  LIBSYMBOL=ruli_sync_query
  PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  [
    PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $RULI_DIR/lib, RULI_SHARED_LIBADD)
    AC_DEFINE(HAVE_RULILIB,1,[ ])
  ],[
    AC_MSG_ERROR([wrong ruli lib version or lib not found])
  ],[
    -L$RULI_DIR/lib -loop
  ])

  PHP_SUBST(RULI_SHARED_LIBADD)

  PHP_ADD_LIBRARY_WITH_PATH(oop, /usr/lib, OOP_SHARED_LIBADD)
  PHP_SUBST(OOP_SHARED_LIBADD)

  PHP_NEW_EXTENSION(ruli, php_ruli.c, $ext_shared)
fi

