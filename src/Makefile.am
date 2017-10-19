bin_PROGRAMS = balloons
balloons_SOURCES = game.cpp game.h game_world.cpp game_world.h main.cpp main.h \
	map.cpp map.h obj.h obj_animate.cpp obj_animate.h obj_mov.cpp obj_mov.h \
	player.cpp player.h scoreList.cpp scoreList.h tiledef.h tiletab.h util.cpp util.h

# set the include path found by configure
AM_CPPFLAGS =  $(LIBSDL_CFLAGS) $(all_includes)

# the library search path.
balloons_LDFLAGS = $(all_libraries) $(LIBSDL_RPATH)
balloons_LDADD = $(LIBSDL_LIBS) -lSDL_image -lSDL_ttf -lSDL_mixer
