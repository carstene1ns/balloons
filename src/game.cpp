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

#include "game.h"
#include "main.h"

using std::list;

#define MAKE_LEFT       0x04
#define MAKE_RIGHT      0x08
#define MAKE_UP         0x01
#define MAKE_DOWN       0x02
#define MAKE_FIRE       0x80

#define BREAK_HORIZ     0x83
#define BREAK_VERT      0x8C
#define BREAK_FIRE      0x0F

#define LBL_PAUSE       11
#define LBL_PLAYER_ONE  12
#define LBL_PLAYER_TWO  13

// --------------------------------------------------------------------
//	Create Game
// --------------------------------------------------------------------

Game::Game(DisplayWindow &window)
{
	DEBUG( "Game::Game()" );

	pwindow = window;
	map.Init(this);

	pFrame = new Surface(image_files[FRAME]);
	pFrame->setscreen(window.m_screen);
	pFont32 = new Font(font_files[FONT32], 32);
	pFont32->setscreen(window.m_screen);
	pFont16 = new Font(font_files[FONT20], 16);
	pFont16->setscreen(window.m_screen);

	// Wavefiles laden
#ifndef NO_SOUND
	for (int i = 0; i < NUM_SOUNDS; i++)
		m_lpWave[i] = new SoundBuffer(sound_files[i]);
#endif

	Language = "en";
	TwoPlayerMode = 0;
	bShowCode = false;

	// animierte Objecte vordefinieren
	// zu objects hinzufügen
	for (int i = 0; i < MAX_OBJECT_ANIM; i++)
	{
		DrawPart[i].game = this;
		objects.push_back(&DrawPart[i]);
	}

	game_timer = new Timer(1000);
	menu_timer = new Timer(1000);
	begin_time = new Timer(1000);
	fps = fps_show = 0;
	Direction = 0;
}

// --------------------------------------------------------------------
//	Terminate Game
// --------------------------------------------------------------------

Game::~Game()
{
	for (int i = 0; i < NUM_SOUNDS; i++)
		delete m_lpWave[i];

	delete pFrame;
	delete pFont32;
	delete pFont16;

	delete game_timer;
	delete menu_timer;
	delete begin_time;

	objects.clear();
}

void Game::on_key_down(const SDL_Keycode key)
{
	if (State == GAME_PAUSED)
	{
		State = GAME_ACTIVE;
		return;
	}

	switch (key)
	{
#ifdef SHOW_DEBUG
		case SDLK_PLUS:
			if (*player.Level < 99)
				*player.Level += 1;
			else
				*player.Level = 1;
			State = GAME_NEXTLEVEL;
			break;

		case SDLK_MINUS:
			if (*player.Level == 1)
				*player.Level = 99;
			else
				*player.Level -= 1;
			State = GAME_NEXTLEVEL;
			break;
#endif
		case SDLK_LEFT:
			if (State == GAME_ACTIVE)
				Direction = (Direction & 131) + MAKE_LEFT;
			break;

		case SDLK_RIGHT:
			if (State == GAME_ACTIVE)
				Direction = (Direction & 131) + MAKE_RIGHT;
			break;

		case SDLK_UP:
			if (State == GAME_ACTIVE)
				Direction = (Direction & 140) + MAKE_UP;
			break;

		case SDLK_DOWN:
			if (State == GAME_ACTIVE)
				Direction = (Direction & 140) + MAKE_DOWN;
			break;

		case SDLK_SPACE:
			if (State == GAME_ACTIVE)
				Direction |= MAKE_FIRE;
			break;

		case SDLK_ESCAPE:
			m_done = true;
			State = GAME_QUIT;
			break;

		case SDLK_F2: // Level erneut starten
			if (State == GAME_ACTIVE)
			{
				NewLevelStart = true;
				*player.Lives -= 1;

				if (*player.Lives > 0)
					State = GAME_NEXTLEVEL;
				else
					State = GAME_NOLIVES;
			}
			break;

		case SDLK_PAUSE:
			State = GAME_PAUSED;
			break;

		case SDLK_LCTRL:
		case SDLK_RCTRL:
			ExtractFromPocket(0);
			PlayWave(S_TYPE);
			break;

		default:
			if (State == GAME_NOLIVES)
			{
				m_done = true;
				State = GAME_OVER;
			}
	}
}

void Game::on_key_up(const SDL_Keycode key)
{
	switch (key)
	{
		case SDLK_LEFT:
		case SDLK_RIGHT:
			Direction &= BREAK_HORIZ;
			break;

		case SDLK_UP:
		case SDLK_DOWN:
			Direction &= BREAK_VERT;
			break;

		case SDLK_SPACE:
			Direction &= BREAK_FIRE;
			break;
	}
}

// --------------------------------------------------------------------
//	Haupt-Message-Loop
// --------------------------------------------------------------------

bool Game::run(void)
{
	SDL_Event event;
	m_done = false;

	// Lade Texte
	labels[LBL_PAUSE]      = "have a brake ... (?)";
	labels[LBL_PLAYER_ONE] = "Player One";
	labels[LBL_PLAYER_TWO] = "Player Two";

	// Start the game
	begin_time->reset();

	while (!m_done)
	{
		time_elapsed = (float)(begin_time->time_passed()) / 1000.0;
		begin_time->reset();

		if (State == GAME_NEXTLEVEL)
			StartLevel();

		if (State > GAME_ACTIVE || State == GAME_NOLIVES)
		{
			if (State == GAME_PAUSED)
			{
				pwindow.clear();
				DrawText(labels[LBL_PAUSE], 0, 2 * 84, SET_CENTER);
			}
			else if (State == GAME_NOLIVES)
			{
				if (TwoPlayerMode)
				{
					if (player.ActualPlayer)
						DrawText(labels[LBL_PLAYER_TWO], 0, 2 * 68, SET_CENTER);
					else
						DrawText(labels[LBL_PLAYER_ONE], 0, 2 * 68, SET_CENTER);
				}
				DrawText("GAME OVER", 0, 2 * 84, SET_CENTER);
			}
		}
		else
			State = GameLoop();

		pwindow.flip();

		while (SDL_PollEvent(&event))
			switch (event.type)
			{
				case SDL_KEYDOWN:
					on_key_down(event.key.keysym.sym);
					break;

				case SDL_KEYUP:
					on_key_up(event.key.keysym.sym);
					break;
			}

	}
	return(false);
}

// --------------------------------------------------------------------
//	Level starten
// --------------------------------------------------------------------

bool Game::StartLevel(void)
{
	bool ret;
	int i;
	char str[80];

	if (!NewLevelStart)
	{
		if (TwoPlayerMode) player.Next();

		if (*player.Lives <= 0)
			if (TwoPlayerMode) player.Next();
	}

	// maximal 99 Level
	if (*player.Level > 99)
		*player.Level = 1;

	if ((ret = map.Load(*player.Level)) == true) // Level laden
	{
		GameState = 0;

		// ansonsten geht's los...
		Timeless = ((Levtime = map.GetTime()) == 0) ? true : false;

		Max_It = map.GetMaxItems();
		fps = fps_show = 0;

		for (i = 0; i < POCKET_SIZE - 1; i++)
			Pocket[i] = -1;
		for (i = 0; i < *player.Lives - 1; i++)
			Pocket[i] = SPRITE_FIGUR;
		PocketCnt = *player.Lives - 1;

		SetFigStart();
	}

	if (!NewLevelStart)
	{
		// Levelnummer anzeigen
		for (i = 0; i < 200; i++)
		{
			pwindow.clear();
			if (TwoPlayerMode)
			{
				if (player.ActualPlayer)
					DrawText( labels[LBL_PLAYER_TWO], 0, 2 * 68, SET_CENTER);
				else
					DrawText( labels[LBL_PLAYER_ONE], 0, 2 * 68, SET_CENTER);
			}

			sprintf(str, "LEVEL %2d", *player.Level);
			DrawText( str, 0, 2 * 84, SET_CENTER);
/*			if(LevTab[Level].Code != 0) // Code ausgeben
			{
				sprintf(sText,"%06lu", (unsigned long)LevTab[Level].Code * (unsigned long)LevTab[0].Code);
				DrawText( sText,0, 108*LINE_BYTES,SET_CENTER);
			}
*/
/*			if(LevTab[Level].Code != 0) // Code vorhanden
				CodeLev = Level;
				port2=0; // warten(Code notieren, Playerwechsel)
			do { } while(!port2);
*/
			FadeScr(F_IN, i / 25);
			GameMenu();
			pwindow.flip();
		}

		// Level anzeigen
		for (i = 0; i < 80; i++)
		{
			map.Draw();
			FadeScr(F_IN, i / 10);
			pwindow.flip();
		}
	}
	NewLevelStart = false;
	menu_timer->reset();

	return(ret);
}

