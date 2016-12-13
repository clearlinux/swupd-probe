#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([swupd-probe], [2], [dev@lists.clearinux.org])
AM_INIT_AUTOMAKE([foreign -Wall -W subdir-objects no-dist-gzip dist-xz])
AM_SILENT_RULES([yes])

AC_CONFIG_SRCDIR([swupd-probe.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_UID_T
AC_TYPE_PID_T

# Checks for library functions.
AC_FUNC_FORK
AC_CHECK_FUNCS([dup2 strdup])

PKG_CHECK_MODULES([SYSTEMD], [systemd])

dir=""
AC_ARG_WITH([systemdsystemunitdir], AS_HELP_STRING([--with-systemdsystemunitdir=DIR],
            [path to systemd system service dir @<:@default=/usr/lib/systemd/system@:>@]), [dir=${withval}],
            [dir="$($PKG_CONFIG --variable=systemdsystemunitdir systemd)"])
test -z "${dir}" && dir=/usr/lib/systemd/system
AC_SUBST(systemdsystemunitdir, [${dir}])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
