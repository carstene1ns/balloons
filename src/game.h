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

#ifndef header_game
#define header_game

#include <cstdlib>
#include <cstring>
#include <list>

#include "util.h"
#include "map.h"
#include "player.h"
#include "tiledef.h"

#include "obj_animate.h"
#include "obj_mov.h"

#define POCKET_SIZE 21 // maximale Pocketgrösse

#define MAX_OBJECT_ANIM 10

#define SPEED_MULTI 20.0 // Geschwindigkeits-Multiplikator
#define MAXSPEED 12.0 * SPEED_MULTI // Maximale Figurgeschwindigkeit
#define MBFIGDIST MAXSPEED - 6.0 * SPEED_MULTI // Abstand der Figur zum Moveblock
#define ACELLTIME 0.24 // Beschleunigungszeitraum in Sekunden
#define ACELFACT MAXSPEED / ACELLTIME

#define LEFT  4 // Richtungen
#define RIGHT 8
#define UP    1
#define DOWN  2
#define FIRE  128

#define GAME_OVER   999 // Spiel beendet
#define GAME_PAUSED 998 // Spiel in Pause
#define GAME_QUIT   997 // Spiel abbrechen
#define GAME_ACTIVE 995 // Spiel laeuft
#define GAME_DEMO   990 // Demo laeuft

#define GAME_NEXTLEVEL 1 // naechsten Level aufrufen
#define GAME_NOLIVES   2 // keine Leben mehr
#define GAME_TIMEOUT   4 // Zeit abgelaufen

#define FIGUR_KOLLISION 8
#define FIGUR_FALLING   32

#define SET_CENTER   1 // Modi Draw_Text
#define SET_SMALL    64
#define SET_SELECTED 128

#define F_IN    1 // Modi Fade
#define F_OUT   2
#define F_SMEAR 4
#define F_FULL  8

enum {
	S_BANG = 0, S_BEEP, S_BOMB,
	S_BUMP, S_CRASH, S_DOOR,
	S_DROP, S_EXIT, S_FADE,
	S_FALL, S_FURZ, S_ITEM,
	S_KEY, S_KLICK, S_MOVE,
	S_POP, S_TWI, S_TYPE,
	S_WALL, S_WARP, S_WBRK
};

class Game
{
public:
	Game(DisplayWindow &window);
	virtual ~Game();

	DisplayWindow pwindow;

	Map map;

	std::list<Object*> objects;

	int State; // Spielstatus
	string Language; // Sprache
	Player player;
	short int TwoPlayerMode;

	bool run(); // Spiel starten
	int GameLoop (void); // Spiel-Hauptschleife

	void PlayWave (int, float vol = 0.5f); // Sound-Ausgabe

	void FadeScr(int mode, int i);
	void ShowDemo(void);
	void DestW2(void);

	int get_x(){
		return (int)x;
	}
	int get_y(){
		return (int)y;
	}

private:

	Surface* pFrame; // Spiel-Menu-Rahmen
	Font* pFont32; // Zeichensatz 32
	Font* pFont16; // Zeichensatz 20

	SoundBuffer *m_lpWave[21]; // Sound-Zeiger...

	Timer *game_timer, *menu_timer;
	Timer *begin_time;
	int fps, fps_show;

	bool StartLevel(void);
	void DrawText(string InputText, int x, int y, int mode);

	string labels[20]; // Texte im Spiel

	float time_elapsed; // verstrichene Zeit zwischen zwei Frames

	// Attributes
	int GameState; // Spielstatus
	bool bShowCode;
	bool NewLevelStart;

	// Spielfigur
	float x, y; // Position
	int SprListOff; // Animationsoffset
	int SprAnimNum; // Animationsnummer
	float SprMoveTimer;

	short int Max_It; // zu suchende Items
	short int Levtime, Timeless; // verstrichene Spielzeit
	short int Pocket[21]; // Tasche
	short int PocketCnt; // Tascheninhalt-Zaehler

	// Bewegung
	short int Direction; // momentane Richtung
	int JoyDir; // angegebene Richtung
	float XSpeed, YSpeed; // Geschwindigkeit

	// Kollision
	short int KollRichtung;
	unsigned int HorKolFlg, VerKolFlg;

	void on_key_down(const SDL_Keycode key);
	void on_key_up(const SDL_Keycode key);

	void GameMenu(void); // Spielermenue (Score, Items, Inventar, etc.)

	//--------------------------
	// Game_Loop.cpp
	//--------------------------
	long swap(long x);
	void SetLoop(void);
	void SetLoopKollision(void);

	bool m_done;

	//--------------------------
	// Game_Kollision.cpp
	//--------------------------

	Object_Animate DrawPart[MAX_OBJECT_ANIM];
	Object_Animate *GetFreeBuffer(void);

	void FiScrSet(void);
	void StopFigur(void);
	void SetFigStart(void); // Startposition der Spielfigur aus Level laden
	void SetExit(void);

	short int KollissionDetect(KollMapCell *Koll);
	short int TakeItem(KollMapCell *Koll);

	short int ItBall(KollMapCell *Koll);
	short int ItTeleport(KollMapCell*);
	short int ItRiss(KollMapCell*);
	short int ItFall(KollMapCell*);
	short int ItIce(KollMapCell*);
	short int ItBomb(void);
	short int ItBumper(void);
	short int ItExit(KollMapCell*);

	short int Wall(void);
	short int NormWall(void);
	short int DestWall(KollMapCell *);
	short int MoveWall(KollMapCell*);
	void Checkbuffer(KollMapCell*);
	short int DoorWall(KollMapCell*);
	short int ClickWall(KollMapCell*);
	short int FlipFlop(KollMapCell*);
	short int KillWall(KollMapCell*);
	short int MirrorNorm(KollMapCell*);
	short int KillEnemy(void);
	short int FlipPlaceWall(KollMapCell*);
	bool CheckNewPos(KollMapCell *New, KollMapCell *Old);
	short int BumpWall(KollMapCell*);

	void ExeClick(KollMapCell*);

	short int InsertPocket(void);
	void ExtractFromPocket(short int tile);

};

#endif