// --------------------------------------------------------------------
//	Textausgaben
// --------------------------------------------------------------------

void Game::DrawText(std::string InputText, int x, int y, int mode)
{
//	Color dColor;
//	Font_Size size;
//
//	if(mode & SET_SELECTED)
//		dColor = COLOR_GRAY;
//	else
//		dColor = COLOR_WHITE;
//
//	if(mode & SET_SMALL)
//		size =  F_SIZE_MEDIUM;
//	else
//		size =  F_SIZE_BIG;

/*	if(mode & SET_CENTER)
		RL?	x= (SCR_W - pFont32->get_width(InputText))/2;
*/
	pFont32->draw(x, y, InputText.c_str());
}

// --------------------------------------------------------------------
//	Level FADE IN / OUT
// --------------------------------------------------------------------

void Game::FadeScr(int mode, int i)
{
	int lx = 0, ly = 0;
	int height = 352;

	if (mode & F_FULL)
		height = SCR_H;

	if (mode & F_IN)
	{
		for (ly = 0; ly < height; ly += RASTER)
			for (lx = 0; lx < SCR_W; lx += RASTER)
				map.DrawTile(lx, ly, A_FADE + i);
	}
	else if (mode & F_OUT)
		for (int f = 0; f < 36; f++)
		{
			for (ly = 0; ly < height; ly += RASTER)
				for (lx = 0; lx < SCR_W; lx += RASTER)
					map.DrawTile(lx, ly, A_FADE + 8 - f / 4);
			pwindow.flip();
		}
}

// --------------------------------------------------------------------
//	Demo Vorschau
// --------------------------------------------------------------------

void Game::ShowDemo(void)
{
	unsigned int time = 0, display = 0;
	int xDist = 0;
	int Last = 0, Lev;
	KollMapCell *pLScreen = map.pKoll;
	int lx = 0, ly = 0;
	int i;
	SDL_Event event;

	Timer m_second(1500);
	Timer m_fade_second(50);
	begin_time->reset();

	State = GAME_DEMO;
	Max_It = 0;
	*player.Score = 0;
	srand(rand() & 127);

	do
	{
#ifdef SHOW_DEBUG
#  define NUM_DISPLAY 100
		if (Lev < 99)
			Lev++;
		else
			Lev = 1;
#else
#  define NUM_DISPLAY 10
		while ((Lev = (rand() & 127) + 1) > 99) ;
		if (Last == Lev)
			Lev++;
		Last = Lev;
#endif

		FadeScr(F_OUT, 0);
		map.Load(Lev);

		Timeless = ((Levtime = map.GetTime()) == 0) ? true : false;

		Max_It = map.GetMaxItems();

		SetFigStart();
		time = display;

		for (i = 0; i < 9; i++)
		{
			if (State != GAME_DEMO)
				return;

			m_fade_second.reset();
			do
			{
				time_elapsed = (float)(begin_time->time_passed()) / 1000.0;
				begin_time->reset();
				SetLoop();
				FadeScr(F_IN, i);
			} while (!m_fade_second.elapsed());
			pwindow.flip();
		}

		m_second.reset();
		do
		{
			time_elapsed = (float)(begin_time->time_passed()) / 1000.0;
			begin_time->reset();
			SetLoop();
			pwindow.flip();
		} while (!m_second.elapsed());

		while (SDL_PollEvent(&event))
			if (event.type == SDL_KEYDOWN)
				on_key_down(event.key.keysym.sym);

	} while (++display < NUM_DISPLAY); // DemoShowtime
}

//	---------------------------------------------------------
//		Subroutine:	Bildschirmwechsel im Landschaft
//												mit oder ohne Animation
//	---------------------------------------------------------

void Game::FiScrSet(void)
{
	Object_Animate *dp;
	list<Object*>::iterator it;

	for (it = objects.begin(); it != objects.end(); it++)
	{
		if ((*it)->get_type() == OBJ_ANIMATE)
		{
			dp = (Object_Animate*) (*it);
			dp->mode = 0;
		}
	}

	SprListOff = A_FIGUR;
}

//	---------------------------------------------------------
//		Subroutine:  Spielfigur anhalten
//	---------------------------------------------------------

void Game::StopFigur(void)
{
	XSpeed = YSpeed = 0;
}

//	---------------------------------------------------------
//		Subroutine:  beim Start eines Levels diverse
//												Einstellungen vornehmen
//	---------------------------------------------------------

void Game::SetFigStart(void)
{
	map.GetFigurPosition(&x, &y);

	FiScrSet();

	SprAnimNum = 0; // Listenoffset setzen

	XSpeed = YSpeed = 0;
}

//	---------------------------------------------------------
//		Subroutine:  Exit setzen nach
//												einsammeln des letzten Items
//	---------------------------------------------------------

void Game::SetExit(void)
{
	map.SetExitAnimation();
}

//	---------------------------------------------------------
//		Koll	=	Zeiger auf aktuelle Kollisionsparameter
//					=	Returnwert der ItemSubroutine f. Figurweiterbewegung
//						true  = weiterbewegen
//						false = stehenbleiben
//	---------------------------------------------------------

short int Game::KollissionDetect(KollMapCell *Koll)
{
	if (!Koll->ebeneM || Koll->flag == F_OPEN)
	{
		if (Koll->ebeneG >= I_BALLOON)
			return(ItBall(Koll));
		else
		{
			switch (map.GetTileType(Koll->ebeneG))
			{
				case TYPE_ICE:
					return(ItIce(Koll));
				case TYPE_BLACKHOLE:
					return(ItFall(Koll));
				case TYPE_WALL:
					return(NormWall());
				case TYPE_INVBUMP:
					return(ItBumper());
				default:
					return(TakeItem(Koll));
			}
		}
	}
	else
	{
		switch (map.GetTileType(Koll->ebeneM))
		{
			case TYPE_MOVEABLE:
				return(MoveWall(Koll));
			case TYPE_DESTROYABLE:
				return(DestWall(Koll));
			case TYPE_MIRROR:
				return(MirrorNorm(Koll));
			case TYPE_KILLENEMY:
				return(KillEnemy());
			case TYPE_FLIPFLOP:
				return(FlipFlop(Koll));
			case TYPE_CHANGER:
				return(FlipPlaceWall(Koll));
			case TYPE_BUMPWALL:
				return(BumpWall(Koll));
			case TYPE_KILLWALL:
				return(KillWall(Koll));
			case TYPE_KEYLOCK:
				return(DoorWall(Koll));
			case TYPE_CLICKWALL:
				return(ClickWall(Koll));

			default:
				return(NormWall());
		}

	}
	return(true);
}

//	---------------------------------------------------------
//		Subroutine:   Spielfigur hat Kollision mit
//												Item ausgeloest
//	---------------------------------------------------------

