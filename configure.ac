#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([blkin], [0.1], [Marios-Evaggelos Kogias <marioskogias@gmail.com>], [blkin], [https://github.com/marioskogias/blkin])

AC_CONFIG_MACRO_DIR([m4])

# Automake
AM_INIT_AUTOMAKE([foreign subdir-objects dist-xz])

# Libtool
LT_INIT

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX

PKG_CHECK_MODULES([LTTNG_UST], [lttng-ust < 2.5])

AC_CHECK_LIB(boost_system, main, [], AC_MSG_NOTICE(["Boost system library not found."]))

AC_CHECK_LIB(boost_thread, main, [], AC_MSG_NOTICE(["Boost thread library not found."]))

AM_PATH_PYTHON([3.0], , [AC_MSG_NOTICE(Python3 required for optional Babeltrace plugins.)])

# Python bindings for Babeltrace plugins
AC_ARG_WITH([babeltrace],
    AS_HELP_STRING([--without-babeltrace], [Ignore presence of babeltrace and disable it]))

have_babeltrace=check
AS_IF([test "x$with_babeltrace" != "xno"],
      [AX_PYTHON_MODULE([babeltrace], [have_babeltrace=no])],
      [have_babeltrace=no])

AS_IF([test "x$have_babeltrace" != "xno"],
      [have_babeltrace=yes],
      [AS_IF([test "x$with_babeltrace" = "xyes"],
             [AC_MSG_ERROR([babeltrace requested but not found])
      ])
])
AM_CONDITIONAL([WITH_BABEL], [test "x$have_babeltrace" = "xyes"])

# Compiler flags

AC_SUBST(AM_CXXFLAGS)
AM_CXXFLAGS="${AM_CXXFLAGS}"

AC_CONFIG_FILES([Makefile
	blkin.pc
	blkin.spec])

AC_OUTPUT
