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

// Tile-Definitionen
#define I_HAMMER      6
#define I_KEY         7
#define I_LIVE        8
#define I_TIME        9
#define I_CODE        10
#define I_KILL        11
#define I_BOMB        12
#define I_INVRISS     13
#define I_LOCH        18
#define I_PLACK       19
#define I_TELEPORT    32
#define I_BLINK       35
#define I_EXIT        38
#define MBHOLZ        40
#define OW1           41
#define OW2           42
#define OW3           43
#define OW4           44
#define OW5           45
#define OW6           46
#define W_DEST        47
#define MBLOCH        56
#define W_NORM        82
#define W_B_OL        87
#define W_B_OR        88
#define W_B_UL        89
#define W_B_UR        90
#define W_B_OM        95
#define W_B_UM        96
#define W_MIRROR      112
#define MB_MIRROR     116
#define W_INVWALL     120
#define W_INVBUMP     121
#define W_KILL_ENEMY  122
#define W_PAS_ENEMY   123
#define W_FLIPFLOP    124
#define W_FLIPPLACE   129
#define W_BUMPWALL    130
#define W_SMALL_VERT  132
#define W_SMALL_HORIZ 133
#define MBNORM        135
#define W_KILL        136
#define W_KEYLOCK     137
#define W_CLICKON     138
#define W_CLICKOFF    139
#define UG_ICE        155
#define W_WERFER      160
#define I_ICE         176
#define UG_PLATTE     220
#define UG_HOLZ       234
#define I_BALLOON     235

// -------------------------------
// Sprites und Animationen
// und Flags etc.
// -------------------------------

// Spritenummern aus Levelstruktur
#define SPR_FIGUR  240
#define SPR_UPDOWN 241
#define SPR_CIRCLE 242
#define SPR_H_DOOR 243
#define SPR_V_DOOR 244

// Nummern in Tiles.png
#define SPRITE_FIGUR  260
#define SPRITE_UPDOWN 300
#define SPRITE_CIRCLE 310
#define SPRITE_H_DOOR 440
#define SPRITE_V_DOOR 460

#define SPRITE_BALL 20 * 19

// Animationen ab 31.03.2007
#define A_FIGUR      260
#define A_FIGUR_R    260
#define A_FIGUR_L    265
#define A_FIGUR_D    270
#define A_FIGUR_U    275
#define A_FIGUR_BRK  280
#define A_FIGUR_FALL 288
#define A_BRKWALL    20 * 21
#define A_BOMB       20 * 20 + 5
#define A_BRKGRD     20 * 21 + 5
#define A_FLIPFLOP   20 * 19 + 10
#define A_BALLBRK    20 * 20
#define A_PLATTE     20 * 20 + 10
#define A_FADE       20 * 21 + 10

// diverse Flags
#define F_MOVEDOOR   2
#define F_OPEN       4
#define F_MOVE       8
#define F_DEST       16
#define FLAG_BALLOON 128

// Tile-Raster in Pixeln
#define RASTER 32

// Definitionen Ende

// Tile-Types
#define TYPE_ICE         2
#define TYPE_BLACKHOLE   3
#define TYPE_WALL        4
#define TYPE_INVBUMP     5
#define TYPE_MOVEABLE    129
#define TYPE_DESTROYABLE 130
#define TYPE_MIRROR      131
#define TYPE_KILLENEMY   133
#define TYPE_FLIPFLOP    134
#define TYPE_CHANGER     135
#define TYPE_BUMPWALL    136
#define TYPE_KILLWALL    137
#define TYPE_KEYLOCK     138
#define TYPE_CLICKWALL   139
