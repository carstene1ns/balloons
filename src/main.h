/***************************************************************************
 *   BALLOONS - prick'n'puzzle -                                           *
 *                                                                         *
 *   Copyright (C) 2007 by -S-v-N-  R.& M. Lagarden                        *
 *   svnmike@sourceforge.net                                               *
 *   eriban62@sourceforge.net                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <fstream>

// May be overwritten by build system
#ifndef DATA_PREFIX
#define DATA_PREFIX "data"
#endif

#define LEVELS DATA_PREFIX "/levels/%d.DAT"

// Tiles
static const char *tile_sets[] = {
	DATA_PREFIX "/gfx/tiles00.png",
	DATA_PREFIX "/gfx/shades.png"
};
enum { TILES00, SHADES  };

// Images
static const char *image_files[] = {
	DATA_PREFIX "/gfx/icon.png",
	DATA_PREFIX "/gfx/frame.png",
	DATA_PREFIX "/gfx/ball.png"
};
enum { ICON, FRAME, BALL_PNG };

// Fonts
static const char *font_files[] = {
	DATA_PREFIX "/fonts/font.ttf",
	DATA_PREFIX "/fonts/font.ttf"
};
enum { FONT20, FONT32 };

// Level
static const char *level_sets[] = {
	DATA_PREFIX "/levels/%d.DAT",
	DATA_PREFIX "/levels/%d.dat"
};
enum { LEVELS00, LEVELS01 };

// Player sounds
#ifndef NO_SOUND

#define NUM_SOUNDS 21

static const char *sound_files[NUM_SOUNDS] = {
	DATA_PREFIX "/sounds/s_bang.wav",
	DATA_PREFIX "/sounds/s_beep.wav",
	DATA_PREFIX "/sounds/s_bomb.wav",
	DATA_PREFIX "/sounds/s_bump.wav",
	DATA_PREFIX "/sounds/s_crash.wav",
	DATA_PREFIX "/sounds/s_door.wav",
	DATA_PREFIX "/sounds/s_drop.wav",
	DATA_PREFIX "/sounds/s_exit.wav",
	DATA_PREFIX "/sounds/s_fade.wav",
	DATA_PREFIX "/sounds/s_fall.wav",
	DATA_PREFIX "/sounds/s_furz.wav",
	DATA_PREFIX "/sounds/s_item.wav",
	DATA_PREFIX "/sounds/s_key.wav",
	DATA_PREFIX "/sounds/s_klick.wav",
	DATA_PREFIX "/sounds/s_move.wav",
	DATA_PREFIX "/sounds/s_pop.wav",
	DATA_PREFIX "/sounds/s_twi.wav",
	DATA_PREFIX "/sounds/s_type.wav",
	DATA_PREFIX "/sounds/s_wall.wav",
	DATA_PREFIX "/sounds/s_warp.wav",
	DATA_PREFIX "/sounds/s_wbrk.wav"
};
#endif

static const char *lang_files[] = {
	DATA_PREFIX "/S1.TXT",
	DATA_PREFIX "/S2.TXT",
	DATA_PREFIX "/S3.TXT",
	DATA_PREFIX "/S4.TXT"
};
enum { DE, EN, FR, NL };

#endif