short int Game::TakeItem(KollMapCell *Koll)
{
	switch (Koll->ebeneG)
	{
		case 1: // Punkte Items
		case 2:
		case 3:
		case 4:
		case 5:
			if (JoyDir & 128) return(true);
			PlayWave(S_ITEM);
			*player.Score += map.GetTilePoints(Koll->ebeneG);
			Koll->ebeneG = 0;
			if (Max_It) Max_It--;
			if (Max_It <= 0) SetExit();
			break;

		case I_HAMMER:
		case I_KEY:
		case I_LIVE:
		case I_PLACK:
			if (JoyDir & 128) return(true);
			if (InsertPocket())
			{
				Pocket[0] = Koll->ebeneG;
				if (Koll->ebeneG == I_LIVE)
				{
					++*player.Lives;
					Pocket[0] = SPRITE_FIGUR;
				}
				PlayWave(S_ITEM);
				Koll->ebeneG = 0;
			}
			break;

		case I_TIME: // ItTime
			if (JoyDir & 128) return(true);
			PlayWave(S_ITEM);
			if (Timeless)
				*player.Score += map.GetTilePoints(Koll->ebeneG); // Punkte +
			else
				Levtime += map.GetTilePoints(Koll->ebeneG); // Zeit +
			Koll->ebeneG = 0;
			break;

		case I_CODE: // ItCode
			if (JoyDir & 128) return(true);
			PlayWave(S_ITEM);
			// CodeLevel = *player.Level;
			// CodeInput=true; // Ausgabe am Schluss nochmal
			bShowCode = true;
			Koll->ebeneG = 0;
			break;

		case I_KILL: // ItKill
			if (JoyDir & 128) return(true);
			Koll->ebeneG = 0;
			KillWall(Koll);
			break;

		case I_BOMB: // ItBomb
			if (JoyDir & 128) return(ItBomb());
			break;

		case I_INVRISS:
		case I_INVRISS + 1:
		case I_INVRISS + 2:
		case I_INVRISS + 3:
		case I_INVRISS + 4:
			return(ItRiss(Koll));

		case 22:
		case 23:
			if (JoyDir & LEFT) return(NormWall());
			break;

		case 24:
		case 25:
			if (JoyDir & RIGHT) return(NormWall());
			break;

		case 26:
		case 27:
			if (JoyDir & UP) return(NormWall());
			break;

		case 28:
		case 29:
			if (JoyDir & DOWN) return(NormWall());
			break;

		case 30: // ItNewStart
		case 31:
			if (map.SetFigurPosition())
				PlayWave(S_BEEP);
			break;

		case I_TELEPORT:
		case I_TELEPORT + 1:
		case I_TELEPORT + 2:
			return(ItTeleport(Koll));

		case I_EXIT:
		case I_EXIT + 1:
			return(ItExit(Koll));
	}

	return(true);
}

// --------------------------------------------------------------------
//	freien Redraw-Puffer suchen
// --------------------------------------------------------------------

Object_Animate* Game::GetFreeBuffer(void)
{
	Object_Animate *buffer;
	list<Object*>::iterator it;

	for (it = objects.begin(); it != objects.end(); it++)
	{
		if ((*it)->get_type() == OBJ_ANIMATE)
		{
			buffer = (Object_Animate*) (*it);
			if (!buffer->mode) break;
		}
	}
	return buffer;
}

//	---------------------------------------------------------
//	  Balloonroutine
//	---------------------------------------------------------

short int Game::ItBall(KollMapCell *Koll)
{
	short int RNum;
	Object_Animate *dp;

	if (JoyDir & 128)
	{
		dp = GetFreeBuffer();
		if (dp->mode > 0) return(true);

		*player.Score += map.GetTilePoints(0);

		Koll->ebeneG = Koll->flag5; // gepuffertes Item einblenden
		Koll->flag = 0;             // Balloonflag loeschen

		if (!Koll->ebeneG) Koll->mask = 0;

		PlayWave(S_BANG);

		dp->x = (map.wKollX << 5) + map.wScrXoff;
		dp->y = (map.wKollY << 5) + map.wScrYoff;
		dp->mode = RM_ANIMATE;
		dp->max_anim = 4;
		dp->tile = A_BALLBRK;
		dp->counter->intervall = 90;
	}

	return(true);
}

//	---------------------------------------------------------
//		Teleporter
//	---------------------------------------------------------

short int Game::ItTeleport(KollMapCell *Koll)
{
	int i;
	int last, d0, d1;
	Timer *tele_timer = new Timer(150);
	PlayWave(S_WARP);

	// Figur auf Position des Teleporters ausrichten
	x = (map.wKollX << 5); // x-Rest abschneiden
	y = (map.wKollY << 5); // y-Rest abschneiden

	// Position der Figur und Levelspezifische Variablen
	//entsprechend umrechnen
	last = SprListOff;
	SprListOff = A_FIGUR_FALL; // Fall-Animationssequenz
	SprAnimNum = 0;

	tele_timer->reset();

	do
	{
		do
		{
			SetLoopKollision();
		} while (!tele_timer->elapsed());
		SprAnimNum++;
		tele_timer->reset();
	} while (SprAnimNum < 6);

	x = (Koll->vbindX << 5);
	y = (Koll->vbindY << 5);

	map.wKollPageDistance = 0;

	for (d1 = (int)x, d0 = 0;;)
	{
		d1 -= XSCROFF;
		if (d1 <= 0)
			break;
		map.wKollPageDistance += 19;
		d0 += XSCROFF;
	}
	x -= d0; map.wScrXoff = d0;

	for (d1 = (int)y, d0 = 0;;)
	{
		d1 -= YSCROFF;
		if (d1 <= 0)
			break;
		map.wKollPageDistance += (map.wKollLine * 10);
		d0 += YSCROFF;
	}
	y -= d0;
	map.wScrYoff = d0;

	StopFigur();
	FiScrSet();
	PlayWave(S_WARP);

	SprListOff = A_FIGUR_FALL; // Fall-Animationssequenz
	SprAnimNum = 6;

	do
	{
		do
		{
			SetLoopKollision();
		} while (!tele_timer->elapsed());
		SprAnimNum--;
		tele_timer->reset();
	} while (SprAnimNum > 0);

	SprListOff = last;
	delete tele_timer;

	return(false);
}

//	---------------------------------------------------------
//		Riss vergroessern
//	---------------------------------------------------------

short int Game::ItRiss(KollMapCell *Koll)
{
	if (Koll->timer <= 0.3)
		Koll->timer += time_elapsed;
	else
	{
		Koll->timer = 0.0;
		Koll->ebeneG++; // Riss vergroessern
		PlayWave(S_FURZ);
	}
	return(true);
}

//	---------------------------------------------------------
//	  In schwarzes Loch hineinfallen
//		oder Platte auf schwarzes Loch legen
//	---------------------------------------------------------

