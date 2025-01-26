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

#ifndef B_OBJECT_ANIMATE_H
#define B_OBJECT_ANIMATE_H

#include "object.h"
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

#endif // B_OBJECT_ANIMATE_H
