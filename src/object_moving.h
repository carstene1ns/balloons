/***************************************************************************
 *   BALLOONS - prick'n'puzzle -                                           *
 *                                                                         *
 *   Copyright (C) 2007 by -S-v-N-  R.& M. Lagarden                        *
 *   See AUTHORS file for full author information.                         *
 *                                                                         *
 *   Balloons is free software under the GNU General Public License        *
 *   (version 2 or later).                                                 *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with balloons; if not, see <http://www.gnu.org/licenses/>.      *
 ***************************************************************************/

#ifndef B_OBJECT_MOVING_H
#define B_OBJECT_MOVING_H

#include "object.h"
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

#endif // B_OBJECT_MOVING_H