short int Game::ItFall(KollMapCell *Koll)
{
	Object_Animate *dp;
	int RNum;
	Timer *fall_timer = new Timer(200);

	if ((JoyDir & 128) && (Pocket[0] == I_PLACK))
	{
		dp = GetFreeBuffer();
		if (dp->mode > 0)
			return(true);

		ExtractFromPocket(I_PLACK);
		Koll->ebeneU = UG_PLATTE; // neuer Hintergrund

		dp->mode = RM_ANIMATE;

		dp->change_map((map.wKollX << 5) + map.wScrXoff, (map.wKollY << 5) + map.wScrYoff, CLEAR, CLEAR, CLEAR);

		dp->max_anim = 9;
		dp->tile = A_PLATTE;
		dp->counter->intervall = 60;

		PlayWave(S_POP);
		return(false);
	}

	if ((KollRichtung & 3))
	{
		HorKolFlg = swap(HorKolFlg);
		if ((map.wKollX << 5) == x)
			if ((Koll + 1)->ebeneM)
				if ((short int)HorKolFlg)
					return(NormWall());

		HorKolFlg = swap(HorKolFlg);
		if ((short int)VerKolFlg & 0x03c0) // überlappe Figur-y mit Loch
		{
			if ((map.wKollX << 5) <= x) // Mike 20.06.01 <=
			{
				if ((Koll + 1)->ebeneG != I_LOCH)
					goto YKollNorm;
				if ((short int)HorKolFlg & 0xffff)
					goto FallDown;
				return(true); // kein Loch rechts daneben
			}
			if ((Koll - 1)->ebeneG != I_LOCH)
				goto YKollNorm;
			else
			{
				if ((short int)HorKolFlg & 0xffff)
					goto FallDown;
				return(true); // kein Loch links daneben
			}
		}
		else
			return(true);

YKollNorm:

		if (!((short int)HorKolFlg & 0x03c0))
			return(true); // kein Loch rechts/links
		goto FallDown;
	}
	else
	{
		if ((map.wKollY << 5) == y)
		{
			Koll += map.wKollLine;
			VerKolFlg = swap(VerKolFlg);
			if (Koll->ebeneM)
				if ((short int)VerKolFlg)
					return(NormWall());
			Koll -= map.wKollLine;
			VerKolFlg = swap(VerKolFlg);
		}
		if ((short int)HorKolFlg & 0x03c0) // überlappe Figur-x mit Loch
		{
			if ((map.wKollY << 5) <= y) // Mike 20.06.01 <=
			{
				Koll += map.wKollLine;
				if (Koll->ebeneG != I_LOCH)
					goto XKollNorm;
				if ((short int)VerKolFlg & 0xffff)
					goto FallDown;
				return(true); // kein Loch darunter
			}
			Koll -= map.wKollLine;
			if (Koll->ebeneG != I_LOCH)
				goto XKollNorm;
			else
			{
				if ((short int)VerKolFlg & 0xffff)
					goto FallDown;
				return(true); // kein Loch darüber
			}
		}
		else
			return(true);

XKollNorm:

		if (!((short int)VerKolFlg & 0x03c0))
			return(true); // kein Loch oben/unten
		goto FallDown;
	}

FallDown:

	if (--*player.Lives <= 0)
		GameState |= GAME_NOLIVES;
	ExtractFromPocket(I_LIVE);
	GameState |= FIGUR_FALLING; // Sp-Figur faellt

	PlayWave(S_FALL);

	SprListOff = A_FIGUR_FALL;
	SprAnimNum = 0;

	fall_timer->reset();

	do
	{
		do
		{
			SetLoopKollision();
		} while (!fall_timer->elapsed());
		SprAnimNum += 2;
		fall_timer->reset();
	} while (SprAnimNum < 8);

	fall_timer->intervall = 400;
	do
	{
		SetLoopKollision();
	} while (!fall_timer->elapsed());

	fall_timer->reset();

	PlayWave(S_DROP);

	do
	{
		SetLoopKollision();
	} while (!fall_timer->elapsed());
	delete fall_timer;

	GameState ^= FIGUR_FALLING;

	if ((GameState & GAME_NOLIVES) == 0)
	{
		SetFigStart();
		StopFigur();
	}

	return(false);
}

//	---------------------------------------------------------
//		Eisflaeche
//	---------------------------------------------------------

short int Game::ItIce(KollMapCell *Koll)
{
	if (!Direction)
		return(true);

	if ((KollRichtung & 3))
	{
		if (Direction == JoyDir)
			if (YSpeed < MAXSPEED + 16 * SPEED_MULTI)
				YSpeed += time_elapsed * ACELFACT;
			else
				YSpeed = MAXSPEED + 16 * SPEED_MULTI;

		return(true);
	}
	else
	{
		if (Direction == JoyDir)
			if (XSpeed < MAXSPEED + 16 * SPEED_MULTI)
				XSpeed += time_elapsed * ACELFACT;
			else
				XSpeed = MAXSPEED + 16 * SPEED_MULTI;
		return(true);
	}
	return false;
}

//	---------------------------------------------------------
//		Bombe zuenden
//	---------------------------------------------------------

short int Game::ItBomb(void)
{
	Object_Animate *dp;

	dp = GetFreeBuffer();
	if (dp->mode > 0)
		return(true);

	dp->x = (map.wKollX << 5) + map.wScrXoff;
	dp->y = (map.wKollY << 5) + map.wScrYoff;
	dp->mode = RM_SEARCH;

	dp->change_map(dp->x, dp->y, NOTHING, NOTHING, NOTHING);

	dp->max_anim    = 4;
	dp->tile = A_BOMB;
	dp->counter->intervall  = 300;

	return(false);
}

//	---------------------------------------------------------
//		Mauer unsichtbar u. Bumperfunktion
//	---------------------------------------------------------

short int Game::ItBumper(void)
{
	if (KollRichtung & 3) // UP & DOWN
	{
		if (YSpeed > 2 * SPEED_MULTI) // Abbruch bei Stillstand
		{
			JoyDir ^= 3;
			YSpeed = MAXSPEED + 16 * SPEED_MULTI;
			PlayWave(S_BUMP);
		}
	}
	else if (XSpeed > 2 * SPEED_MULTI) // Abbruch bei Stillstand
	{
		JoyDir ^= 12; // LEFT & RIGHT
		XSpeed = MAXSPEED + 16 * SPEED_MULTI;
		PlayWave(S_BUMP);
	}
	return(true);
}

//	---------------------------------------------------------
//		Exit
//	---------------------------------------------------------

short int Game::ItExit(KollMapCell *Koll)
{
	if (JoyDir & 128)
		return(false); // Feuerknopf ?

	*player.Level += 1;
	GameState |= GAME_NOLIVES + GAME_NEXTLEVEL; // Landschaft beendet

	PlayWave(S_EXIT, 0.3f);

	// Figur ausrichten ...
	x = (map.wKollX << 5);
	y = (map.wKollY << 5);

	// ... im Exit versinken lassen
	Timer *fall_timer = new Timer(200);
	SprListOff = A_FIGUR_FALL;
	SprAnimNum = 0;
	do
	{
		do
		{
			SetLoopKollision();
		} while (!fall_timer->elapsed());
		SprAnimNum += 2;
		fall_timer->reset();
	} while (SprAnimNum < 8);

	// Kollisionsparameter loeschen
	Koll->mask = 0;
	Koll->ebeneG = 0;

	// Übrige Spielzeit zum Score addierern
	Levtime /= 10;
	fall_timer->intervall = 100;
	do
	{
		do
		{
			SetLoopKollision();
		} while (!fall_timer->elapsed());
		Levtime--;
		*player.Score += 1;
		PlayWave(S_BEEP, 0.3f);
		fall_timer->reset();
	} while (Levtime > 0);

	delete fall_timer;
	return(false);
}

//	-------------------
//		Mauer - Routinen
//	-------------------


//	---------------------------------------------------------
//		Mauerberuehrung feststellen
//		true = abgefedert      false = Stillstand
//	---------------------------------------------------------

short int Game::Wall(void)
{
	if ((KollRichtung & 12)) // LEFT & RIGHT
	{
		if (XSpeed <= 2 * SPEED_MULTI) {
			XSpeed = 0;
			return(false); // Abbruch bei Stillstand
		}
		if (XSpeed > 6 * SPEED_MULTI)
		{
			JoyDir ^= 12;
			XSpeed -= 4 * SPEED_MULTI;
		}
		else
			XSpeed = 1 * SPEED_MULTI;
	}
	else // UP & DOWN
	{
		if (YSpeed <= 2 * SPEED_MULTI) {
			YSpeed = 0;
			return(false); // Abbruch bei Stillstand
		}
		if (YSpeed > 6 * SPEED_MULTI)
		{
			JoyDir ^= 3;
			YSpeed -= 4 * SPEED_MULTI;
		}
		else
			YSpeed = 1 * SPEED_MULTI;
	}
	return(true);
}

//	---------------------------------------------------------
//		Mauer normal
//	---------------------------------------------------------

short int Game::NormWall(void)
{
	if (Wall())
		PlayWave(S_WALL);
	return(false);
}

//	---------------------------------------------------------
//		Mauer	zerstoerbar
//	---------------------------------------------------------

short int Game::DestWall(KollMapCell *Koll)
{
	Object_Animate *dp;

	if ((JoyDir & 128)) // Feuertaste gedrueckt
	{
		if (Pocket[0] == 6) // Hammer in Pocket ?
		{
			map.wKollX += (map.wScrXoff >> 5);
			map.wKollY += (map.wScrYoff >> 5);

			dp = GetFreeBuffer();
			if (dp->mode > 0)
				return(false);

			if (dp->mode != RM_SEARCH)
				dp->mode = RM_ANIMATE;

			dp->change_map((map.wKollX << 5), (map.wKollY << 5), CLEAR, NOTHING, CLEAR);

			dp->max_anim = 4;
			dp->tile = A_BRKWALL;
			dp->counter->intervall = 90;

			PlayWave(S_CRASH);
		}
		Wall();
	}
	else if (Wall())
		PlayWave(S_WBRK);

	return (false);
}

