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

#include "map.h"
#include "game.h"
#include "main.h"

#include "tiletab.h"
#include "tiledef.h"

using std::list;

// --------------------------------------------------------------------
//	Konstruktion/Destruktion
// --------------------------------------------------------------------

Map::Map()
{
	tiletab = Tab;

	// Setze Tile Dimensionen
	iTilesWidth = RASTER;
	iTilesHeight = RASTER;

	// Speicher für Level und Koll allozieren
	pMemory = malloc(size_t(64000));
	pKMem = malloc(size_t(64000));
	pKoll = (KollMapCell*) pKMem;

	char LevelName[255];
	char *helper;
	FILE *hFile;

	for (int i = 1; i < 100; i++)
	{
		sprintf(LevelName, level_sets[LEVELS00], i);

		if ((hFile = fopen(LevelName, "rb")) != NULL)
		{
			fread(pMemory, sizeof(MapHeader), 1, hFile);
			fclose(hFile);
			helper = (char *)(pMemory);
			pMLevel = (MapHeader *)helper;
			LevelCodes[i] = pMLevel->code;
		}
	}
}

Map::~Map()
{
	free(pMemory);
	free(pKMem);
	delete pTiles;
	delete pShades;
}

void Map::Init(Game *parent)
{
	game = parent;

	// Tiles einlesen
	pTiles =  new Surface(tile_sets[TILES00]);
	pTiles->setscreen(game->pwindow.m_screen);

	// Setze Tile Dimensionen
	iTilesWidth = RASTER;
	iTilesHeight = RASTER;
	iTilesPerRow = pTiles->get_width() / iTilesWidth;

	pShades =  new Surface(tile_sets[SHADES]);
	pShades->setscreen(game->pwindow.m_screen);
}

// --------------------------------------------------------------------
//	Leveldatei einlesen
// --------------------------------------------------------------------

bool Map::Load(int Level)
{
	char LevelName[255];
	char   *helper;
	FILE *hFile;

	if (Level)
	{
		sprintf(LevelName, level_sets[LEVELS00], Level);

		if ((hFile = fopen(LevelName, "rb")) != NULL)
		{
			fread(pMemory, 64000, 1, hFile);
			fclose(hFile);
		}

		helper = (char *)(pMemory);
		pMLevel = (MapHeader *)helper;
		helper += sizeof(MapHeader);
		pMScreen = (MapCell *)helper;

		wKollLine = pMLevel->xscreens;
		Shuffle();
		FigurStartPos[0] = pMLevel->figx;
		FigurStartPos[1] = pMLevel->figy;

		list<Object*>::iterator it;

		for (int i = 0; i < 5; i++)
			for (it = game->objects.begin(); it != game->objects.end(); it++)
			{
				if (((*it)->get_type() > OBJ_MOVING))
				{
					delete (*it);
					it = game->objects.erase(it);
				}
			}

		CreateKollField();
		CreateEnemies();
	}
	return true;
}

// --------------------------------------------------------------------
//	Level zeichnen
// --------------------------------------------------------------------

void Map::Draw()
{
	int lx = 0, ly = 0, tilex = 0, tiley = 0, tilenum = 0;
	int xDist = 0;
	KollMapCell *pLScreen = pKoll;

	pLScreen += wKollPageDistance;

	for (ly = 0; ly < (YSCROFF + RASTER); ly += RASTER, pLScreen += wKollLine - xDist)
		for (lx = 0, xDist = 0; lx < SCR_W; lx += RASTER, pLScreen++, xDist++)
		{
			if (pLScreen->ebeneU > 0) // Untergründe zeichnen
				DrawTile(lx, ly, pLScreen->ebeneU);
			if ((pLScreen->ebeneG) > 0)
			{
				//DShade(pLScreen, lx, ly);
				DrawTile(lx, ly, pLScreen->ebeneG);
				//continue;
			}
			if ((pLScreen->ebeneM) > 0  && (pLScreen->ebeneM) < 240)
			{
				DrawTile(lx, ly, pLScreen->ebeneM);
			}
			else
				DShade(pLScreen, lx, ly);
		}

	Animate(); // evtl. Items-Animationen zeichnen
}

// --------------------------------------------------------------------
//	Schattierungsroutine
// --------------------------------------------------------------------

#define DHORIZ      244 // Schatten Definitionen
#define A_HOR       246
#define HORIZ       240
#define DVERT       245
#define A_VERT      247
#define VERT        241
#define ECKE_INNEN  242
#define ECKE_AUSSEN 243

