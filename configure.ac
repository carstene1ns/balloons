AC_PREREQ([2.69])
AC_INIT([balloons], [0.2.0], [https://github.com/carstene1ns/balloons/issues],
  [balloons], [https://github.com/carstene1ns/balloons])
AM_INIT_AUTOMAKE([1.11 foreign subdir-objects silent-rules])
AC_CONFIG_HEADERS([src/config.h])

AC_PROG_CXX
AC_PROG_CPP
PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES([SDL], [sdl])
PKG_CHECK_MODULES([SDLIMAGE], [SDL_image])
PKG_CHECK_MODULES([SDLMIXER], [SDL_mixer >= 1.2.5])
PKG_CHECK_MODULES([SDLTTF], [SDL_ttf >= 2.0.6])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
AC_MSG_RESULT([
   Installation prefix:      $prefix
   C++ compiler & flags:     $CXX $CXXFLAGS
   Linker flags & libraries: $LDFLAGS $LIBS

BALLOONS configured, you can now run `make`...])
