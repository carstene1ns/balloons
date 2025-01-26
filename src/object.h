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

#ifndef B_OBJECT_H
#define B_OBJECT_H

#include "util.h"
#include "tiledef.h"

class Object
{
public:
	virtual ~Object() {
		return;
	}

	virtual int get_type() = 0;
	virtual void show(int view_x = 0, int view_y = 0) = 0;
	virtual bool turn(float time_elapsed) = 0;

private:
	int obj;
};

#endif // B_OBJECT_H
