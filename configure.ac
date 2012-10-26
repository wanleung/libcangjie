AC_PREREQ(2.63)
AC_INIT([libcangjie], [0.0.1], [https://github.com/wanleung/libcangjie], [libcangjie])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_SUBST([PACKAGE_NAME], ["$PACKAGE_NAME"])
AC_SUBST([PACKAGE_VERSION], ["$PACKAGE_VERSION"])

AM_INIT_AUTOMAKE([1.11 no-dist-gzip dist-xz tar-ustar foreign])
AC_PROG_MKDIR_P

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

# Needed to build C++ and check for deps
AC_PROG_CXX
AC_LANG_PUSH([C++])

# Check for Berkeley DB
AC_CHECK_HEADERS(db_cxx.h,HAVE_DBCXX="yes",HAVE_DBCXX="no")
if test "$HAVE_DBCXX" = no
then
  AC_MSG_ERROR([Berkeley DB C++ bindings not found!])
fi
AC_CHECK_LIB(db_cxx,main,DBCXX_LIBS="-ldb_cxx",DBCXX_LIBS="")
AC_SUBST(DBCXX_LIBS)

AC_LANG_POP([C++])

# OUTPUT files
AC_CONFIG_FILES([
  Makefile
])

AC_OUTPUT