void Game::DestW2(void)
{
	Object_Animate *dp;

	dp = GetFreeBuffer();
	if (dp->mode > 0)
		return;

	if (dp->mode != RM_SEARCH)
		dp->mode = RM_ANIMATE;

	dp->change_map((map.wKollX << 5), (map.wKollY << 5), CLEAR, NOTHING, CLEAR);

	dp->max_anim = 4;
	dp->tile = A_BRKWALL;
	dp->counter->intervall = 60;

	PlayWave(S_CRASH);
}

//	---------------------------------------------------------
//		Mauer verschiebbar
//	---------------------------------------------------------

short int Game::MoveWall(KollMapCell *Koll)
{
	KollMapCell *LpKoll;

	LpKoll = Koll;

	if (JoyDir & LEFT)
	{
		// nicht verschieben bei geringem Abstand
		if (XSpeed < MBFIGDIST)
			goto NoMove;
		// Kollisionsausgrenzung
		if ((VerKolFlg & 0x0ff0) == 0)
			goto NoMove;

		LpKoll--;
		if (!LpKoll->ebeneM)
			goto Push;
		goto NoMove;
	}

	if (JoyDir & RIGHT)
	{
		if (XSpeed < MBFIGDIST)
			goto NoMove;
		if ((VerKolFlg & 0x0ff0) == 0)
			goto NoMove;

		LpKoll++;
		if (!LpKoll->ebeneM)
			goto Push;
		goto NoMove;
	}

	if (JoyDir & UP)
	{
		if (YSpeed < MBFIGDIST)
			goto NoMove;
		if ((HorKolFlg & 0x0ff0) == 0)
			goto NoMove;

		LpKoll -= map.wKollLine;
		if (!LpKoll->ebeneM)
			goto Push;
		goto NoMove;
	}

	if (JoyDir & DOWN)
	{
		if (YSpeed < MBFIGDIST)
			goto NoMove;
		if ((HorKolFlg & 0x0ff0) == 0)
			goto NoMove;

		LpKoll += map.wKollLine;
		if (!LpKoll->ebeneM)
			goto Push;
		goto NoMove;
	}

Push: // Moveblock kann verschoben werden

	LpKoll->mask = Koll->mask; // Werte des Moveblocks an neuen
	LpKoll->ebeneM = Koll->ebeneM; // Standort kopieren

	if (Koll->ebeneG) // war an letzter Position ein Gegenstand ?
	{
		Koll->mask = 0x3ffc; // Default Maske eintragen
		Checkbuffer(Koll); // ggf. pruefen auf Besonderheiten
	}
	else
		Koll->mask = 0;


	PlayWave(S_MOVE);

	// neue Position prüfen
	switch (LpKoll->ebeneM)
	{
	case MBHOLZ: // versenkbarer Stein ?
		if (LpKoll->ebeneG == I_LOCH) // schwarzes Loch ?
		{
			LpKoll->mask = 0;
			LpKoll->ebeneG = 0;
			LpKoll->ebeneM = 0;
			LpKoll->ebeneU = UG_HOLZ;
			PlayWave(S_DROP);
		}
		break;

	case MBLOCH: // Block mit Loch ?
		if (LpKoll->ebeneG >= I_BLINK && LpKoll->ebeneG <= I_BLINK + 3) // Blinkendes Loch ?
			ExeClick(LpKoll);
		break;
	}

	switch (LpKoll->ebeneG)
	{
	case I_BOMB: // Bombe im Puffer?
		LpKoll->mask = 0x0180;
		LpKoll->ebeneG = I_INVRISS + 1; // 2.ter Riss
		LpKoll->timer = 0;
		PlayWave(S_CRASH);
		break;
	}
	Koll->ebeneM = 0; // letzten Moveblock-Standort loeschen

	XSpeed = YSpeed = 0;
	return(false);

NoMove: // zu kurze Distanz, kein verschieben möglich

	switch (Koll->ebeneM)
	{
	case MB_MIRROR:
	case MB_MIRROR + 1:
	case MB_MIRROR + 2:
	case MB_MIRROR + 3:
		MirrorNorm(Koll); break;
	default:
		NormWall();
	}

	return(false);
}

//---- zu MoveWall ---------------------------------------

void Game::Checkbuffer(KollMapCell *Koll)
{
	// vorm verschieben alten Pufferinhalt
	// pruefen auf "Blinkendes Loch"
	if (Koll->ebeneM == MBLOCH) // Block mit Loch ?
		if (Koll->ebeneG >= I_BLINK && Koll->ebeneG <= I_BLINK + 3)
			ExeClick(Koll);
}

//	---------------------------------------------------------
//		Mauer zum oeffnen (mit Schlsselloch)
//	---------------------------------------------------------

short int Game::DoorWall(KollMapCell *Koll)
{
	Object_Animate *dp;

	if (!Wall())
		return(false);
	PlayWave(S_WALL);

	if (Pocket[0] != I_KEY)
		return(true); // Schlüssel in Pocket ?

	dp = GetFreeBuffer();
	if (dp->mode > 0)
		return(false);

	ExtractFromPocket(I_KEY);

	dp->x = (map.wKollX << 5) + map.wScrXoff;;
	dp->y = (map.wKollY << 5) + map.wScrYoff;
	dp->mode = RM_ANIMATE;
	dp->max_anim = 6;
	dp->tile = A_FADE;
	dp->counter->intervall = 90;

	Koll->mask = 0;
	Koll->ebeneM = 0;

	PlayWave(S_DOOR);

	return(false);
}

//	---------------------------------------------------------
//		Mauer mit Schalter
//	---------------------------------------------------------

short int Game::ClickWall(KollMapCell *Koll)
{
	if (!Wall())
		return(false);

	if (Koll->ebeneM == W_CLICKON) // Schalter ändern
		Koll->ebeneM = W_CLICKOFF;
	else
		Koll->ebeneM = W_CLICKON;

	ExeClick(Koll);

	return(false);
}



//	---------------------------------------------------------
//		Mauer (Flip-Flop-blinken) als Ausloeser
//	---------------------------------------------------------

short int Game::FlipFlop(KollMapCell *Koll)
{
	Object_Animate *dp;

	if (!Wall())
		return(false);

	dp = GetFreeBuffer();
	if (dp->mode > 0)
		return(false);

	dp->x = (map.wKollX << 5) + map.wScrXoff;
	dp->y = (map.wKollY << 5) + map.wScrYoff;
	dp->mode = RM_ANIMATE;
	dp->max_anim = 5;
	dp->tile = A_FLIPFLOP;
	dp->counter->intervall = 90;

	ExeClick(Koll);

	return(false);
}

//	---------------------------------------------------------
//		Mauer kostet bei Berhrung Ein Leben
//	---------------------------------------------------------

short int Game::KillWall(KollMapCell *Koll)
{
	Timer *kill_timer = new Timer(100);

	if (--*player.Lives <= 0) GameState |= GAME_NOLIVES;
	ExtractFromPocket(I_LIVE);

	PlayWave(S_CRASH, 0.8f);

	SprListOff = A_FIGUR_BRK;
	SprAnimNum = 0;

	do
	{
		do
		{
			SetLoopKollision();
		} while (!kill_timer->elapsed());
		SprAnimNum++;
		kill_timer->reset();
	} while (SprAnimNum < 4);
	delete kill_timer;

	if ((GameState & GAME_NOLIVES) == 0)
	{
		if (Koll != NULL)
			SetFigStart();
		else
		{
			StopFigur();
			FiScrSet();
		}
	}

	return(false);
}

//	---------------------------------------------------------
//		Mauer als Reflektorspiegel fuer Baelle
//	---------------------------------------------------------

short int Game::MirrorNorm(KollMapCell *Koll)
{
	if (!Wall())
		return(false);

	PlayWave(S_BEEP);

	if (Koll->ebeneM == W_MIRROR + 3) // Spiegel ändern
		Koll->ebeneM = W_MIRROR;
	else if (Koll->ebeneM == MB_MIRROR + 3) // verschiebbaren Spiegel ändern
		Koll->ebeneM = MB_MIRROR;
	else
		Koll->ebeneM++;

	return(false);
}

