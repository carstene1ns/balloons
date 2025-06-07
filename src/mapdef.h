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

#ifndef B_MAPDEF_H
#define B_MAPDEF_H

// Screen resolutions
#define SCR_W   640
#define SCR_H   480

#define XSCROFF 608 // Screenbreite-RASTER
#define YSCROFF 320 // Screenhöhe-RASTER

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

#endif // B_MAPDEF_H
