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

#ifndef B_UTIL_H
#define B_UTIL_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#ifndef NO_SOUND
#include <SDL_mixer.h>
#endif

#include <cstdlib>
#include <string>
using std::string;

// Debugging macros
#define SHOW_DEBUG      1

#if SHOW_DEBUG == 1
#define DEBUG(msg) printf("Balloons_Msg: %s\n", msg);
#define SHOW_VERSION() printf("Balloons_Msg: Version: %s\n", VERSION);
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
	Surface() = default;
	Surface(const char *filename);
	~Surface();

	void draw(int, int);
	void draw(SDL_Rect, SDL_Rect);
	int get_width();
	void setscreen(SDL_Surface *screen)
	{
		m_screen = screen;
	}

protected:
	SDL_Surface *srf, *m_screen;
};

class Font : public Surface
{
public:
	Font(const char *filename, int size);
	~Font();

	void draw(int x, int y, const char *text);
	int get_width(const char *);

private:
	TTF_Font *ttf_font;
	SDL_Color fgcolor;
	// SDL_Color bgcolor;
};

// DISPLAYWINDOW -------------------------------------------------

class DisplayWindow
{
public:
	DisplayWindow() = default;
	DisplayWindow(const char*, int, int, bool, bool);
	~DisplayWindow();

	inline bool is_fullscreen(){
		return(fullscreen);
	};
	void set_windowed();
	void set_fullscreen(int, int, int);
	void clear();
	void flip();

	SDL_Surface *m_screen;
	SDL_Window *m_window;
	SDL_Texture *m_texture;
	SDL_Renderer *m_renderer;

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

#endif // B_UTIL_H