//	---------------------------------------------------------
//		Mauer (Schneeflocke) schaltet bei Berhrung bis zu
//												 3 Feinde aus
//	---------------------------------------------------------

short int Game::KillEnemy(void)
{
	KollMapCell *LpKoll;
	int Num = 0, Kx, Ky;
	Object_Moving *buffer;

	if (!Wall())
		return(false);

	list<Object*>::iterator it;

	for (it = objects.begin(); it != objects.end(); it++)
	{
		if (((*it)->get_type() == OBJ_UPDOWN) || ((*it)->get_type() == OBJ_CIRCLE))
		{
			buffer = (Object_Moving*) (*it);
			if (buffer->state == OFF)
				continue;

			Kx = buffer->get_x() - RASTER;
			Ky = buffer->get_y() - RASTER;

			if ((Kx -= map.wScrXoff) < 0 || (Kx > XSCROFF)) // Enemy nicht sichtbar
				if ((Ky -= map.wScrYoff) < 0 || (Ky > YSCROFF))
					continue;

			LpKoll = map.GetLocalKollPosition(buffer->get_x(), buffer->get_y(), true);

			if (LpKoll->ebeneG) // befindet sich dort etwas an pKoll-Position ?
				continue; // ja! nächste Position suchen
			else
			{
				// Item "Herz" eintragen zeichnen etc.
				LpKoll->mask = 0x0ff0;
				LpKoll->ebeneG = 4;

				// Feind abschalten ...
				buffer->state = OFF;
				// ... und aus Liste lÃ¶schen
				delete (*it);
				it = objects.erase(it);

				PlayWave(S_FADE);
			}
			if (Num < 2)
			Num++; // nächste Position
			else
				break;
		}
	}

	return(false);
}

//	---------------------------------------------------------
//		Mauer verschiebbar bei Kollision mit anderer Mauer
//          vertauschen beide die Positionen
//	---------------------------------------------------------

short int Game::FlipPlaceWall(KollMapCell* Koll)
{
	KollMapCell *LpKoll;

	LpKoll = Koll;

	if (JoyDir & LEFT)
	{
		if (XSpeed < MBFIGDIST)
			goto NoMove;
		if ((VerKolFlg & 0x0ff0) == 0)
		goto NoMove; // Kollisionsausgrenzung

		LpKoll--;
		if (CheckNewPos(LpKoll, Koll) == true)
			goto Push;
		goto NoMove;
	}

	if (JoyDir & RIGHT)
	{
		if (XSpeed < MBFIGDIST)
			goto NoMove;
		if ((VerKolFlg & 0x0ff0) == 0)
			goto NoMove;

		LpKoll++;
		if (CheckNewPos(LpKoll, Koll) == true)
			goto Push;
		goto NoMove;
	}

	if (JoyDir & UP)
	{
		if (YSpeed < MBFIGDIST)
			goto NoMove;
		if ((HorKolFlg & 0x0ff0) == 0)
			goto NoMove;

		LpKoll -= map.wKollLine;
		if (CheckNewPos(LpKoll, Koll) == true)
			goto Push;
		goto NoMove;
	}


	if (JoyDir & DOWN)
	{
		if (YSpeed < MBFIGDIST)
			goto NoMove;
		if ((HorKolFlg & 0x0ff0) == 0)
			goto NoMove;

		LpKoll += map.wKollLine;
		if (CheckNewPos(LpKoll, Koll) == true)
			goto Push;
		goto NoMove;
	}

Push:

	PlayWave(S_MOVE);

	XSpeed = YSpeed = 0;
	return(false);

NoMove:
	NormWall();

	return(false);
}

// -- zu FlipPlaceWall -------------------------------------

bool Game::CheckNewPos(KollMapCell *New, KollMapCell *Old)
{
	unsigned short Buffer;

	switch (New->ebeneM)
	{
		case W_KEYLOCK:
		case W_CLICKON:
		case W_CLICKOFF:
		case W_WERFER:
		case W_WERFER + 1:
		case W_WERFER + 2:
		case W_WERFER + 3:
		case W_FLIPFLOP:
			return(false);

		default:
			Buffer = New->mask; // Maske ...
			New->mask = Old->mask;
			Old->mask = Buffer;

			Buffer = New->ebeneM; // Mauer ...
			New->ebeneM = Old->ebeneM;
			Old->ebeneM = (unsigned char)Buffer;

			Buffer = New->flag; // und Flags umkopieren
			New->flag = Old->flag;
			Old->flag = (unsigned char)Buffer;
	}

	return(true);
}

//	---------------------------------------------------------
//		Mauer	kann ber Kettenreaktion bei bis zu 10
//					Holzsteinen immer den letzten in der
//          Reihe eine Position weiter Schieben
//	---------------------------------------------------------

short int Game::BumpWall(KollMapCell* Koll )
{
	KollMapCell *LpKoll;
	short int i;

	LpKoll = Koll;

	if (JoyDir & LEFT)
	{
		if (XSpeed < MBFIGDIST)
			goto NoMove;
		if ((VerKolFlg & 0x0ff0) == 0)
			goto NoMove; // Kollisionsausgrenzung

		for (i = 20; i; i--, LpKoll--, map.wKollX--)
		{
			if (!LpKoll->ebeneM)
				goto NoMove;
			if (LpKoll->ebeneM == MBHOLZ)
			{
				if ((LpKoll - 1)->ebeneM)
					continue;
				//if(wKollX > 19)
					//goto NoMove;
				goto Push;
			}
		}
		goto NoMove;
	}

	if (JoyDir & RIGHT)
	{
		if (XSpeed < MBFIGDIST)
			goto NoMove;
		if ((VerKolFlg & 0x0ff0) == 0)
			goto NoMove;

		for (i = 20; i; i--, LpKoll++, map.wKollX++)
		{
			if (!LpKoll->ebeneM)
				goto NoMove;
			if (LpKoll->ebeneM == MBHOLZ)
			{
				if ((LpKoll + 1)->ebeneM)
					continue;
				//if(wKollX < 0)
					//goto NoMove;
				goto Push;
			}
		}
		goto NoMove;
	}

	if (JoyDir & UP)
	{
		if (YSpeed < MBFIGDIST)
			goto NoMove;
		if ((HorKolFlg & 0x0ff0) == 0)
			goto NoMove;

		for (i = 10; i; i--, LpKoll -= map.wKollLine, map.wKollY--)
		{
			if (!LpKoll->ebeneM)
				goto NoMove;
			if (LpKoll->ebeneM == MBHOLZ)
			{
				if ((LpKoll - map.wKollLine)->ebeneM)
					continue;
				//if(wKollY < 0)
					//goto NoMove;
				goto Push;
			}
		}
		goto NoMove;
	}

	if (JoyDir & DOWN)
	{
		if (YSpeed < MBFIGDIST)
			goto NoMove;
		if ((HorKolFlg & 0x0ff0) == 0)
			goto NoMove;

		for (i = 10; i; i--, LpKoll += map.wKollLine, map.wKollY++)
		{
			if (!LpKoll->ebeneM)
				goto NoMove;
			if (LpKoll->ebeneM == MBHOLZ)
			{
				if ((LpKoll + map.wKollLine)->ebeneM)
					continue;
				//if(wKollY > 10)
					//goto NoMove;
				goto Push;
			}
		}
		goto NoMove;
	}

Push:
	MoveWall(LpKoll);
	return(false);

NoMove:
	NormWall();

	return(false);
}

//	---------------------------------------------------------
//		Subroutine:   Wertet Beruehrung auf Schalter
//													bzw. FlipFlop aus
//	---------------------------------------------------------

