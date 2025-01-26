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

#include "object_animate.h"
#include "game.h"

Object_Animate::Object_Animate()
{
	counter = new Timer(100);
	mode = 0;
	item = 0;
}

Object_Animate::~Object_Animate()
{
	delete counter;
}

void Object_Animate::show(int center_x, int center_y)
{
	draw();
}

bool Object_Animate::turn(float time_elapsed)
{
	return(true);
}

//	---------------------------------------------------------
//		Subroutine:     Ausschnitt restaurieren mit
//													oder ohne Animation
//	---------------------------------------------------------

void Object_Animate::draw(void)
{
	switch (mode)
	{
		case RM_DONE:
			return;

		case RM_ANIMATE:
		case RM_SEARCH:
			if (counter->elapsed())
			{
				counter->reset();

				if (anim != max_anim) // letzter Animationsschritt ?
					anim++; // naechster Animationsschritt
				else
				{
					if (mode == RM_SEARCH)
					{
						if (item) koll->ebeneG = item;
						search();
					}
					else
						mode = RM_DRAW; // Animation beenden
					return;
				}
			}

		case RM_DRAW:
			if (mode >= RM_DRAW)
				game->map.DrawTile((x - game->map.wScrXoff), (y - game->map.wScrYoff), (tile + anim));

			if (mode <= RM_DRAW)
			{
				mode--;
				anim = 0;
			}
			return;
	}
}

//	--------------------------------------------------------------------------
//		Subroutine:   Explodierende Features animativ
//												darstellen und ggf. weitersuchen
//												nach zerstörbaren Wänden oder Bomben
//	--------------------------------------------------------------------------

void Object_Animate::search()
{
	int Rx, Ry;

	mode = CLEAR; // weitersuchen-Flag löschen

	// aktuelle koll.-Position
	koll = game->map.GetLocalKollPosition(x, y, true);

	Rx = x;
	Ry = y;

	if (koll->ebeneG == I_BOMB) // 1. Position Bombe gefunden
		goto TstBomb;

	koll--; //  links daneben
	Rx -= RASTER;
	if (koll->ebeneM == W_DEST) // Destroiwall gefunden
		goto TstWall;
	if (koll->ebeneG == I_BOMB) // Bombe
		goto TstBomb;

	koll -= game->map.wKollLine - 1; // oben darüber
	Rx += RASTER;
	Ry -= RASTER;
	if (koll->ebeneM == W_DEST) // Destroiwall gefunden
		goto TstWall;
	if (koll->ebeneG == I_BOMB) // Bombe
		goto TstBomb;

	koll += game->map.wKollLine + 1; // rechts daneben
	Rx += RASTER;
	Ry += RASTER;
	if (koll->ebeneM == W_DEST) // Destroiwall gefunden
		goto TstWall;
	if (koll->ebeneG == I_BOMB) // Bombe
		goto TstBomb;

	koll += game->map.wKollLine - 1; // unten darunter
	Rx -= RASTER;
	Ry += RASTER;
	if (koll->ebeneM == W_DEST) // Destroiwall gefunden
		goto TstWall;
	if (koll->ebeneG == I_BOMB) // Bombe
		goto TstBomb;

	return;

TstWall:
	// Mauer-Eintraege löschen
	change_map(Rx, Ry, CLEAR, NOTHING, CLEAR);
	item = NOTHING;

	if (mode) // im sichtbaren Bereich
	{
		max_anim = 4;
		tile = A_BRKWALL;
		counter->intervall = 75;
	}
	mode = RM_SEARCH;

	game->PlayWave(S_CRASH, 0.3f); // 30%
	return;

TstBomb:

	// Bomben-Einträge löschen
	// und schwarzes Loch eintragen
	change_map(Rx, Ry, 0x03C0, CLEAR, NOTHING);
	item = I_LOCH;

	if (mode) // im sichtbaren Bereich
	{
		max_anim = 4;
		tile = A_BRKGRD;
		counter->intervall = 75;
	}
	mode = RM_SEARCH;

	game->PlayWave(S_BOMB);
	return;
}

//	---------------------------------------------------------
//		Subroutine:     Redraw setzen
//												diverse Eintraege loeschen
//	---------------------------------------------------------

void Object_Animate::change_map(int Sx, int Sy, short int Mask, unsigned char Item, unsigned char Wall)
{
	koll = game->map.GetLocalKollPosition(Sx, Sy, true);

	if (Mask != NOTHING)
		koll->mask = Mask;
	if (Item != NOTHING)
		koll->ebeneG = Item;
	if (Wall != NOTHING)
		koll->ebeneM = Wall;

	if (!mode) mode = RM_DRAW;
	x = Sx & ~31;
	y = Sy & ~31;
	anim = 0;
	max_anim = 0;
}
