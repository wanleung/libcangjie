AC_PREREQ(2.63)
AC_INIT([libcangjie], [0.0.1], [https://github.com/wanleung/libcangjie], [libcangjie])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

AC_SUBST([PACKAGE_NAME], ["$PACKAGE_NAME"])
AC_SUBST([PACKAGE_VERSION], ["$PACKAGE_VERSION"])

AM_INIT_AUTOMAKE([1.11 no-dist-gzip dist-xz tar-ustar foreign])
AC_PROG_MKDIR_P

m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

# OUTPUT files
AC_CONFIG_FILES([
  Makefile
])

AC_OUTPUT