void Game::ExeClick(KollMapCell *Koll)
{
	Object_Animate *dp, *dp1;
	unsigned char pG, pW;
	short int pM, RNum, pf;
	KollMapCell *EpKoll, *LpKoll;

	list<Object*>::iterator it;

	EpKoll = map.GetLocalKollPosition(Koll->vbindX, Koll->vbindY, false);

	switch (EpKoll->ebeneM)
	{
		case W_WERFER: // Ballwerfer schalten
		case W_WERFER + 1:
		case W_WERFER + 2:
		case W_WERFER + 3:
			for (it = objects.begin(); it != objects.end(); it++)
				if ((*it)->get_type() == OBJ_BALL)
				{
					Object_Ball *ball = (Object_Ball*) (*it);
					if (ball->state == OFF)
					{
						ball->StartBall(Koll, EpKoll);
						PlayWave(S_POP);
						break;
					}
				}
			break;

	default:
		// Schiebetuer an- / abschalten
		if (EpKoll->flag == F_MOVEDOOR)
		{
			for (it = objects.begin(); it != objects.end(); it++)
				if ((*it)->get_type() == OBJ_DOOR)
				{
					Object_Door *door = (Object_Door*) (*it);
					if ((Koll->vbindX << 5) == door->get_x() && (Koll->vbindY << 5) == door->get_y())
					{
						if (door->state != BUSY) // Tür ist ausgeschaltet
							door->state = BUSY;
						else // Tuer ist eingeschaltet
							door->state = ON;
						break;
					}
				}

			PlayWave(S_KEY);
			return;
		}

		// Austauschen zweier Positionen untereinander

		// Adresse der 1. Tauschposition berechnen
		LpKoll = map.GetLocalKollPosition(Koll->vbindX, Koll->vbindY, false);

		pM = LpKoll->mask;
		pG = LpKoll->ebeneG;
		pW = LpKoll->ebeneM;
		pf = LpKoll->flag;

		// Adresse der 2. Tauschposition berechnen
		EpKoll = map.GetLocalKollPosition(LpKoll->vbindX, LpKoll->vbindY, false);

		// Inhalte der Positionen vertauschen
		LpKoll->mask   = EpKoll->mask;
		LpKoll->ebeneG = EpKoll->ebeneG;
		LpKoll->ebeneM = EpKoll->ebeneM;
		LpKoll->flag   = EpKoll->flag;

		EpKoll->mask   = pM;
		EpKoll->ebeneG = pG;
		EpKoll->ebeneM = pW;
		EpKoll->flag   = pf;

		// auf beiden Positionen ein redraw ausfhren !!
		dp = GetFreeBuffer();
		dp->change_map((Koll->vbindX << 5), (Koll->vbindY << 5), NOTHING, NOTHING, NOTHING);
		dp1 = GetFreeBuffer();
		dp1->change_map((LpKoll->vbindX << 5), (LpKoll->vbindY << 5), NOTHING, NOTHING, NOTHING);

		PlayWave(S_TWI);
	}
}

//	---------------------------------------------------------
//		Subroutine:   Gegenstand in Tasche mitnehmen
//	---------------------------------------------------------

short int Game::InsertPocket(void)
{
	int loop;

	if (PocketCnt == POCKET_SIZE - 1)
		return(false); // letzer Eintrag belegt ?

	for (loop = ++PocketCnt; loop; loop--)
		Pocket[loop] = Pocket[loop - 1];

	return(true);
}

//	---------------------------------------------------------
//		Subroutine:   Gegenstand aus Tasche ablegen
//	---------------------------------------------------------

void Game::ExtractFromPocket(short int tile)
{
	short int i = 0, j = 0, P0 = Pocket[0];
	bool found = false;

	if (!tile)
	{
		for (; i < PocketCnt - 1; i++) // Item wechseln
			Pocket[i] = Pocket[i + 1];
		Pocket[i] = P0;
	}
	else
	{
		for (; i < PocketCnt; i++) // Item löschen
		{
			if (tile == Pocket[i]) // gefunden
				found = true;
			if (found)
				Pocket[i] = Pocket[i + 1];
		}
		PocketCnt--;
	}
}

// --------------------------------------------------------------------
//	Spielermenue
// --------------------------------------------------------------------
#define BASELINE 360

void Game::GameMenu(void) // Spielermenue (Score, Items, Inventar, etc.)
{
	char str[80];

	// Menü zeichnen
	pFrame->draw(0, 352);

	if (bShowCode) // Code ausgeben
	{
		static int x = 520;
		static Timer *code_timer = new Timer(10);

		sprintf(str, "%u", map.GetCode());
		pFont32->draw(x, BASELINE, str);

		if (code_timer->elapsed())
		{
			code_timer->reset();
			x--;
			if (x < 16)
			{
				x = 520;
				bShowCode = false;
			}
		}
	}
	else
	{
		sprintf(str, "%4u,%02u", *player.Score / 100, *player.Score % 100);
		pFont32->draw(220, BASELINE, str);

		// eingesammelte Items
		sprintf(str, "%-2d", Max_It);
		pFont32->draw(432, BASELINE, str);

		sprintf(str, "%3d fps", fps_show);
		pFont16->draw(336, 380, str);

		// Zeit
		sprintf(str, "%01u:%02u", Levtime / 60, Levtime % 60);
		pFont32->draw(544, BASELINE, str);

		int count = (PocketCnt <= 5) ? PocketCnt : 5;

		// Inventar anzeigen
		for (int loops = 0, x = 16 + loops * RASTER; loops < count; loops++, x += RASTER)
			map.DrawTile(x, 372, Pocket[loops]);
	}

}


void Game::SetLoopKollision(void)
{
	SetLoop();
	pwindow.flip();
}
/* --------------------------------------------------------------------
        Definitionen
   -------------------------------------------------------------------- */

#define LFIGISDIR 0x00003FFC
#define LFIGNODIR 0x00001ff8

/* --------------------------------------------------------------------
        Variablen
   -------------------------------------------------------------------- */

long Game::swap( long x )
{
	return((x << 16) + (x >> 16));
}

void Game::PlayWave(int i, float vol)
{
#ifndef NO_SOUND
	m_lpWave[i]->set_volume(vol);
	m_lpWave[i]->play();
#endif
}

// --------------------------------------------------------------------
//	Haupt-Spiel loop
// --------------------------------------------------------------------