void Map::DShade(KollMapCell *Koll, int x, int y)
{
	Koll -= wKollLine;

	// Mauer links und darüber
	if ((Koll)->ebeneM &&  (Koll - 1 + wKollLine)->ebeneM)
	{
		DrawShadow(x, y, ECKE_INNEN);
	}
	else
	{
		if (Koll->ebeneM && y) // Mauer drüber
		{
			if (!(Koll - 1)->ebeneM && x) // keine Mauer links drüber
			{
				if (Koll->flag == F_OPEN) // F_OPEN
					DrawShadow(x, y, DHORIZ);
				else
					DrawShadow(x, y, A_HOR);
			}
			else
				DrawShadow(x, y, HORIZ);
		}

		if ((Koll - 1 + wKollLine)->ebeneM && x) // Mauer links daneben
		{
			if (!(Koll - 1)->ebeneM) // keine Mauer links
			{
				if ((Koll + wKollLine - 1)->flag == F_OPEN) // F_OPEN
					DrawShadow(x, y, DVERT);
				else
					DrawShadow(x, y, A_VERT);
			}
			else
			{
				DrawShadow(x, y, VERT);
			}
		}
		else
		{
			if ((Koll - 1)->ebeneM && !(Koll)->ebeneM && x && y)
				DrawShadow(x, y, ECKE_AUSSEN);
		}
	}
}

//-----------------------------------------------------------------------------
// Name:	DrawShadow()
//
// Desc:	berechnet, mit der Nummer (num), die Position im Surface
//				und gibt das Tile auf dem Hintergrundpuffer aus
//
// M.Lagarden (c) 18.09.2001
//-----------------------------------------------------------------------------

void Map::DrawShadow(int x, int y, int num)
{
	SDL_Rect s, t;
	int sx, sy;

	s.w = t.w = iTilesWidth;
	s.h = t.h = iTilesHeight;

	sx = (num - 240) % iTilesPerRow;
	sy = (num - 240) / iTilesPerRow;
	s.x = sx * iTilesWidth;
	s.y = sy * iTilesHeight;
	t.x = x;
	t.y = y;

	pShades->draw(s, t );
};

//-----------------------------------------------------------------------------
// Name:	DrawTile()
//
// Desc:	berechnet, mit der Nummer (num), die Position im Surface
//				und gibt das Tile auf dem Hintergrundpuffer aus
//
// M.Lagarden (c) 18.09.2001
//-----------------------------------------------------------------------------

void Map::DrawTile(int x, int y, int num)
{
	SDL_Rect s, t;
	int sx, sy;

	s.w = t.w = iTilesWidth;
	s.h = t.h = iTilesHeight;

	sx = num % iTilesPerRow;
	sy = num / iTilesPerRow;
	s.x = sx * iTilesWidth;
	s.y = sy * iTilesHeight;
	t.x = x;
	t.y = y;

	pTiles->draw(s, t);
};

//-----------------------------------------------------------------------------
// Name:	DrawSprite()
//
// Desc:	berechnet, mit der Nummer (frame), die Position im Surface
//				und gibt das Tile auf dem Hintergrundpuffer mit Schattierung aus
//
// M.Lagarden (c) 18.09.2001
//-----------------------------------------------------------------------------

void Map::DrawSprite(int x, int y, int num)
{
	SDL_Rect s, t;
	int sx, sy;

	s.w = t.w = iTilesWidth;
	s.h = t.h = iTilesHeight;

	sx = (num - 240) % iTilesPerRow;
	sy = (num - 240) / iTilesPerRow;
	s.x = sx * iTilesWidth;
	s.y = sy * iTilesHeight;
	t.x = x + 5;
	t.y = y + 5;

	pShades->draw(s, t);

	sx = num % iTilesPerRow;
	sy = num / iTilesPerRow;
	s.x = sx * iTilesWidth;
	s.y = sy * iTilesHeight;
	t.x = x;
	t.y = y;

	pTiles->draw(s, t );
}

//-----------------------------------------------------------------------------
// Name:	DrawFigur()
//
// Desc:	zeichnet die Spielfigur, eventuelle Durchgänge werden
//				anschließend auch neu gezeichnet
//
// M.Lagarden (c) 30.03.2007
//-----------------------------------------------------------------------------

