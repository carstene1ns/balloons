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

#include "object_moving.h"
#include "game.h"


Object_Moving::Object_Moving(Game *game) : game(game)
{
	state   = ON;
	animCounter = 0;
	moveCounter = 0;
}

Object_Moving::~Object_Moving()
{
}

void Object_Moving::show(int vx, int vy)
{
	int Sx, Sy;

	Sx = x - game->map.wScrXoff;
	Sy = y - game->map.wScrYoff;

	// für das Clipping sorgt Clanlib
	game->map.DrawSprite(Sx, Sy, typ + animCounter);
}

void Object_Moving::animate(float time_elapsed)
{
	cycleCounter += time_elapsed * 72;

	if (cycleCounter > cycles)
	{
		cycleCounter = 0;
		if (animCounter < animations)
			animCounter++;
		else
			animCounter = 0;
	}
}

bool Object_Moving::checkplayerkollision(void)
{
	// Kollisionsabfrage Feind gegen Spielfigur !
	if (state == OFF)
		return(true);

	if (x - 20 > game->get_x() + game->map.wScrXoff)
		return(true);
	else if (x < game->get_x() + game->map.wScrXoff - 24)
		return(true);
	else if (y - 16 > game->get_y() + game->map.wScrYoff)
		return(true);
	else if (y < game->get_y() + game->map.wScrYoff - 20)
		return(true);

	state = OFF; // Feindsprite ausschalten
	return(false);
}

//-------------------------------------------------------------------------------
/// Class Object_Circle
//-------------------------------------------------------------------------------

Object_Circle::Object_Circle(Game *game) : Object_Moving(game)
{
	typ = SPRITE_CIRCLE;
	animations = 7;
	animCounter = (rand() & animations - 1) + 1;
	cycles = 1;
	direction = A_RIGHT;
	speed = 280;
}


bool Object_Circle::turn(float time_elapsed)
{
	animate(time_elapsed);

	moveCounter += time_elapsed * speed;
	if (moveCounter > 1)
		moveCounter = 0;
	else
		return(true);

	KollMapCell *LpKoll = game->map.pKoll;
	LpKoll = game->map.GetLocalKollPosition(x, y, true);

	switch (direction)
	{
		case A_RIGHT:
			if (y & FULL_BREAK)
			{
				y += 1;
				break;
			}
			direction = A_UP;
			LpKoll++;
			if (LpKoll->ebeneG == W_INVWALL || LpKoll->ebeneM && LpKoll->ebeneM != W_PAS_ENEMY)
				direction = A_DOWN;
			else
			{
				if (x & HALF_BREAK)
					direction = A_RIGHT;
				else
					x += 1;
			}
			break;

		case A_LEFT:
			if (y & FULL_BREAK)
			{
				y -= 1;
				break;
			}
			direction = A_DOWN;
			LpKoll--;
			if (LpKoll->ebeneG == W_INVWALL || LpKoll->ebeneM && LpKoll->ebeneM != W_PAS_ENEMY)
				direction = A_UP;
			else
			{
				if (x & HALF_BREAK)
					direction = A_LEFT;
				else
					x -= 1;
			}
			break;

		case A_DOWN:
			if (x & FULL_BREAK)
			{
				x -= 1;
				break;
			}
			direction = A_RIGHT;
			LpKoll += game->map.wKollLine;
			if (LpKoll->ebeneG == W_INVWALL || LpKoll->ebeneM && LpKoll->ebeneM != W_PAS_ENEMY)
				direction = A_LEFT;
			else
			{
				if (y & HALF_BREAK)
					direction = A_DOWN;
				else
					y += 1;
			}
			break;

		case A_UP:
			if (x & FULL_BREAK)
			{
				x += 1;
				break;
			}
			direction = A_LEFT;
			LpKoll -= game->map.wKollLine;
			if (LpKoll->ebeneG == W_INVWALL || LpKoll->ebeneM && LpKoll->ebeneM != W_PAS_ENEMY)
				direction = A_RIGHT;
			else
			{
				if (y & HALF_BREAK)
					direction = A_UP;
				else
					y -= 1;
			}
			break;
	}

	return(checkplayerkollision());
}



//-------------------------------------------------------------------------------
/// Class Object_UpDown
//-------------------------------------------------------------------------------

Object_UpDown::Object_UpDown(Game *game) : Object_Moving(game)
{
	typ = SPRITE_UPDOWN;
	animations = 7;
	animCounter = (rand() & animations - 1) + 1;
	cycles = 3;
	direction = 0;
	speed = 300;
}


