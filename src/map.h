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

// Screen resolutions
#define SCR_W   640
#define SCR_H   480
#define SCR_BPP 16

#define XSCROFF 608 // Screenbreite-RASTER
#define YSCROFF 320 // Screenhöhe-RASTER

#define RGB(r, g, b) r | g | b

typedef struct // Aufbau eines Kollisionsfeld-Eintrages
{
	unsigned short int mask; // Kollisionsmaske
	unsigned char ebeneU;    // Untergrund
	unsigned char ebeneG;    // Gegenstaende und Zustaende
	unsigned char ebeneM;    // Mauern
	unsigned char flag;      // diverse Flags
	unsigned char vbindX;    // Verbindung X
	unsigned char vbindY;    // Verbindung Y
	unsigned char Counter;   // Zaehlwerk
	unsigned char flag5;
	float timer;
} KollMapCell;

typedef struct
{
	unsigned char version;   // Header-Versionsnummer
	char id[5];              // Autor-initialien
	short int size;          // Levelgroesse in Bytes
	unsigned char xscreens;  // Breite in Bildschirmen
	unsigned char yscreens;  // Hoehe in Bildschirmen
	short int time;          // Spielzeit
	short int counter;
	unsigned short int code; // Level-Eintritts-Code
	short int figx;
	short int figy;
	short int TileSet;       // Nummer des Dekor-Satzes
	char name[16];           // Name des Levels
} MapHeader;

typedef struct // Aufbau eines Leveleintrages
{
	unsigned char ebeneU; // Untergrund
	unsigned char ebeneG; // Gegenstaende
	unsigned char ebeneM; // Mauern
	unsigned char flag;   // diverse Flags
	unsigned char vbindX; // Verbindung X
	unsigned char vbindY; // Verbindung Y
	unsigned char flag4;
	unsigned char flag5;
} MapCell;

typedef struct
{
	short int type;
	short int offset;        // not used
	short int points;        // not used
	unsigned short kollmask;
	short int ext1;          // CycleDistance, Points
	short int ext2;          // Anzahl Animationen
} TileTab;

#define MAXANIM 60 // Maximalanzahl Animationen

typedef struct // Animationstabelle
{
	int AnimNum;
	int X;             // x (abs)
	int Y;             // y (abs)
	int Tile;          // 1.Tile der Sequenz
	int Counter;       // (Animationen -1) * 4 Zaehler
	int Max;           // (Animationen -1) * 4 MaxAnzahl
	int CycleCounter;  // Zyklenzaehler
	int CycleDistance; // Zyklendistanz
} Animation;

#include "object_moving.h"

class Game;

class Map
{
public:
	Map();
	virtual ~Map();

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
