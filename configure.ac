AC_PREREQ(2.63)
AC_INIT([libcangjie], [0.0.1], [https://github.com/wanleung/libcangjie/issues], [libcangjie])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.11 no-dist-gzip dist-xz tar-ustar foreign])
AC_PROG_MKDIR_P

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

# Needed to build C++ and check for deps
AC_PROG_CXX
AC_LANG_PUSH([C++])

# Initialize libtool
LT_INIT([disable-static])

# Define these substitions here to keep all version information in one place.
# For information on how to properly maintain the library version information,
# refer to the libtool manual, section "Updating library version information":
# http://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html
AC_SUBST([CANGJIE_SO_VERSION], [0:0:0])

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
