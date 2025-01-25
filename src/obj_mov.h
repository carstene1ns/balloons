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

#ifndef header_object_moving
#define header_object_moving

#include "obj.h"
#include "map.h"

#define OFF 0 // State
#define ON  1

#define A_LEFT  4
#define A_RIGHT 8
#define A_UP    1
#define A_DOWN  2

#define FULL_BREAK 31
#define HALF_BREAK 16

#define OBJ_MOVING 200

class Game;

class Object_Moving : public Object
{
public:
	Object_Moving(Game *game);
	virtual ~Object_Moving();

	Game *game;

	virtual void show(int view_x = 0, int view_y = 0);

	int get_x() const {
		return x;
	}
	int get_y() const {
		return y;
	}
	void set_x(int nx) {
		x = nx;
	}
	void set_y(int ny) {
		y = ny;
	}
	void set_speed(float new_speed) {
		speed = new_speed;
	}

	int state; // Status des Sprites
	int typ; // Sprite-Typ

protected:

	int x, y;
	int animations; // Anzahl Animationen
	int animCounter; // Animations-Geschwindigkeit
	int cycles;
	float cycleCounter; // Zyklenzähler
	int direction;
	float moveCounter; // Pixel-Geschwindigkeit
	float speed; // Geschwindigkeit

	void animate(float time_elapsed);
	virtual bool checkplayerkollision(void);
};

//-------------------------------------------------------------------------------
/// Class Object_Ball
//-------------------------------------------------------------------------------

#define OBJ_BALL 210

class Object_Ball : public Object_Moving
{
public:
	Object_Ball(Game *game);

	virtual int get_type() {
		return OBJ_BALL;
	}
	virtual void show(int view_x = 0, int view_y = 0);
	virtual bool turn(float time_elapsed);

	bool StartBall(KollMapCell *Koll, KollMapCell *EpKoll);
};

//-------------------------------------------------------------------------------
/// Class Object_Circle
//-------------------------------------------------------------------------------

#define OBJ_CIRCLE 220

class Object_Circle : public Object_Moving
{
public:
	Object_Circle(Game *game);

	virtual int get_type() {
		return OBJ_CIRCLE;
	}
	virtual bool turn(float time_elapsed);
};

//-------------------------------------------------------------------------------
/// Class Object_UpDown
//-------------------------------------------------------------------------------

#define OBJ_UPDOWN 230

class Object_UpDown : public Object_Moving
{
public:
	Object_UpDown(Game *game);

	virtual int get_type() {
		return OBJ_UPDOWN;
	}
	virtual bool turn(float time_elapsed);
};

//-------------------------------------------------------------------------------
/// Class Object_Door
//-------------------------------------------------------------------------------

#define OBJ_DOOR 290
#define BUSY 2

class Object_Door : public Object_Moving
{
public:
	Object_Door(Game *game);

	virtual int get_type() {
		return OBJ_DOOR;
	}
	virtual bool turn(float time_elapsed);

private:
	int phase; // Phasenzaehler
};

#endif
