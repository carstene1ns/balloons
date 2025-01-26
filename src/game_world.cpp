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

#include "game_world.h"
#include "main.h"

// --------------------------------------------------------------------
//	Konstruktor / Destruktor
// --------------------------------------------------------------------

World::World(DisplayWindow &window)
{
	DEBUG("Setting up game-world...");

	pwindow = window;

	game = new Game(window);

	font32 = new Font(font_files[FONT32], 28);
	font32->setscreen(window.m_screen);
	font20 = new Font(font_files[FONT20], 20);
	font20->setscreen(window.m_screen);

	m_title = new Surface(image_files[BALL_PNG]);
	m_title->setscreen(window.m_screen);

	// wieviele Sprachen sind vorhanden
	Language = "en;de;fr;ne";
	int i = 0;
	for (int a = 0; a < Language.length(); i++, a += 3)
		LangNames[i] = Language.substr(a, 2);

	MaxLang = i;
	// Lade Texte
	LoadLanguage(0);

	CursorPos = 0;
	diff = PRESENT;

	bFaded = false;
	CodeInput = false;
	NewEntry = false;

	State = GAME_QUIT;

	// Anzeigedauer in Sekunden
	menutimes[PRESENT] = 3;
	menutimes[PRESENT + 1] = 1;
	menutimes[PRESENT + 2] = 4;
	menutimes[PRESENT + 3] = 5;
	menutimes[PRESENT + 4] = 3;
	menutimes[PRESENT + 5] = 4;
	menutimes[MENUTIME] = 16;
	menutimes[HIGHTIME] = 8;
	menutimes[DEMOTIME] = 1;

	menu_timer = new Timer(1000);

	DEBUG("game-world is working!");
}

World::~World()
{
	DEBUG("Shutting down the game-world...");

	delete menu_timer;
	delete m_title;
	delete font20;
	delete font32;
	delete game;

	DEBUG("game-world is shutdown!");
}

void World::LoadLanguage(int lang)
{
	Language = LangNames[lang];
	game->Language = Language;

	menu_text[LBL_START_GAME] = "F2 Start Game";
	menu_text[LBL_ONE_PLAYER] = "F3 One Player";
	menu_text[LBL_TWO_PLAYER] = "F3 Two Players";
	menu_text[LBL_ENTER_CODE] = "F4 Enter Levelcode";
	menu_text[LBL_VIEW_GUIDE] = "F7 Player's Guidelines";
	menu_text[LBL_VIEW_HIGH]  = "F8 View Highscores";
	menu_text[LBL_QUIT]       = "F10 Quit";
	menu_text[LBL_NEW_HIGH]   = "You're welcome ...";
	menu_text[LBL_PRESENT]    = "presents";
	menu_text[LBL_CREATED]    = "CREATED BY";
	menu_text[LBL_LANDSCAPES] = "LANDSCAPES";
	menu_text[LBL_ANY_KEY]    = "press any key to continue";
	menu_text[LBL_ENTER_NAME] = "enter your name";
	//menu_text[LBL_LANGUAGE]   = "F6 Choose Language";
}

/////////////////////////////////////////////////////////////////////////////
// World attributes:

/////////////////////////////////////////////////////////////////////////////
// World operations:

void World::run(DisplayWindow &window)
{
	m_quit = false;
	m_game = false;
	State = MEN_SHOW;

	menu_timer->reset();

	while (!m_quit)
	{
		if (!m_game)
		{
			if (game->State == GAME_OVER)
			{
				if (HighScore.Check(*game->player.Score) == true)
				{
					NewEntry = true;
					strcpy(InputText, " ");
					CursorPos = 0;
					diff = HIGHTIME;
					menu_timer->set(500);
					game->State = GAME_QUIT;
				}
				else if (game->player.Check() == true)
				{
					game->State = GAME_NEXTLEVEL;
					diff = -1;
				}
				else
				{
					diff = MENUTIME;
					menu_timer->set(1000);
					game->State = GAME_QUIT;
				}
			}

			if (!CodeInput && !NewEntry)
				Show();
			else
			{
				if (menu_timer->elapsed())
				{
					if (InputText[CursorPos] == ' ')
						InputText[CursorPos] = '-';
					else
						InputText[CursorPos] = ' ';
					InputText[CursorPos+1] = '\0';
					menu_timer->reset();
				}
				if (CodeInput)
					EnterCode();
				else if (NewEntry)
					EnterHigh();
			}

			if (menu_timer->time_passed() >= menutimes[diff] * 1000.0)
			{
				if (diff == DEMOTIME)
					diff = PRESENT;
				else
					diff++;
				menu_timer->reset();
				bFaded = false;
			}
		}
		else
		{
			m_game = game->run();
			menu_timer->reset();
		}
		while (SDL_PollEvent(&event))
			if (event.type == SDL_KEYDOWN)
			{
				on_key_down(event.key.keysym.sym);
			}
	}
}