void Map::DrawFigur(int x, int y, int frame)
{
	KollMapCell *Koll;
	int Rx, Ry;

	DrawSprite(x, y, frame);

	Ry = (y >> 5); Rx = (x >> 5);
	Koll = pKoll + Rx + Ry * wKollLine + wKollPageDistance;

	if (Koll->flag == F_OPEN) // Durchgang ?
		DrawTile((Rx << 5), (Ry << 5), Koll->ebeneM);

	if ((Koll + 1)->flag == F_OPEN) // Durchgang rechts daneben ?
		if (Rx < 19)
			DrawTile(RASTER + (Rx << 5), (Ry << 5), (Koll + 1)->ebeneM);

	if (Ry < 10)
	{
		Koll += wKollLine;
		Ry += 1;
		if (Koll->flag == F_OPEN) // Durchgang darunter ?
			DrawTile((Rx << 5), (Ry << 5), Koll->ebeneM);

		if ((Koll + 1)->flag == F_OPEN) // Durchgang rechts daneben ?
			if (Rx < 19)
				DrawTile(RASTER + (Rx << 5), (Ry << 5), (Koll + 1)->ebeneM);
	}
}

// --------------------------------------------------------------------
//	Balloons im Spielfeld vermischen
// --------------------------------------------------------------------

void Map::Shuffle(void)
{
	unsigned char FirstBall;
	int LastBall, Max;
	int i, loops;

	Max = (pMLevel->xscreens * pMLevel->yscreens);
	for (loops = rand() & 7; loops; loops--)
	{
		LastBall = 0;
		FirstBall = 0;
		for (i = 0; i < Max; i++)
			if ((pMScreen + i)->flag == FLAG_BALLOON)
			{
				FirstBall = (pMScreen + i)->ebeneG; // sichere ersten Ballon
				LastBall = i;
				break;
			}
		i++;
		for (; i < Max; i++) //  Balloons vertauschen
			if ((pMScreen + i)->flag == FLAG_BALLOON) // Balloon ? dann merken
			{
				// Ballon an Vorgaengerposition
				(pMScreen + LastBall)->ebeneG = (pMScreen + i)->ebeneG;
				(pMScreen + i)->ebeneG = 0;
				LastBall = i;
			}

		if (FirstBall)
			(pMScreen + LastBall)->ebeneG = FirstBall; // letzter Ballon
	}

}

// --------------------------------------------------------------------
//	Kollissionsfeld  erstellen
// --------------------------------------------------------------------

void Map::CreateKollField()
{
	KollMapCell *pLKl = (KollMapCell *)pKoll;
	MapCell *pLSr = (MapCell *)pMScreen;
	int i, Kx, Ky;
	unsigned int wYMax, wXMax;
	unsigned int time = 0, AnNum = 0, count = 0;
	bool bwerfer = false;

	memset(AnimListe, 0, MAXANIM * sizeof(Animation));

	for (Ky = 0, wYMax = pMLevel->yscreens + 1; wYMax; wYMax--, Ky += RASTER)
	{
		for (Kx = 0, wXMax = pMLevel->xscreens; wXMax; wXMax--, Kx += RASTER, pLSr++, pLKl++)
		{
			pLKl->mask = 0;
			pLKl->ebeneU = pLSr->ebeneU;
			pLKl->ebeneG = pLSr->ebeneG;
			pLKl->ebeneM = pLSr->ebeneM;
			pLKl->flag   = pLSr->flag;
			pLKl->vbindX = pLSr->vbindX;
			pLKl->vbindY = pLSr->vbindY;
			pLKl->flag5 = 0;

			// Gegenstände
			if (pLKl->ebeneG < 240)
			{
				i = pLKl->ebeneG;
				if (i == I_EXIT)
				{
					ExitPosition[0] = Kx;
					ExitPosition[1] = Ky;
					if (pMLevel->counter)
						pLKl->ebeneG = 0;
					else
						SetExitAnimation();
				}
				else
				if (tiletab[i].ext2 > 8 && AnNum < MAXANIM)
				{
					AnimListe[AnNum].CycleCounter = (rand() & MAXANIM) + 1;
					AnimListe[AnNum].CycleDistance = tiletab[i].ext1;
					AnimListe[AnNum].X = Kx;
					AnimListe[AnNum].Y = Ky;
					AnimListe[AnNum].AnimNum = i;
					AnimListe[AnNum].Tile = i;
					AnimListe[AnNum].Counter = tiletab[i].ext2 / 16; // Animationen
					AnimListe[AnNum].Max = AnimListe[AnNum].Counter;
					AnNum++;
				}

				if (pLKl->ebeneG >= I_LOCH - 5 && pLKl->ebeneG < I_LOCH)
				{
					pLKl->timer = 0.0;
				}

				pLKl->mask = tiletab[pLKl->ebeneG].kollmask;
			}
			else
				pLKl->ebeneG = 0;

			// spezielle Untergründe
			if (pLKl->ebeneU >= UG_ICE && pLKl->ebeneU < UG_ICE + 4)
			{
				if (!pLKl->ebeneG)
					pLKl->ebeneG = I_ICE;
				pLKl->mask = 0xFFFF;
			}

			if (pLKl->flag == FLAG_BALLOON)
			{
				i = rand() & 7;
				pLKl->flag5 = pLKl->ebeneG;
				if (i > 4) i = 4;
				pLKl->ebeneG = (I_BALLOON + i);
				pLKl->mask = tiletab[pLKl->ebeneG].kollmask;
			}

			// Hindernisse
			if (pLKl->ebeneM > 0 && pLKl->ebeneM < 240)
				pLKl->mask = tiletab[pLKl->ebeneM].kollmask;
			else
				pLKl->ebeneM = 0;

			switch (pLKl->ebeneM)
			{
			case W_WERFER:
			case W_WERFER + 1:
			case W_WERFER + 2:
			case W_WERFER + 3:
				// Bälle vordefinieren
				// zu objects hinzufügen
				if (!bwerfer)
				{
					for (int i = 0; i < 4; i++)
						game->objects.push_back(new Object_Ball(game));
					bwerfer = true;
				}
				break;
			}
		}
	}
}

