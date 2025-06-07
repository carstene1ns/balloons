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

#ifndef B_MAP_H
#define B_MAP_H

#include "mapdef.h"
#include "object_moving.h"

class Game;

class Map
{
public:
	Map();
	~Map();

	KollMapCell *pKoll;
	MapHeader *pMLevel; // Level-Header
	MapCell *pMScreen; // Level-Aufbau
	int wKollX, wKollY, wKollLine;
	int wKollPageDistance;

	int wScrXoff, wScrYoff; // Bildschirmraster

	unsigned short int LevelCodes[101];

	// Resourcen und Game an Map übergeben
	void Init(Game * parent);

	bool Load(int Level);
	void Draw();

	void DrawTile(int x, int y, int num);
	void DrawSprite(int x, int y, int offset);
	void DrawFigur(int x, int y, int offset);

	KollMapCell* GetKollPosition(int x, int y);
	KollMapCell* GetLocalKollPosition(int x, int y, bool shift);

	int GetTileType(int TileNum);
	int GetTilePoints(int TileNum);

	bool SetFigurPosition();
	void GetFigurPosition(float *x, float *y);

	int GetTime();
	int GetMaxItems();
	int GetCode();

	void SetExitAnimation();

private:
	Game * game;

	Surface* pTiles;
	Surface* pShades;

	void *pMemory, *pKMem; // Hilfszeiger

	TileTab *tiletab; // Tiletabelle
	Animation AnimListe[MAXANIM];

	int iTilesPerRow;
	int iTilesWidth;
	int iTilesHeight;

	int FigurStartPos[2]; // Startposition der Figur
	int ExitPosition[2]; // Position des Exit

	void DShade(KollMapCell *Koll, int x, int y);
	void DrawShadow(int x, int y, int num);

	void Shuffle(void);
	void CreateKollField();
	void CreateEnemies();

	void Animate();
};

#endif // B_MAP_H
