#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(mygame, 0.0.1, test@mygame.com)
AM_INIT_AUTOMAKE([foreign])

# Checks for programs.
AC_PROG_RANLIB
AC_PROG_CXX
AC_PROG_LIBTOOL

#ToLua.cpp的依赖文件
GAME_TOLUA_DEPEND=`grep '$cfile' game_server/script/ToScript.pkg -w | sed 's/$cfile//' | sed 's/"/ /g' | sed ':a;N;$!ba;s/\n//g'`
AC_SUBST(GAME_TOLUA_DEPEND)
#ToLua.cpp的依赖文件
ROBOT_TOLUA_DEPEND=`grep '$cfile' robot_server/script/ToScript.pkg -w | sed 's/$cfile//' | sed 's/"/ /g' | sed ':a;N;$!ba;s/\n//g'`
AC_SUBST(ROBOT_TOLUA_DEPEND)

AC_CONFIG_HEADERS([conf.h])

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_ARG_ENABLE([log],
            AS_HELP_STRING([--enable-log], [enbale log trace,info,debug]),
            [enable_log="yes"],
            [enable_log="yes"])
if test "x$enable_log" = "xyes"; then
    if test -d ".svn"; then
        AC_DEFINE([ENABLE_LOG_FATAL],[],[enable fatal output])
        AC_DEFINE([ENABLE_LOG_ERROR],[],[enable error output])
        AC_DEFINE([ENABLE_LOG_WARN],[],[enable warn output])
        AC_DEFINE([ENABLE_LOG_INFO],[],[enable info output])
        AC_DEFINE([ENABLE_LOG_TRACE],[],[enable trace output])
        AC_DEFINE([ENABLE_LOG_DEBUG],[],[enable debug output])
    else
        AC_DEFINE([ENABLE_LOG_FATAL],[],[enable fatal output])
        AC_DEFINE([ENABLE_LOG_ERROR],[],[enable error output])
        AC_DEFINE([ENABLE_LOG_WARN],[],[enable warn output])
        AC_DEFINE([ENABLE_LOG_INFO],[],[enable info output])
        AC_DEFINE([ENABLE_LOG_TRACE],[],[enable trace output])
        AC_DEFINE([ENABLE_LOG_DEBUG],[],[enable debug output])
    fi
fi

AC_DEFINE([SANDTOY_DEBUG],[],[compile debug mode])

# debug build flag
AM_CONDITIONAL([IS_BUILD_DEBUG],false)
AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],[build with debug(default is no)])],
[
if test "$enableval" = "yes" ; then
AM_CONDITIONAL([IS_BUILD_DEBUG],true)
fi
],
IS_BUILD_DEBUG=false)

# release build flag
AM_CONDITIONAL([IS_BUILD_RELEASE],false)
AC_ARG_ENABLE([release],
    [AS_HELP_STRING([--enable-release],[build with release(default is no)])],
[
if test "$enableval" = "yes" ; then
AM_CONDITIONAL([IS_BUILD_RELEASE],true)
fi
],
IS_BUILD_RELEASE=false)

# valgrind build flag
AM_CONDITIONAL([IS_BUILD_VALGRIND],false)
AC_ARG_ENABLE([valgrind],
    [AS_HELP_STRING([--enable-valgrind],[build with valgrind(default is no)])],
[
if test "$enableval" = "yes" ; then
AM_CONDITIONAL([IS_BUILD_VALGRIND],true)
fi
],
IS_BUILD_VALGRIND=false)

# online build flag
AM_CONDITIONAL([IS_BUILD_ONLINE],false)
AC_ARG_ENABLE([online],
    [AS_HELP_STRING([--enable-online],[build with online(default is no)])],
[
if test "$enableval" = "yes" ; then
AM_CONDITIONAL([IS_BUILD_ONLINE],true)
fi
],
IS_BUILD_ONLINE=false)

# gprof build flag
AM_CONDITIONAL([IS_BUILD_GPROF],false)
AC_ARG_ENABLE([gprof],
    [AS_HELP_STRING([--enable-gprof],[build with gprof(default is no)])],
[
if test "$enableval" = "yes" ; then
AM_CONDITIONAL([IS_BUILD_GPROF],true)
fi
],
IS_BUILD_GPROF=false)

AC_CONFIG_FILES([Makefile
                common/Makefile
                network/Makefile
                protocpp/Makefile
                transfer_server/Makefile
                account_server/Makefile
                area_server/Makefile
                login_server/Makefile
                gate_server/Makefile
                game_server/Makefile
                chat_server/Makefile
                center_server/Makefile
                db_server/Makefile
                log_server/Makefile
                gm_server/Makefile
				video_server/Makefile
                robot_server/Makefile
                ])
AC_OUTPUT