bool Object_UpDown::turn(float time_elapsed)
{
	animate(time_elapsed);

	moveCounter += time_elapsed * speed;
	if (moveCounter > 1)
		moveCounter = 0;
	else
		return(true);

	KollMapCell *LpKoll = game->map.pKoll;

	if (direction == A_UP)
	{
		y -= 1;
		LpKoll = game->map.GetLocalKollPosition(x, y, true);
		if (LpKoll->ebeneG == W_INVWALL || LpKoll->ebeneM && LpKoll->ebeneM != W_PAS_ENEMY)
			direction = A_DOWN;
	}
	else
	{
		y += 1;
		LpKoll = game->map.GetLocalKollPosition(x, (FULL_BREAK + y), true);
		if (LpKoll->ebeneG == W_INVWALL || LpKoll->ebeneM && LpKoll->ebeneM != W_PAS_ENEMY)
			direction = A_UP;
	}

	return(checkplayerkollision());
}

//-------------------------------------------------------------------------------
/// Class Object_Door
//-------------------------------------------------------------------------------

Object_Door::Object_Door(Game *game) : Object_Moving(game)
{
	animations = 19;
	cycles = 4;
	cycleCounter = (rand() & cycles) + 1;
	phase = (rand() & 199) + 1;
	speed = 72;
}

bool Object_Door::turn(float time_elapsed)
{
	moveCounter += time_elapsed * speed;
	if (moveCounter > 1)
		moveCounter = 0;
	else
		return(true);

	if (state == ON)
	{
		if (cycleCounter <= 0)
		{
			cycleCounter = cycles;
			if (animCounter != animations)
				animCounter++;
			else
				animCounter = 0;
		}
		else
			cycleCounter--;

		if (animCounter == 19)
		{
			state = OFF;
			return(true);
		}
	}
	else if (state == BUSY) // durch Schalter aus ?
	{
		animCounter = 0;
		return(true);
	}

	if (phase < 200) // Phasenzaehler schon 200
		phase += 1;
	else
	{
		phase = 0;
		state = ON; // Tür einschalten
		animCounter = 0;
	}

	return(checkplayerkollision());
}

//-------------------------------------------------------------------------------
/// Class Object_Ball
//-------------------------------------------------------------------------------

#define BALLSPEED 2 // Ball-Speed
#define BALLBREAK 30

Object_Ball::Object_Ball(Game *game) : Object_Moving(game)
{
	typ = SPRITE_BALL;
	state = OFF;
	animations = 7;
	cycles = 2;
	speed = 400;
}

void Object_Ball::show(int vx, int vy)
{
	if (state != ON)
		return;
	Object_Moving::show(vx, vy);
}


