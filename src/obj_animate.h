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

#ifndef header_object_animate
#define header_object_animate

#if _MSC_VER > 1000
#pragma once
#endif

#include "obj.h"
#include "map.h"

#define OBJ_ANIMATE 100

#define CLEAR   0
#define NOTHING 255

#define RM_DONE    0
#define RM_DRAW    1
#define RM_ANIMATE 2
#define RM_SEARCH  5

class Game;

class Object_Animate : public Object
{
public:
	Object_Animate();
	virtual ~Object_Animate();

	virtual int get_type()  {
		return OBJ_ANIMATE;
	}

	virtual int get_x() {
		return x;
	}
	virtual int get_y() {
		return y;
	}

	virtual void show(int view_x = 0, int view_y = 0);
	virtual bool turn(float time_elapsed);

	Game *game;

	int mode; // Mode (zeichnen/kopieren/animieren/weitersuchen)
	int x;
	int y;
	Timer *counter;
	short int tile; // 1.Tile der Sequenz
	int max_anim; // Anzahl Animationen

	// Werte in der Kollisions-Map ändern
	void change_map(int Sx, int Sy, short int Mask, unsigned char Item, unsigned char Wall);

private:
	KollMapCell *koll;
	int anim;
	short int item; // wird nach der Animation gesetzt
	int itx, ity;

	// zeichne Animation
	void draw();
	// suche nach zerstoerbaren Wänden oder Bomben
	void search();
};

#endif