//	---------------------------------------------------------
//	Feindsprites erstellen
//	---------------------------------------------------------

void Map::CreateEnemies(void)
{
	KollMapCell *pLKl = (KollMapCell *)pKoll;
	MapCell *pLSr = (MapCell *)pMScreen;
	int Kx, Ky;
	unsigned int wYMax, wXMax;

	Object_Door *door;
	Object_UpDown *updw;
	Object_Circle *circ;

	for (Ky = 0, wYMax = pMLevel->yscreens; wYMax; wYMax--, Ky += RASTER)
		for (Kx = 0, wXMax = pMLevel->xscreens; wXMax; wXMax--, Kx += RASTER, pLSr++, pLKl++)
		{
			if (pLSr->ebeneM > 240) // Feindsprites
			{
				switch (pLSr->ebeneM)
				{
					case SPR_UPDOWN:
						updw = new Object_UpDown(game);
						updw->set_x(Kx);
						updw->set_y(Ky);
						game->objects.push_back(updw);
						break;

					case SPR_CIRCLE:
						circ = new Object_Circle(game);
						circ->set_x(Kx);
						circ->set_y(Ky);
						game->objects.push_back(circ);
						break;

					case SPR_H_DOOR:
						door = new Object_Door(game);
						door->set_x(Kx);
						door->set_y(Ky);
						door->typ = SPRITE_H_DOOR;
						game->objects.push_back(door);
						pLKl->flag = F_MOVEDOOR;
						break;

					case SPR_V_DOOR:
						door = new Object_Door(game);
						door->set_x(Kx);
						door->set_y(Ky);
						door->typ = SPRITE_V_DOOR;
						game->objects.push_back(door);
						pLKl->flag = F_MOVEDOOR;
						break;
				}
			}
			else
				pLSr->ebeneM = 0;
		}
}

// --------------------------------------------------------------------
//	Position der Kollision ermitteln
// --------------------------------------------------------------------

KollMapCell* Map::GetKollPosition(int x, int y)
{
	wKollX = (x >> 5);
	wKollY = (y >> 5);
	return(pKoll + wKollX + wKollY * wKollLine + wKollPageDistance);
}

// --------------------------------------------------------------------
//	Position der Kollision ermitteln
// --------------------------------------------------------------------

KollMapCell* Map::GetLocalKollPosition(int x, int y, bool shift)
{
	if (shift)
	{
		x = x >> 5;
		y = y >> 5;
	}
	return(pKoll + x + y * wKollLine);
}

// --------------------------------------------------------------------
//	Hole Tiletyp
// --------------------------------------------------------------------

int Map::GetTileType(int TileNum)
{
	return(tiletab[TileNum].type);
}

