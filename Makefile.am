
# development builds use local data files
if NO_DEVBUILD
AM_CPPFLAGS = -DDATA_PREFIX=\"$(pkgdatadir)\"
endif

# src
bin_PROGRAMS = balloons
balloons_SOURCES = \
	src/main.cpp \
	src/main.h \
	src/game.cpp \
	src/game.h \
	src/game_world.cpp \
	src/game_world.h \
	src/map.cpp \
	src/map.h \
	src/obj.h \
	src/obj_animate.cpp \
	src/obj_animate.h \
	src/obj_mov.cpp \
	src/obj_mov.h \
	src/player.cpp \
	src/player.h \
	src/scoreList.cpp \
	src/scoreList.h \
	src/tiledef.h \
	src/tiletab.h \
	src/util.cpp \
	src/util.h
balloons_CXXFLAGS = \
	$(SDLMIXER_CFLAGS) \
	$(SDLIMAGE_CFLAGS) \
	$(SDLTTF_CFLAGS) \
	$(SDL_CFLAGS)
balloons_LDADD = \
	$(SDLMIXER_LIBS) \
	$(SDLIMAGE_LIBS) \
	$(SDLTTF_LIBS) \
	$(SDL_LIBS)

# data
dist_pkgdata_DATA = \
	data/S1.TXT \
	data/S2.TXT \
	data/S3.TXT \
	data/S4.TXT

fontsdir = $(pkgdatadir)/fonts
dist_fonts_DATA = \
	data/fonts/font20.png \
	data/fonts/font32.png \
	data/fonts/font.ttf

gfxdir = $(pkgdatadir)/gfx
dist_gfx_DATA = \
	data/gfx/ball.png \
	data/gfx/frame.png \
	data/gfx/icon.png \
	data/gfx/shades.png \
	data/gfx/tiles00.png

levelsdir = $(pkgdatadir)/levels
dist_levels_DATA = \
	data/levels/1.DAT \
	data/levels/2.DAT \
	data/levels/3.DAT \
	data/levels/4.DAT \
	data/levels/5.DAT \
	data/levels/6.DAT \
	data/levels/7.DAT \
	data/levels/8.DAT \
	data/levels/9.DAT \
	data/levels/10.DAT \
	data/levels/11.DAT \
	data/levels/12.DAT \
	data/levels/13.DAT \
	data/levels/14.DAT \
	data/levels/15.DAT \
	data/levels/16.DAT \
	data/levels/17.DAT \
	data/levels/18.DAT \
	data/levels/19.DAT \
	data/levels/20.DAT \
	data/levels/21.DAT \
	data/levels/22.DAT \
	data/levels/23.DAT \
	data/levels/24.DAT \
	data/levels/25.DAT \
	data/levels/26.DAT \
	data/levels/27.DAT \
	data/levels/28.DAT \
	data/levels/29.DAT \
	data/levels/30.DAT \
	data/levels/31.DAT \
	data/levels/32.DAT \
	data/levels/33.DAT \
	data/levels/34.DAT \
	data/levels/35.DAT \
	data/levels/36.DAT \
	data/levels/37.DAT \
	data/levels/38.DAT \
	data/levels/39.DAT \
	data/levels/40.DAT \
	data/levels/41.DAT \
	data/levels/42.DAT \
	data/levels/43.DAT \
	data/levels/44.DAT \
	data/levels/45.DAT \
	data/levels/46.DAT \
	data/levels/47.DAT \
	data/levels/48.DAT \
	data/levels/49.DAT \
	data/levels/50.DAT \
	data/levels/51.DAT \
	data/levels/52.DAT \
	data/levels/53.DAT \
	data/levels/54.DAT \
	data/levels/55.DAT \
	data/levels/56.DAT \
	data/levels/57.DAT \
	data/levels/58.DAT \
	data/levels/59.DAT \
	data/levels/60.DAT \
	data/levels/61.DAT \
	data/levels/62.DAT \
	data/levels/63.DAT \
	data/levels/64.DAT \
	data/levels/65.DAT \
	data/levels/66.DAT \
	data/levels/67.DAT \
	data/levels/68.DAT \
	data/levels/69.DAT \
	data/levels/70.DAT \
	data/levels/71.DAT \
	data/levels/72.DAT \
	data/levels/73.DAT \
	data/levels/74.DAT \
	data/levels/75.DAT \
	data/levels/76.DAT \
	data/levels/77.DAT \
	data/levels/78.DAT \
	data/levels/79.DAT \
	data/levels/80.DAT \
	data/levels/81.DAT \
	data/levels/82.DAT \
	data/levels/83.DAT \
	data/levels/84.DAT \
	data/levels/85.DAT \
	data/levels/86.DAT \
	data/levels/87.DAT \
	data/levels/88.DAT \
	data/levels/89.DAT \
	data/levels/90.DAT \
	data/levels/91.DAT \
	data/levels/92.DAT \
	data/levels/93.DAT \
	data/levels/94.DAT \
	data/levels/95.DAT \
	data/levels/96.DAT \
	data/levels/97.DAT \
	data/levels/98.DAT \
	data/levels/99.DAT

soundsdir = $(pkgdatadir)/sounds
dist_sounds_DATA = \
	data/sounds/s_bang.wav \
	data/sounds/s_beep.wav \
	data/sounds/s_bomb.wav \
	data/sounds/s_bump.wav \
	data/sounds/s_crash.wav \
	data/sounds/s_door.wav \
	data/sounds/s_drop.wav \
	data/sounds/s_exit.wav \
	data/sounds/s_fade.wav \
	data/sounds/s_fall.wav \
	data/sounds/s_furz.wav \
	data/sounds/s_item.wav \
	data/sounds/s_key.wav \
	data/sounds/s_klick.wav \
	data/sounds/s_move.wav \
	data/sounds/s_pop.wav \
	data/sounds/s_twi.wav \
	data/sounds/s_type.wav \
	data/sounds/s_wall.wav \
	data/sounds/s_warp.wav \
	data/sounds/s_wbrk.wav

dist_doc_DATA = \
	doc/index.html \
	doc/BALL.gif \
	doc/BEAMER.gif \
	doc/BLINKER.gif \
	doc/BLITZ.gif \
	doc/BOMB.gif \
	doc/CLOCK.gif \
	doc/CODE.gif \
	doc/EINBAHN.gif \
	doc/EXIT.gif \
	doc/FLIESE.gif \
	doc/HALT.gif \
	doc/HAMMER.gif \
	doc/ITEM.gif \
	doc/KEY.gif \
	doc/LIVE.gif \
	doc/LOCHST.gif \
	doc/PALETTE.gif \
	doc/PFEIL.gif \
	doc/RISS.gif \
	doc/SNOW.gif \
	doc/SWITCH.gif \
	doc/UMLENKER.gif \
	doc/WERFER.gif
