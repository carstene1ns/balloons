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

#include <cstdio>
#include "player.h"

// --------------------------------------------------------------------
//	Konstruktor
// --------------------------------------------------------------------

Player::Player()
{
	Lives = &iLives[0];
	Score = &iScore[0];
	Level = &iLevel[0];
	ActualPlayer = 0;
}

// --------------------------------------------------------------------
//	Destruktor
// --------------------------------------------------------------------

Player::~Player()
{

}

// --------------------------------------------------------------------
//	Initialisierung aller Spieler
// --------------------------------------------------------------------

void Player::Init(int Num)
{
	int i;
	for (i = 0; i < MAX_PLAYER; i++)
	{
		iLives[i] = 0;
		iScore[i] = 0;
		iLevel[i] = 1;
	}
	for (i = 0; i < Num + 1; i++)
	{
		iLives[i] = 3;
		iScore[i] = 0;
		iLevel[i] = 1;
		sprintf(sName[i], "%i", i);
	}
}


// --------------------------------------------------------------------
//	nächster Spieler
// --------------------------------------------------------------------

void Player::Next(void)
{
	if (++ActualPlayer >= MAX_PLAYER)
		ActualPlayer = 0;

	Lives = &iLives[ActualPlayer];
	Score = &iScore[ActualPlayer];
	Level = &iLevel[ActualPlayer];
	Name  = sName[ActualPlayer];
}


// --------------------------------------------------------------------
//	suche nach Spieler der noch Leben hat
// --------------------------------------------------------------------

bool Player::Check(void)
{
	for (int i = 0; i < MAX_PLAYER; i++)
		if (iLives[i])
		{
			ActualPlayer = i - 1;
			return(true);
		}
	return(false);
}
