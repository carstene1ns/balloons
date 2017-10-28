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

#ifndef UTILITIESUTIL_H
#define UTILITIESUTIL_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#ifndef NO_SOUND
#include <SDL/SDL_mixer.h>
#endif

using namespace std;
#include <cstdlib>
#include <string>

// Debugging macros
// use gcc -D ... to change these values without touching the sources
#define SHOW_DEBUG      1

#if SHOW_DEBUG == 1
#define DEBUG(msg) printf("Balloons_Msg: "); printf(msg); printf("\n");
#define SHOW_VERSION() printf("Balloons_Msg: "); \
	printf("Version: %s", VERSION); \
	printf("\n");
#else
    #define DEBUG(msg)
    #define SHOW_VERSION()
#endif

/**
   tools needed by the game like timers, etc.

        @author R & M Lagarden <roland@lagarden.de oder m.l.k@freenet.de >
 */

class Surface
{
public:
	Surface(){
	};
	Surface(const char *filename);
	Surface(const char *filename, int alpha);
	~Surface(){
	};

	void draw(int, int);
	void draw(SDL_Rect, SDL_Rect);
	int get_width();
	void setscreen(SDL_Surface *screen)
	{
		m_screen = screen;
	}
	void set_alpha(int);

private:
	SDL_Surface *screen, *m_screen;
};

class Font : public Surface
{
public:
	Font(const char *filename, int size);
	~Font();

	void draw(int x, int y, const char *text);
	int get_width(const char *);
	void setscreen(SDL_Surface *screen)
	{
		m_screen = screen;
	}

private:
	SDL_Surface *screen, *m_screen;
	TTF_Font *ttf_font;
	SDL_Color fgcolor;
	// SDL_Color bgcolor;
};

// DISPLAY -------------------------------------------------

class Display
{
public:
	Display();
	~Display(){
	};
private:
	const SDL_VideoInfo *VideoInfo;
	Uint32 VideoMem; // Size of (available) video memory
	Uint32 hw_available; // indicated support of SDL_HWSURFACE
	Uint8 bpp; // the best pixel format (colour depth)
};

// DISPLAYWINDOW -------------------------------------------------

class DisplayWindow
{
public:
	DisplayWindow();
	DisplayWindow(const char*, int, int, bool, bool);
	~DisplayWindow();

	bool is_fullscreen(){
		return(fullscreen);
	};
	void set_windowed();
	void set_fullscreen(int, int, int);
	void clear();
	void flip()
	{
		SDL_Flip( m_screen );
	}

	SDL_Surface *m_screen;

private:
	bool fullscreen;
};



class Timer
{
public:
	Timer(Uint32 milliseconds){
		start_time = SDL_GetTicks();
		intervall = milliseconds;
	};
	bool elapsed(){
		if (SDL_GetTicks() - start_time > intervall)
			return true;
		else
			return false;
	};
	Uint32 time_passed()
	{
		return(SDL_GetTicks() - start_time);
	};
	void set(Uint32 milliseconds){
		intervall = milliseconds;
	};
	void reset(){
		start_time = SDL_GetTicks();
	};         //
	~Timer(){
	};
	Uint32 intervall; // Milliseconds to go
	Uint32 start_time;
};


class SoundBuffer
{
public:
	SoundBuffer(const char*);
	~SoundBuffer();

	virtual void play();
	void set_volume(float);

private:
	Mix_Chunk *sample;
};

#endif