// --------------------------------------------------------------------
//	Hole Tile-Punkte
// --------------------------------------------------------------------

int Map::GetTilePoints(int TileNum)
{
	return(tiletab[TileNum].ext1);
}

// --------------------------------------------------------------------
//	Setze Figurposition
// --------------------------------------------------------------------

bool Map::SetFigurPosition()
{
	int Sx, Sy;

	Sx = ((wKollX << 5) + wScrXoff) & 0xffe0;
	Sy = ((wKollY << 5) + wScrYoff) & 0xffe0;

	if (Sx != FigurStartPos[0])
		if (Sy != FigurStartPos[1])
		{
			FigurStartPos[0] = ((wKollX << 5) + wScrXoff) & 0xffe0;
			FigurStartPos[1] = ((wKollY << 5) + wScrYoff) & 0xffe0;
			return(true);
		}
	return(false);
}

// --------------------------------------------------------------------
//	Hole Figurposition
// --------------------------------------------------------------------

void Map::GetFigurPosition(float *x, float *y)
{
	int dx, dy;

	*x = FigurStartPos[0] % XSCROFF; // Figur-Position in Pixeln relativ
	*y = FigurStartPos[1] % YSCROFF;

	wScrXoff = FigurStartPos[0] - (int)*x;
	wScrYoff = FigurStartPos[1] - (int)*y;

	dx = FigurStartPos[0] / XSCROFF;
	dy = FigurStartPos[1] / YSCROFF;

	wKollPageDistance = dx * 19 + dy * wKollLine * 10;
}

// --------------------------------------------------------------------
//	Hole Levelzeit
// --------------------------------------------------------------------

int Map::GetTime()
{
	return (pMLevel->time);
}

// --------------------------------------------------------------------
//	Hole Anzahl einzusammelnder Items
// --------------------------------------------------------------------

int Map::GetMaxItems()
{
	return (pMLevel->counter);
}

// --------------------------------------------------------------------
//	Hole Code
// --------------------------------------------------------------------

int Map::GetCode()
{
	return (pMLevel->code);
}

// --------------------------------------------------------------------
//	Animiere Tiles
// --------------------------------------------------------------------

void Map::Animate(void)
{
	int AnNum;
	KollMapCell *cell;

	static Timer sec(10);

	if (sec.elapsed())
	{
		sec.reset();

		for (AnNum = 0; AnNum < MAXANIM; AnNum++)
		{
			if (!AnimListe[AnNum].Tile) // kein Tile, keine Animation
				return;

			if (!--AnimListe[AnNum].CycleCounter) // Zähler schon Null?
			{
				// Zyklendistanz
				AnimListe[AnNum].CycleCounter = AnimListe[AnNum].CycleDistance;

				if (AnimListe[AnNum].Counter) // nächster Animationsschritt
					AnimListe[AnNum].Counter--;
				else
					AnimListe[AnNum].Counter = AnimListe[AnNum].Max; // Sequenzoffset
			}
			cell = pKoll + (AnimListe[AnNum].X >> 5) + (AnimListe[AnNum].Y >> 5) * wKollLine;
			cell->ebeneG = AnimListe[AnNum].Tile + AnimListe[AnNum].Counter;
		}
	}
}

// --------------------------------------------------------------------
//	Setze Exitposition
// --------------------------------------------------------------------

void Map::SetExitAnimation(void)
{
	int i;

	for (i = 0; i < MAXANIM; i++)
		if (AnimListe[i].Tile == 0)
			break; // kein Tile dann leer/

	if (i)
		AnimListe[i].CycleCounter = AnimListe[i - 1].CycleCounter + 2;
	else
		AnimListe[i].CycleCounter = 2; // einzige Animation ?


	AnimListe[i].CycleDistance = tiletab[I_EXIT].ext1;
	AnimListe[i].X = ExitPosition[0];
	AnimListe[i].Y = ExitPosition[1];
	AnimListe[i].AnimNum = I_EXIT;

	AnimListe[i].Tile = I_EXIT;
	AnimListe[i].Counter = tiletab[I_EXIT].ext2 / 16; // Animationen
	AnimListe[i].Max = AnimListe[i].Counter;

	KollMapCell *Koll = GetLocalKollPosition(ExitPosition[0], ExitPosition[1], true);

	Koll->mask = 0x0180;
	Koll->ebeneG = I_EXIT;
	Koll->ebeneM = 0;
}
