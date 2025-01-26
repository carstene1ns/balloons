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

#ifndef B_GAME_WORLD_H
#define B_GAME_WORLD_H

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
	void on_key_down(const SDL_Keycode key);

	int Lang;
	int MaxLang;
	string Language;
	string LangNames[20];

	Surface *m_title;
	DisplayWindow pwindow;

	string menu_text[20];
	string guide_text[6];

	ScoreList HighScore;
	int State;
	int diff;
	bool NewEntry; // Neuer Score - Flag
	bool CodeInput; // Codeeingabe
	bool bFaded;

	SDL_Keycode m_key; // gedrückte Taste
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
	void DrawText(string InputText, int x, int y, int mode);

	void LoadLanguage(int lang);
};

#endif // B_GAME_WORLD_H