int Game::GameLoop(void)
{
	KollMapCell *Koll;
	int ReturnFlg, Buffer;

	if (Direction)
	{
		if (!XSpeed) JoyDir = (JoyDir & 3) | (Direction & 140);
		if (!YSpeed) JoyDir = (JoyDir & 12) | (Direction & 131);
		if (Direction & 128) JoyDir |= 128;
	}

	Buffer = (int)x;
	Buffer &= 0x001F; Buffer = Buffer >> 1;
	HorKolFlg = LFIGISDIR;
	HorKolFlg <<= Buffer;

	Buffer = (int)y;
	Buffer &= 0x001F; Buffer = Buffer >> 1;
	VerKolFlg = LFIGNODIR;
	VerKolFlg <<= Buffer;
	ReturnFlg = true;

	Koll = map.GetKollPosition((int)x, (int)y);

	if (JoyDir & RIGHT)
	{
		SprListOff = A_FIGUR_R;

		Koll++;
		map.wKollX++;
		HorKolFlg = swap(HorKolFlg);
		KollRichtung = RIGHT;

		// Kollision oben
		if ((unsigned short int)VerKolFlg && (Koll->mask & (unsigned short int)HorKolFlg))
			if ((Koll->mask & (unsigned short int)VerKolFlg))
			{
				ReturnFlg = KollissionDetect(Koll);
				if (!ReturnFlg) goto jR_Move;
			}

		// Kollision unten
		VerKolFlg = swap( VerKolFlg );
		if ((unsigned short int)VerKolFlg)
		{

			Koll += map.wKollLine;
			map.wKollY++;

			if (Koll->mask & (unsigned short int)HorKolFlg)
				if ((Koll->mask & (unsigned short int)VerKolFlg))
					ReturnFlg = KollissionDetect(Koll);
		}
jR_Move:
		// Bewegung einleiten
		if (ReturnFlg)
			if (JoyDir == Direction)
			{
				if (XSpeed < MAXSPEED)
					XSpeed += time_elapsed * ACELFACT;
				x += time_elapsed * XSpeed;
			}
			else
			{
				if (XSpeed > 0.0)
				{
					XSpeed -= time_elapsed * ACELFACT;
					x += time_elapsed * XSpeed;
				}
				else
				{
					x -= time_elapsed * XSpeed;
					XSpeed = 0.0;
				}
			}

		if (x >= XSCROFF)
		{
			map.wKollPageDistance += 19;
			x -= XSCROFF;
			map.wScrXoff += XSCROFF;
		}
	}
	else if (JoyDir & LEFT)
	{
		SprListOff = A_FIGUR_L;
		KollRichtung = LEFT;

		// Kollision oben
		if ((unsigned short int)VerKolFlg && (Koll->mask & (unsigned short int)HorKolFlg))
			if ((Koll->mask & (unsigned short int)VerKolFlg))
			{
				ReturnFlg = KollissionDetect(Koll);
				if (!ReturnFlg)
					goto jL_Move;
			}

		// Kollision unten
		VerKolFlg = swap( VerKolFlg );
		if ((unsigned short int)VerKolFlg)
		{
			Koll += map.wKollLine;
			map.wKollY++;
			if ((Koll->mask & (unsigned short int)HorKolFlg))
				if ((Koll->mask & (unsigned short int)VerKolFlg))
					ReturnFlg = KollissionDetect(Koll);
		}

jL_Move:
		// Bewegung einleiten
		if (ReturnFlg)
			if (JoyDir == Direction)
			{
				if (XSpeed < MAXSPEED)
					XSpeed += time_elapsed * ACELFACT;
				x -= time_elapsed * XSpeed;
			}
			else
			{
				if (XSpeed > 0.0)
				{
					XSpeed -= time_elapsed * ACELFACT;
					x -= time_elapsed * XSpeed;
				}
				else
				{
					x += time_elapsed * XSpeed;
					XSpeed = 0.0;
				}
			}

		if (x <= 0)
		{
			map.wKollPageDistance -= 19;
			x += XSCROFF;
			map.wScrXoff -= XSCROFF;
		}
	}

	Buffer = (int)x;
	Buffer &= 0x001F; Buffer = Buffer >> 1;
	HorKolFlg = LFIGNODIR;
	HorKolFlg <<= Buffer;

	Buffer = (int)y;
	Buffer &= 0x001F; Buffer = Buffer >> 1;
	VerKolFlg = LFIGISDIR;
	VerKolFlg <<= Buffer;

	Koll = map.GetKollPosition((int)x, (int)y);

	ReturnFlg = true;

	if (JoyDir & UP)
	{
		SprListOff = A_FIGUR_U;
		KollRichtung = UP;

		if ((unsigned short int)HorKolFlg && (Koll->mask & (unsigned short int)VerKolFlg))
			if ((Koll->mask & (unsigned short int)HorKolFlg))
			{
				ReturnFlg = KollissionDetect(Koll);
				if (!ReturnFlg)
					goto jU_Move;
			}

		HorKolFlg = swap( HorKolFlg );
		if ((unsigned short int)HorKolFlg)
		{
			Koll++;
			map.wKollX++;
			if ((Koll->mask & (unsigned short int)VerKolFlg))
				if ((Koll->mask & (unsigned short int)HorKolFlg))
					ReturnFlg = KollissionDetect(Koll);
		}

jU_Move:
		if (ReturnFlg)
			if (JoyDir == Direction)
			{
				if (YSpeed < MAXSPEED) YSpeed += time_elapsed * ACELFACT;
				y -= time_elapsed * YSpeed;
			}
			else
			{
				if (YSpeed > 0.0)
				{
					YSpeed -= time_elapsed * ACELFACT;
					y -= time_elapsed * YSpeed;
				}
				else
				{
					y += time_elapsed * YSpeed;
					YSpeed = 0.0;
				}
			}

		if (y < 0)
		{
			map.wKollPageDistance -= (map.wKollLine * 10);
			map.wScrYoff -= YSCROFF;
			y += YSCROFF;
		}
	}
	else if (JoyDir & DOWN)
	{
		SprListOff = A_FIGUR_D;

		Koll += map.wKollLine;
		map.wKollY++;
		VerKolFlg = swap( VerKolFlg );
		KollRichtung = DOWN;

		if ((unsigned short int)HorKolFlg && (Koll->mask & (unsigned short int)VerKolFlg))
			if ((Koll->mask & (unsigned short int)HorKolFlg))
			{
				ReturnFlg = KollissionDetect(Koll);
				if (!ReturnFlg)
					goto jD_Move;
			}

		HorKolFlg = swap( HorKolFlg );
		if ((unsigned short int)HorKolFlg)
		{
			Koll++;
			map.wKollX++;
			if (Koll->mask & (unsigned short int)VerKolFlg)
				if (Koll->mask & (unsigned short int)HorKolFlg)
					ReturnFlg = KollissionDetect(Koll);
		}

jD_Move:
		if (ReturnFlg)
			if (JoyDir == Direction)
			{
				if (YSpeed < MAXSPEED) YSpeed += time_elapsed * ACELFACT;
				y += time_elapsed * YSpeed;
			}
			else
			{
				if (YSpeed > 0.0)
				{
					YSpeed -= time_elapsed * ACELFACT;
					y += time_elapsed * YSpeed;
				}
				else
				{
					y -= time_elapsed * YSpeed;
					YSpeed = 0.0;
				}
			}

		if (y >= YSCROFF)
		{
			map.wKollPageDistance += (map.wKollLine * 10);
			map.wScrYoff += YSCROFF;
			y -= YSCROFF;
		}
	}

	SetLoop();

	if (GameState & GAME_NOLIVES)
	{
		GameState ^= GAME_NOLIVES;
		if (GameState & GAME_NEXTLEVEL)
		{
			GameState ^= GAME_NEXTLEVEL;
			return(GAME_NEXTLEVEL);
		}
		return(GAME_NOLIVES);
	}

	fps++;
	if (menu_timer->elapsed())
	{
		if (!Timeless) Levtime--;
		menu_timer->reset();
		fps_show = fps;
		fps = 0;
	}

	if (game_timer->elapsed())
	{
		game_timer->reset();

		SprAnimNum = (SprAnimNum) ? 0 : 1;
		if (!Timeless && Levtime <= 0)
		{
			*player.Lives -= 1;
			if ((*player.Lives > 0))
			{
				NewLevelStart = true;
				return(GAME_NEXTLEVEL);
			} else
				return(GAME_NOLIVES);
		}
	}

	return(GAME_ACTIVE); // alle OK
}

//	---------------------------------------------------------
//		Subroutine:   Ausschnitt restaurieren mit
//									oder ohne Animation
//	---------------------------------------------------------

void Game::SetLoop(void)
{
	map.Draw(); // Level zeichnen

	// Teile neuzeichnen bzw. animieren
	std::list<Object*>::iterator it;
	for (it = objects.begin(); it != objects.end(); it++)
	{
		Object *cur = *it;
		cur->show();
	}

	// Spielfigur zeichnen
	map.DrawFigur((int)x, (int)y, SprListOff + SprAnimNum);

	float time_elapsed2 = time_elapsed;

	while (time_elapsed2 > 0)
	{
		float turn_time = (time_elapsed2 > 0.05f) ? 0.05f : time_elapsed2;
		for (it = objects.begin(); it != objects.end(); it++)
		{
			if ((*it)->turn(turn_time) == false)
			{
				// Feind aus Liste lÃ¶schen
				if (((*it)->get_type() == OBJ_UPDOWN) || ((*it)->get_type() == OBJ_CIRCLE))
				{
					delete (*it);
					it = objects.erase(it);
				}
				// nur falls die Figur nicht gerade fÃ¤llt oder Leben bereits eine Kollision stattfindet
				if (((GameState & FIGUR_KOLLISION) == 0) && ((GameState & FIGUR_FALLING) == 0))
				{
					GameState |= FIGUR_KOLLISION;
					KillWall(NULL);
					GameState ^= FIGUR_KOLLISION;
				}
			}
		}
		time_elapsed2 -= 0.05f;
	}

	// Score etc.
	GameMenu();

}
