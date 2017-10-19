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
	ActualPlayer=0;
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
	for(i=0;i<MAX_PLAYER;i++)
	{
		iLives[i] = 0;
		iScore[i] = 0;
		iLevel[i] = 1;
	}
	for(i=0;i<Num+1;i++)
	{
		iLives[i] = 3;
		iScore[i] = 0;
		iLevel[i] = 1;
		sprintf(sName[i],"%i",i);
	}
}


// --------------------------------------------------------------------
//	n�hster Spieler 
// --------------------------------------------------------------------

void Player::Next(void)
{
	if(++ActualPlayer >= MAX_PLAYER) ActualPlayer=0;

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
	for(int i=0;i<MAX_PLAYER;i++)
		if(iLives[i])	
		{	
			ActualPlayer=i-1;
			return(true);
		}
return(false);
}

