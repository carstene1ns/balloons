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

#ifndef WORLD_H
#define WORLD_H

#include "util.h"

#define PRESENT  0
#define MENUTIME 6
#define HIGHTIME 7
#define DEMOTIME 8

#define MEN_SHOW  100
#define MEN_GUIDE 107
#define MEN_DEMO  109

#define LBL_START_GAME 0
#define LBL_ONE_PLAYER 1
#define LBL_TWO_PLAYER 2
#define LBL_ENTER_CODE 3
#define LBL_VIEW_GUIDE 4
#define LBL_VIEW_HIGH  5
#define LBL_QUIT       6
#define LBL_NEW_HIGH   7
#define LBL_PRESENT    8
#define LBL_CREATED    9
#define LBL_LANDSCAPES 10
#define LBL_ANY_KEY    11
#define LBL_ENTER_NAME 12
#define LBL_LANGUAGE   13
#define MENU_COUNT     14

#include "game.h"
#include "scoreList.h"

class World
{
public:
	World(DisplayWindow &window);
	~World();

	Font *font32;
	Font *font20;

	Game *game;
	bool m_quit;

	//: Slot for shutting down
	void on_quit() {
		m_quit = true;
	}

	void run(DisplayWindow &window);

private:
	void on_key_down(SDLKey &key);

	int Lang;
	int MaxLang;
	std::string Language;
	std::string LangNames[20];

	Surface *m_title;
	DisplayWindow pwindow;

	std::string menu_text[20];
	std::string guide_text[6];

	ScoreList HighScore;
	int State;
	int diff;
	bool NewEntry; // Neuer Score - Flag
	bool CodeInput; // Codeeingabe
	bool bFaded;

	Uint16 m_key; // gedrückte Taste
	SDL_Event event;

	int CursorPos;
	char InputText[31]; // Textfeld für Tastatureingaben

	int menutimes[8]; // Standzeiten in Sekunden
	Timer* menu_timer;

	bool m_done;
	bool m_game;

	void Show();
	void ShowMainMenu (int type);
	void ShowProlog(int type);
	void ShowGuide(void);

	void EnterCode();
	void EnterHigh();
	void DrawText(std::string InputText, int x, int y, int mode);

	void LoadLanguage(int lang);
};

#endif
