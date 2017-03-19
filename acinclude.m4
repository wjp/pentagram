dnl When config.status generates a header, we must update config.h.stamp.
dnl This file resides in the same directory as the config header
dnl  that is generated.

dnl Autoconf calls _AC_AM_CONFIG_HEADER_HOOK (when defined) in the
dnl loop where config.status creates the headers, so we can generate
dnl our stamp files there.
AC_DEFUN([_AC_AM_CONFIG_HEADER_HOOK],
[[echo "# timestamp for $1" >$1.stamp]])




dnl Set CC_FOR_BUILD to a native compiler. This is used to compile
dnl tools needed for the Pentagram build process on the build machine.
dnl (adapted from config.guess, (C) 1992-2005 Free Software Foundation, Inc.)
AC_DEFUN([PENT_BUILDCC],[
if test x$cross_compiling = xyes; then
  AC_MSG_CHECKING(for a native C compiler)
  if test x${CC_FOR_BUILD+set} != xset ; then
    echo "int x;" > conftest.$ac_ext
    for c in cc gcc c89 c99 ; do
      if ($c -c -o conftest.o conftest.$ac_ext) >/dev/null 2>&1 ; then
        CC_FOR_BUILD="$c"; break
      fi
    done
    if test x"$CC_FOR_BUILD" = x ; then
      CC_FOR_BUILD=no_compiler_found
    fi
  fi
  if test "x$CC_FOR_BUILD" = xno_compiler_found ; then
    AC_MSG_RESULT(none found)
  else
    AC_MSG_RESULT($CC_FOR_BUILD)
  fi

  # Very basic check for EXEEXT_FOR_BUILD
  BUILDEXEEXT=
  case "$build_os" in
  mingw32* ) BUILDEXEEXT=.exe ;;
  cygwin* ) BUILDEXEEXT=.exe ;;
  esac
else
  CC_FOR_BUILD=$CC
  BUILDEXEEXT=$EXEEXT
fi
]);



dnl Check if we have SDL2_ttf (header and library) version >= 2.0.14
dnl Input:
dnl SDL2_CFLAGS, SDL2_LIBS are set correctly
dnl Output:
dnl SDLTTF2_LIBS contains the necessary libs/ldflags
dnl HAVE_SDL2_TTF_H is AC_DEFINE-d if including "SDL2_ttf.h" works
dnl HAVE_SDL2_SDL_TTF_H is AC_DEFINE-d if including <SDL/SDL2_ttf.h> works