void World::on_key_down(const SDL_Keycode key)
{
	menu_timer->reset();

	if (!m_game)
	{
		switch (key)
		{
		// Starte Spiel
		case SDLK_F2:
			bFaded = false;
			game->player.Init(game->TwoPlayerMode);
			game->State = GAME_NEXTLEVEL;
			diff = MENUTIME;
			m_game = true;
			break;

		// Ein/Zwei Spieler Modus
		case SDLK_F3:
			bFaded = true;
			game->TwoPlayerMode = (game->TwoPlayerMode) ? 0 : 1;
			game->player.ActualPlayer = -1;
			break;

		// Codeeingabe
		case SDLK_F4:
			CodeInput = true;
			menu_timer->set(500);
			memset(InputText, 0, 30);
			CursorPos = 0;
			strcpy(InputText, " ");
			diff = MENUTIME;
			break;

		case SDLK_F6:
			bFaded = true;
			if (Lang < MaxLang - 1) Lang++;
			else Lang = 0;
			LoadLanguage(Lang);
			break;

		case SDLK_F7:
			State = MEN_GUIDE;
			ShowGuide();
			break;

		case SDLK_F8:
			bFaded = false;
			diff = HIGHTIME;
			break;

		case SDLK_F9:
			bFaded = false;
			diff = DEMOTIME;
			break;

		case SDLK_F10:
			m_quit = true;
			break;

		// Texteingabe
		case SDLK_BACKSPACE:
			if (CursorPos)
			{
				InputText[CursorPos] = ' ';
				CursorPos--;
			}
			break;

		case SDLK_ESCAPE:
			if (CodeInput || NewEntry)
			{
				if (CursorPos)
				{
					memset(InputText, 0, 20);
					CursorPos = 0;
				}
				else
				{
					InputText[0] = '\0';
					NewEntry = false;
					CodeInput = false;
				}
				menu_timer->set(1000);
			}
			else
			{
				if (diff != MENUTIME) bFaded = false;
				diff = MENUTIME;
			}
			State = MEN_SHOW;
			break;

		case SDLK_RETURN:
			if (CodeInput || NewEntry)
			{
				InputText[CursorPos] = '\0';
				if (CodeInput)
					CodeInput = false;
				else
				{
					NewEntry = false;
					HighScore.Enter(InputText, *game->player.Score);
					if (game->player.Check() == true)
						game->State = GAME_NEXTLEVEL;
					else
						diff = HIGHTIME;
				}
				menu_timer->set(1000);
			}
			else
			{
				bFaded = false;
			}
			break;

		case SDLK_F11:
			if (pwindow.is_fullscreen())
				pwindow.set_windowed();
			else
				pwindow.set_fullscreen(SCR_W, SCR_H, SCR_BPP);
			break;

		default:
			m_key = event.key.keysym.sym;
			break;
		}

	}
	else
		switch (key)
		{
		case SDLK_F11:
			if (pwindow.is_fullscreen())
				pwindow.set_windowed();
			else
				pwindow.set_fullscreen(SCR_W, SCR_H, SCR_BPP);

			break;
		}
}

// --------------------------------------------------------------------
//	Menuablauf darstellen
// --------------------------------------------------------------------

void World::Show()
{
	Timer m_fade_second(50);

	if (bFaded == false)
	{
		game->FadeScr(F_OUT | F_FULL, 0);
		game->PlayWave(S_FADE);
	}

	for (int i = 0; i < 9; i++)
	{
		pwindow.clear();
		switch (diff)
		{
			case PRESENT:
			case PRESENT + 1:
			case PRESENT + 2:
			case PRESENT + 3:
			case PRESENT + 4:
			case PRESENT + 5:
				ShowProlog(diff);
				break;

			case MENUTIME:
			case HIGHTIME:
				ShowMainMenu (diff);
				break;

			case DEMOTIME:
				game->ShowDemo();
				menu_timer->reset();
				diff = PRESENT;
				break;
		}

		if (bFaded == false)
		{
			m_fade_second.reset();
			game->FadeScr(F_IN, i);
			while (!m_fade_second.elapsed()) ;
		}

		pwindow.flip();
	}
	bFaded = true;
}

