#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([apt-transport-private], [1.1], [vlastavesely@protonmail.ch])
AC_CONFIG_SRCDIR([acquire-method.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MKDIR_P
AC_PROG_MAKE_SET

# Checks for libraries.
PKG_CHECK_MODULES(OPENSSL, openssl)
PKG_CHECK_MODULES(CURL, libcurl)
PKG_CHECK_MODULES(CHECK, check)

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h string.h unistd.h stdbool.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([memmove memset strdup])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