AC_DEFUN([PENT_CHECK_SDL2TTF],[
  REQ_MAJOR=2
  REQ_MINOR=0
  REQ_PATCHLEVEL=14
  REQ_VERSION=$REQ_MAJOR.$REQ_MINOR.$REQ_PATCHLEVEL

  AC_MSG_CHECKING([for SDL2_ttf - version >= $REQ_VERSION])

  pent_backupcppflags="$CPPFLAGS"
  pent_backupldflags="$LDFLAGS"
  pent_backuplibs="$LIBS"

  CPPFLAGS="$CPPFLAGS $SDL2_CFLAGS"
  LDFLAGS="$LDFLAGS $SDL2_LIBS"

  pent_sdl2ttfok=yes

  dnl First: include "SDL_ttf.h"

  AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
  #include "SDL_ttf.h"
  ]],)],sdlttfh_found=yes,sdlttf_found=no)

  if test x$sdlttfh_found = xno; then

    dnl If failed: include <SDL2/SDL_ttf.h>

    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
    #include <SDL2/SDL_ttf.h>
    ]],)],sdl2sdlttfh_found=yes,sdl2sdlttfh_found=no)
    if test x$sdl2sdlttfh_found = xno; then
      pent_sdl2ttfok=no
    else
      AC_DEFINE(HAVE_SDL2_SDL_TTF_H, 1, [Define to 1 if you have the <SDL2/SDL_ttf.h> header file but not "SDL_ttf.h"])
    fi
  else
    AC_DEFINE(HAVE_SDL_TTF_H, 1, [Define to 1 if you have the "SDL_ttf.h" header file])
  fi

  if test "x$pent_sdl2ttfok" = xyes; then
    dnl Version check (cross-compile-friendly idea borrowed from autoconf)

    AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
    #include "SDL.h"
    #ifdef HAVE_SDL_TTF_H
    #include "SDL_ttf.h"
    #else
    #include <SDL2/SDL_ttf.h>
    #endif

    int main()
    {
      static int test_array[1-2*!(SDL_TTF_MAJOR_VERSION>$REQ_MAJOR||(SDL_TTF_MAJOR_VERSION==$REQ_MAJOR&&(SDL_TTF_MINOR_VERSION>$REQ_MINOR||(SDL_TTF_MINOR_VERSION==$REQ_MINOR&&SDL_TTF_PATCHLEVEL>=$REQ_PATCHLEVEL))))];
      test_array[0] = 0;
      return 0;
    }
    ]])],,[[pent_sdl2ttfok="no, version < $REQ_VERSION found"]])
  fi

  dnl Next: try linking

  if test "x$pent_sdl2ttfok" = xyes; then

    LIBS="$LIBS -lSDL2_ttf"

    AC_LINK_IFELSE([AC_LANG_SOURCE([[
    #include "SDL.h"
    #ifdef HAVE_SDL_TTF_H
    #include "SDL_ttf.h"
    #else
    #include <SDL2/SDL_ttf.h>
    #endif

    int main(int argc, char* argv[]) {
      TTF_Init();
      return 0;
    }
    ]])],sdl2ttflinkok=yes,sdl2ttflinkok=no)
    if test x$sdl2ttflinkok = xno; then

      dnl If failed: try -lSDL2_ttf -lfreetype
      dnl Note: This is mainly for my personal convenience. It should not
      dnl       be necessary usually, but shouldn't hurt either -wjp, 20050727
      dnl       (The reason is that I often statically link against SDL_ttf)

      LIBS="$LIBS -lfreetype"
      AC_LINK_IFELSE([AC_LANG_SOURCE([[
      #include "SDL.h"
      #ifdef HAVE_SDL_TTF_H
      #include "SDL_ttf.h"
      #else
      #include <SDL2/SDL_ttf.h>
      #endif

      int main(int argc, char* argv[]) {
        TTF_Init();
        return 0;
      }
      ]])],sdlttflinkok=yes,sdlttflinkok=no)

      if test x$sdl2ttflinkok = xno; then
        pent_sdl2ttfok=no
      else
        SDL2TTF_LIBS="-lSDL2_ttf -lfreetype"
      fi
    else  
      SDL2TTF_LIBS="-lSDL2_ttf"
    fi
  fi

  AC_MSG_RESULT($pent_sdl2ttfok)

  LDFLAGS="$pent_backupldflags"
  CPPFLAGS="$pent_backupcppflags"
  LIBS="$pent_backuplibs"

  if test "x$pent_sdl2ttfok" = xyes; then
    ifelse([$1], , :, [$1])
  else
    ifelse([$2], , :, [$2])
  fi
]);





dnl Check if we have SDL2 (sdl2-config, header and library) version >= 2.0.4
dnl Output:
dnl SDL2_CFLAGS and SDL2_LIBS are set and AC_SUBST-ed
dnl HAVE_SDL2_H is AC_DEFINE-d