// --------------------------------------------------------------------
//	Hauptmenue
// --------------------------------------------------------------------

void World::ShowMainMenu (int type)
{
	switch (type)
	{
		case MENUTIME:
			DrawText(menu_text[LBL_START_GAME], 0,  2 * 18, SET_CENTER);
			if (!game->TwoPlayerMode)
				DrawText(menu_text[LBL_ONE_PLAYER], 0,  2 * 34, SET_CENTER);
			else
				DrawText(menu_text[LBL_TWO_PLAYER], 0,  2 * 34, SET_CENTER);
			DrawText(menu_text[LBL_ENTER_CODE], 0,  2 * 92, SET_CENTER);
			m_title->draw((SCR_W - m_title->get_width()) / 2, 115);

//			DrawText(menu_text[LBL_LANGUAGE],0,  2*108,SET_CENTER);
			DrawText(menu_text[LBL_VIEW_GUIDE], 0,  2 * 124, SET_CENTER);
			DrawText(menu_text[LBL_VIEW_HIGH], 0,  2 * 140, SET_CENTER);
			DrawText(menu_text[LBL_QUIT], 0,  2 * 156, SET_CENTER);
			break;

		case HIGHTIME:
			int wCnt, y;
			char sText[80];
			char * txt;
//			DrawText(" ",0, 2*24,SET_CENTER);
			if (NewEntry)
			{
				DrawText(menu_text[LBL_NEW_HIGH], 0, 18, SET_CENTER);
				game->PlayWave(S_EXIT, 0.3f);
			}
			else
			{
				txt = (char*)menu_text[LBL_VIEW_HIGH].c_str();
				DrawText(txt + 3, 0, 18, SET_CENTER);
			}

			for (wCnt = 0, y = 80; wCnt < 10; wCnt++, y += 32)
			{
				sprintf(sText, "%2d.", wCnt + 1);
				DrawText(sText, 100, y, 0);
				sprintf(sText, "%s", HighScore.GetName(wCnt));
				DrawText(sText, 150, y, 0);
				sprintf(sText, "%3u,%02u €", HighScore.GetScore(wCnt) / 100, HighScore.GetScore(wCnt) % 100);
				DrawText(sText, 400, y, 0);
			}
			break;
	}
}

// --------------------------------------------------------------------
//	Prolog
// --------------------------------------------------------------------

void World::ShowProlog(int type)
{
	switch (type)
	{
		case PRESENT:
			pwindow.clear();
			DrawText("-S-v-N-", 0, 168, SET_CENTER);
			break;
		case PRESENT + 1:
			DrawText("Software vom Niederrhein", 0, 168, SET_CENTER);
			break;
		case PRESENT + 2:
			DrawText(menu_text[LBL_PRESENT], 0, 168, SET_CENTER);
			break;
		case PRESENT + 3:
			m_title->draw((SCR_W - m_title->get_width()) / 2, 160);
			break;
		case PRESENT + 4:
			DrawText("(c) 1993-2007", 0, 168, SET_CENTER);
			break;
		case PRESENT + 5:
			// Spielidee
			DrawText(menu_text[LBL_CREATED], 0, 152, SET_CENTER | SET_SMALL);
			DrawText("Roland & Michael Lagarden", 0, 184, SET_CENTER);
			break;
		case PRESENT + 6:
			// Landschaften
			DrawText(menu_text[LBL_LANDSCAPES], 0, 120, SET_CENTER | SET_SMALL);
			DrawText("Dennis Becker", 0, 160, SET_CENTER);
			DrawText("Roland Lagarden", 0, 192, SET_CENTER);
			DrawText("Michael Lagarden", 0, 224, SET_CENTER);
			break;
	}
}

// --------------------------------------------------------------------
//	Hilfe
// --------------------------------------------------------------------