bool Object_Ball::turn(float time_elapsed)
{
	if (state != ON)
		return(true);

	KollMapCell *LpKoll = game->map.pKoll;

	animate(time_elapsed);

	moveCounter += time_elapsed * speed;
	if (moveCounter > 1)
		moveCounter = 0;
	else
		return(true);

	switch (direction)
	{
		case A_RIGHT: // Ball nach rechts

TestRight:
		x += BALLSPEED;
		if (x & BALLBREAK)
			break;
		LpKoll = game->map.GetLocalKollPosition(x, y, true);
		switch (LpKoll->ebeneM)
		{
			case W_MIRROR:
			case MB_MIRROR:
				direction = A_LEFT;
				game->PlayWave(S_WALL);
				break;

			case W_MIRROR + 1:
			case MB_MIRROR + 1:
				direction = A_UP;
				game->PlayWave(S_WALL);
				goto TestUp;

			case W_MIRROR + 2:
			case MB_MIRROR + 2:
			case W_PAS_ENEMY:
				break;

			case W_MIRROR + 3:
			case MB_MIRROR + 3:
				direction = A_DOWN;
				game->PlayWave(S_WALL);
				goto TestDown;

			default:
				if (!LpKoll->ebeneM)
					break;
				if (LpKoll->ebeneM == W_DEST)
				{
					game->map.wKollX = (x >> 5);
					game->map.wKollY = (y >> 5);
					game->DestW2();
					state = OFF;
					break;
				}
				game->PlayWave(S_FURZ);
				state = OFF;
				break;
		} // switch Ball rechts
		break;

	case A_LEFT: // Ball nach links

TestLeft:
		x -= BALLSPEED;
		if (x & BALLBREAK)
			break;
		LpKoll = game->map.GetLocalKollPosition(x, y, true);
		switch (LpKoll->ebeneM)
		{
			case W_MIRROR:
			case MB_MIRROR:
				direction = A_RIGHT;
				game->PlayWave(S_WALL);
				break;

			case W_MIRROR + 1:
			case MB_MIRROR + 1:
				direction = A_DOWN;
				game->PlayWave(S_WALL);
				goto TestDown;

			case W_MIRROR + 2:
			case MB_MIRROR + 2:
			case W_PAS_ENEMY:
				break;

			case W_MIRROR + 3:
			case MB_MIRROR + 3:
				direction = A_UP;
				game->PlayWave(S_WALL);
				goto TestUp;

			default:
				if (!LpKoll->ebeneM)
					break;
				if (LpKoll->ebeneM == W_DEST)
				{
					game->map.wKollX = (x >> 5);
					game->map.wKollY = (y >> 5);
					game->DestW2();
					state = OFF;
					break;
				}
				game->PlayWave(S_FURZ);
				state = OFF;
				break;
		} // switch Ball links
		break;

	case A_UP: // Ball nach oben

TestUp:
		y -= BALLSPEED;
		if (y & BALLBREAK)
			break;
		LpKoll = game->map.GetLocalKollPosition(x, y, true);
		switch (LpKoll->ebeneM)
		{
			case W_MIRROR:
			case MB_MIRROR:
			case W_PAS_ENEMY:
				break;

			case W_MIRROR + 1:
			case MB_MIRROR + 1:
				direction = A_RIGHT;
				game->PlayWave(S_WALL);
				goto TestRight;

			case W_MIRROR + 2:
			case MB_MIRROR + 2:
				direction = A_DOWN;
				game->PlayWave(S_WALL);
				break;

			case W_MIRROR + 3:
			case MB_MIRROR + 3:
				direction = A_LEFT;
				game->PlayWave(S_WALL);
				goto TestLeft;

			default:
				if (!LpKoll->ebeneM)
					break;
				if (LpKoll->ebeneM == W_DEST)
				{
					game->map.wKollX = (x >> 5);
					game->map.wKollY = (y >> 5);
					game->DestW2();
					state = OFF;
					break;
				}
				game->PlayWave(S_FURZ);
				state = OFF;
				break;
		} // switch Ball oben
		break;

	case A_DOWN: // Ball nach unten

TestDown:
		y += BALLSPEED;
		if (y & BALLBREAK)
			break;
		LpKoll = game->map.GetLocalKollPosition(x, y, true);

		switch (LpKoll->ebeneM)
		{
			case W_MIRROR:
			case MB_MIRROR:
			case W_PAS_ENEMY:
				break;

			case W_MIRROR + 1:
			case MB_MIRROR + 1:
				direction = A_LEFT;
				game->PlayWave(S_WALL);
				goto TestLeft;

			case W_MIRROR + 2:
			case MB_MIRROR + 2:
				direction = A_UP;
				game->PlayWave(S_WALL);
				break;

			case W_MIRROR + 3:
			case MB_MIRROR + 3:
				direction = A_RIGHT;
				game->PlayWave(S_WALL);
				goto TestRight;

			default:
				if (!LpKoll->ebeneM)
					break;
				if (LpKoll->ebeneM == W_DEST)
				{
					game->map.wKollX = (x >> 5);
					game->map.wKollY = (y >> 5);
					game->DestW2();
					state = OFF;
					break;
				}
				game->PlayWave(S_FURZ);
				state = OFF;
				break;
		} // switch Ball unten
		break;
	} // switch

	return(true);
}

//	---------------------------------------------------------
//	Ball starten
//	---------------------------------------------------------

bool Object_Ball::StartBall(KollMapCell *Koll, KollMapCell *EpKoll)
{
	if (state == ON)
		return(false);

	state = ON;
	x = Koll->vbindX << 5;
	y = Koll->vbindY << 5;
	animCounter = 0;
	moveCounter = 0;

	switch (EpKoll->ebeneM)
	{
		case W_WERFER:
			direction = A_RIGHT;
			break;
		case W_WERFER + 1:
			direction = A_LEFT;
			break;
		case W_WERFER + 2:
			direction = A_DOWN;
			break;
		case W_WERFER + 3:
			direction = A_UP;
			break;
	}

	return true;
}