AC_DEFUN([PENT_CHECK_SDL2],[
  REQ_MAJOR=2
  REQ_MINOR=0
  REQ_PATCHLEVEL=4
  REQ_VERSION=$REQ_MAJOR.$REQ_MINOR.$REQ_PATCHLEVEL

  pent_backupcppflags="$CPPFLAGS"
  pent_backupldflags="$LDFLAGS"
  pent_backuplibs="$LIBS"

  pent_sdl2ok=yes

  AC_ARG_WITH(sdl2-prefix,[  --with-sdl2-prefix=PFX   Prefix where SDL2 is installed (optional)], sdl2_prefix="$withval", sdl2_prefix="")
  AC_ARG_WITH(sdl2-exec-prefix,[  --with-sdl2-exec-prefix=PFX Exec prefix where SDL2 is installed (optional)], sdl2_exec_prefix="$withval", sdl2_exec_prefix="")

  dnl First: find sdl2-config

  sdl2_args=""
  if test x$sdl2_exec_prefix != x ; then
     sdl2_args="$sdl2_args --exec-prefix=$sdl2_exec_prefix"
     if test x${SDL2_CONFIG+set} != xset ; then
        SDL2_CONFIG=$sdl2_exec_prefix/bin/sdl2-config
     fi
  fi
  if test x$sdl2_prefix != x ; then
     sdl2_args="$sdl2_args --prefix=$sdl2_prefix"
     if test x${SDL2_CONFIG+set} != xset ; then
        SDL2_CONFIG=$sdl2_prefix/bin/sdl2-config
     fi
  fi

  PATH="$prefix/bin:$prefix/usr/bin:$PATH"
  AC_PATH_PROG(SDL2_CONFIG, sdl2-config, no, [$PATH])
  if test "$SDL2_CONFIG" = "no" ; then
    pent_sdl2ok=no
  else
    SDL2_CFLAGS=`$SDL2_CONFIG $sdl2conf_args --cflags`
    SDL2_LIBS=`$SDL2_CONFIG $sdl2conf_args --libs`

    sdl2_major_version=`$SDL2_CONFIG $sdl2_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    sdl2_minor_version=`$SDL2_CONFIG $sdl2_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    sdl2_patchlevel=`$SDL2_CONFIG $sdl2_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
  fi

  AC_MSG_CHECKING([for SDL2 - version >= $REQ_VERSION])


  dnl Second: include "SDL.h"

  if test x$pent_sdl2ok = xyes ; then
    CPPFLAGS="$CPPFLAGS $SDL2_CFLAGS"
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
    #include "SDL.h"
    ]],)],sdlh_found=yes,sdlh_found=no)

    if test x$sdlh_found = xno; then
      pent_sdl2ok=no
    else
      AC_DEFINE(HAVE_SDL_H, 1, [Define to 1 if you have the "SDL.h" header file])
    fi
  fi

  dnl Next: version check (cross-compile-friendly idea borrowed from autoconf)
  dnl (First check version reported by sdl2-config, then confirm
  dnl  the version in SDL.h matches it)

  if test x$pent_sdl2ok = xyes ; then

    if test ! \( \( $sdl2_major_version -ge $REQ_MAJOR \) -o \( \( $sdl2_major_version -eq $REQ_MAJOR \) -a \( \( $sdl2_minor_version -ge $REQ_MINOR \) -o \( \( $sdl2_minor_version -eq $REQ_MINOR \) -a \( $sdl2_patchlevel -ge $REQ_PATCHLEVEL \) \) \) \) \); then
      pent_sdl2ok="no, version < $REQ_VERSION found"
    else
      AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
      #include "SDL.h"

      int main()
      {
        static int test_array[1-2*!(SDL_MAJOR_VERSION==$sdl2_major_version&&SDL_MINOR_VERSION==$sdl2_minor_version&&SDL_PATCHLEVEL==$sdl2_patchlevel)];
        test_array[0] = 0;
        return 0;
      }
      ]])],,[[pent_sdl2ok="no, version of SDL.h doesn't match that of sdl2-config"]])
    fi
  fi

  dnl Next: try linking

  if test "x$pent_sdl2ok" = xyes; then
    LIBS="$LIBS $SDL2_LIBS"

    AC_LINK_IFELSE([AC_LANG_SOURCE([[
    #include "SDL.h"

    int main(int argc, char* argv[]) {
      SDL_Init(0);
      return 0;
    }
    ]])],sdl2linkok=yes,sdl2linkok=no)
    if test x$sdl2linkok = xno; then
      pent_sdl2ok=no
    fi
  fi

  AC_MSG_RESULT($pent_sdl2ok)

  LDFLAGS="$pent_backupldflags"
  CPPFLAGS="$pent_backupcppflags"
  LIBS="$pent_backuplibs"

  if test "x$pent_sdl2ok" = xyes; then
    AC_SUBST(SDL2_CFLAGS)
    AC_SUBST(SDL2_LIBS)
    ifelse([$1], , :, [$1])
  else
    ifelse([$2], , :, [$2])
  fi
]);