void World::ShowGuide(void)
{
	string line, prefix;
	int y, len;

	game->FadeScr(F_OUT | F_FULL, 0);
	game->PlayWave(S_FADE);

	std::ifstream guide(lang_files[Lang]);

	do
	{
		m_key = 0;
		pwindow.clear();

		for (y = 16; y < 416; y += 32)
		{
			getline(guide, line);
			len = line.length() - 1;
			if (len <= 0)
				continue;

			prefix = line.substr(0, 1);
			if (prefix == "#")
				break;
			else if (prefix == "&")
				DrawText(line.substr(1, len), 0, y, 0);
			else if (prefix == "$")
			{
				y -= 16;
				DrawText(line.substr(1, len), 60, y, SET_SMALL | SET_CENTER);
			}
			else
				DrawText(line, 0,  y, SET_CENTER);
		}

		do
		{
			DrawText(menu_text[LBL_ANY_KEY], 0, 388, SET_CENTER | SET_SMALL);
			pwindow.flip();

			while (SDL_PollEvent(&event))
				if (event.type == SDL_KEYDOWN)
				{
					on_key_down(event.key.keysym.sym);
//					m_key = 1;
					if (guide.eof())
						State = MEN_SHOW;
				}
			if (m_key)
				break;
		}
		while (State != MEN_SHOW);
	}
	while (!guide.eof() && State != MEN_SHOW);

	guide.close();
}

// --------------------------------------------------------------------
//	Eingabe des Level-Codes
// --------------------------------------------------------------------

void World::EnterCode()
{
	int CodeLevel = 0;

	if (m_key <= '9' && m_key >= '0') // printable char
	{
		game->PlayWave(S_TYPE);
		InputText[CursorPos] = (char)m_key;
		CursorPos++;
		if (CursorPos == 5)
			CodeInput = false;
		InputText[CursorPos] = '\0';
	}
	m_key = 0;

	pwindow.clear();

//	DrawFrame(-1, 160, 6*RASTER);
	DrawText(InputText, 0, 174, SET_CENTER);

	pwindow.flip();

	// Eingabe des Codes
	if (!CodeInput)
	{
		unsigned short int wCode;
		CursorPos = 0;
		game->player.Init( game->TwoPlayerMode);
		diff = MENUTIME;
		m_game = true;

		// Code in Tabelle suchen
		if ((wCode = (unsigned int)(atol(InputText))) != 0)
			for (CodeLevel = 99; CodeLevel > 1; CodeLevel--)
				if (game->map.LevelCodes[CodeLevel] == wCode) break;
// #ifdef SHOW_DEBUG	// Nur für Testzwecke
//		*game->player.Level = CodeLevel = wCode;
// #endif

		if ((*game->player.Level = CodeLevel) == 1) // neuen Startlevel festlegen
		{
			if (*game->player.Level > 0 && *game->player.Level < 100)
			{
				game->State = GAME_NEXTLEVEL;
			} else // Test Ende
				game->PlayWave(S_BEEP); // Warnton für falschen Code
		} else
			game->State = GAME_NEXTLEVEL;
	}
}

// --------------------------------------------------------------------
//	Highscore eintragen
// --------------------------------------------------------------------

void World::EnterHigh()
{
	if (m_key <= SDLK_z && m_key >= SDLK_SPACE) // printable char
	{
		game->PlayWave(S_TYPE);
		InputText[CursorPos] = (char)m_key;
		CursorPos++;
		InputText[CursorPos] = '\0';
	}
	m_key = 0;
	pwindow.clear();

	DrawText(menu_text[LBL_NEW_HIGH], 0, 80, SET_CENTER);
	DrawText(menu_text[LBL_ENTER_NAME], 0, 112, SET_CENTER);
	DrawText(InputText, 0, 200, SET_CENTER);

	pwindow.flip();
}

// --------------------------------------------------------------------
//	Textausgaben
// --------------------------------------------------------------------

void World::DrawText(string InputText, int x, int y, int mode)
{
	if (mode & SET_SMALL)
	{
		if (mode & SET_CENTER)
			x = (SCR_W - font20->get_width(InputText.c_str())) / 2;
		font20->draw(x, y, InputText.c_str());
	}
	else
	{
		if (mode & SET_CENTER)
			x = (SCR_W - font32->get_width(InputText.c_str())) / 2;
		font32->draw(x, y, InputText.c_str());
	}
}
