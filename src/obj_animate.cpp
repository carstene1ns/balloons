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

#include "obj_animate.h"
#include "game.h"


Object_Animate::Object_Animate()
{
	counter = new Timer(100);
	mode =0;
	item=0;
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
//		Subroutine:   	Ausschnitt restaurieren mit
//													oder ohne Animation
//	---------------------------------------------------------

void Object_Animate::draw(void)
{
		switch(mode)
		{
			case 	RM_DONE:				
							
						return;

			case 	RM_ANIMATE:
			case 	RM_SEARCH:

						if(counter->elapsed())	
						{
							counter->reset();

							if(anim != max_anim)		// letzter Animationsschritt ?
								anim++;											// naechster Animationsschritt
							else
							{
								if(mode==RM_SEARCH)
								{
									if(item) 	koll->ebeneG=item;
									search();
								}
								else
									mode=RM_DRAW;		// Animation beenden
								return;
							}
						}
						
			case RM_DRAW:

							if(mode >= RM_DRAW)
								game->map.DrawTile(	(x-game->map.wScrXoff), (y-game->map.wScrYoff), 	(tile+ anim));
								
							if(mode<=RM_DRAW)
							{
								mode--;
								anim=0;
							}

							return;
		} // switch

}


//	--------------------------------------------------------------------------
//		Subroutine:   Explodierende Features animativ
//												darstellen und ggf. weitersuchen
//												nach zerstoerbaren Waenden oder Bomben
//	--------------------------------------------------------------------------

void Object_Animate::search()
{
int Rx,Ry;

	mode=CLEAR; // weitersuchen-Flag loeschen

	// aktuelle koll.-Position
	koll = game->map.GetLocalKollPosition(x, y,true);

	Rx = x;
	Ry = y;

	if( koll->ebeneG == I_BOMB)			//  1. Position Bombe gefunden
		goto TstBomb;

	koll--;													//  links daneben
	Rx-=RASTER;
	if( koll->ebeneM == W_DEST)			//  Destroiwall gefunden
		goto TstWall;
	if( koll->ebeneG == I_BOMB)			//  Bombe
		goto TstBomb;

	koll-= game->map.wKollLine-1;							//  oben darber
	Rx+=RASTER;
	Ry-=RASTER;
	if( koll->ebeneM == W_DEST)			//  Destroiwall gefunden
		goto TstWall;
	if( koll->ebeneG == I_BOMB)			//  Bombe
		goto TstBomb;

	koll+= game->map.wKollLine+1;							//  rechts daneben
	Rx+=RASTER;
	Ry+=RASTER;
	if( koll->ebeneM == W_DEST)			//  Destroiwall gefunden
		goto TstWall;
	if( koll->ebeneG == I_BOMB)			//  Bombe
		goto TstBomb;

	koll+= game->map.wKollLine-1;							//  unten darunter
	Rx-=RASTER;
	Ry+=RASTER;
	if( koll->ebeneM == W_DEST)			//  Destroiwall gefunden
		goto TstWall;
	if( koll->ebeneG == I_BOMB)			//  Bombe
		goto TstBomb;

	return;

TstWall:
	//  Mauer-Eintraege loeschen
	change_map(Rx,Ry,CLEAR,NOTHING,CLEAR);
	item = NOTHING;


	if(mode)						//  im sichtbaren Bereich
	  {
			max_anim=4;
			tile= A_BRKWALL;
			counter->intervall= 75;
		}
	mode= 	RM_SEARCH;

	game->PlayWave(S_CRASH, 0.3f); //30%
	return;

TstBomb:

	//  Bomben-Eintraege loeschen
	//  und schwarzes Loch eintragen
	change_map(Rx,Ry,0x03C0,CLEAR,NOTHING);
	item = I_LOCH;
	
	if(mode)				//  im sichtbaren Bereich
	  {
			max_anim=4;
			tile= A_BRKGRD;
			counter->intervall= 75;
		}
	mode= 	RM_SEARCH;

	game->PlayWave(S_BOMB);
	return;
}

//	---------------------------------------------------------
//		Subroutine: 	Redraw setzen
//												diverse Eintraege loeschen
//	---------------------------------------------------------

void Object_Animate::change_map(int Sx,int Sy, short int Mask, unsigned char Item,unsigned char Wall)
{

	koll=game->map.GetLocalKollPosition(Sx, Sy,true);

	if(Mask != NOTHING) koll->mask			=Mask;
	if(Item != NOTHING) 	koll->ebeneG=Item;
	if(Wall != NOTHING) 	koll->ebeneM=Wall;

	if(!mode) mode=RM_DRAW;
	x = Sx&~31;
	y = Sy&~31;
	anim=0;
	max_anim=0;
}
