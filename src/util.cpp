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

#include "util.h"
#include "main.h"

Surface::Surface(const char *filename)
{
	srf = IMG_Load(filename);
	if (!srf)
		printf("sdl_Msg: Can't load '%s'\n", filename);
	else
		printf("sdl_Msg: Loaded image '%s'\n", filename);

	m_screen = NULL;
}

Surface::~Surface()
{
	if(srf)
		SDL_FreeSurface(srf);
}

int Surface::get_width()
{
	if(srf)
		return(srf->w);

	return(0);
}

void Surface::draw(int x, int y)
{
	SDL_Rect src, dest;

	src.x = 0;
	src.y = 0;
	dest.x = x;
	dest.y = y;
	src.w = dest.w = srf->w;
	src.h = dest.h = srf->h;

	SDL_BlitSurface(srf, &src, m_screen, &dest);
}

void Surface::draw(SDL_Rect src, SDL_Rect dest)
{
	SDL_BlitSurface(srf, &src, m_screen, &dest);
}

// FONT -------------------------------------------------

Font::Font(const char *filename, int size)
{
	ttf_font = TTF_OpenFont(filename, size);
	fgcolor.r = fgcolor.g = fgcolor.b = 0xff; // font white
//	bgcolor.r = bgcolor.g = bgcolor.b = 0x00; // background black

	m_screen = NULL;
	srf = NULL;
}

Font::~Font()
{
	TTF_CloseFont(ttf_font);
	srf = NULL;
}

void Font::draw(int x, int y, const char *text)
{
	SDL_Rect src, dest;

	srf = TTF_RenderUTF8_Blended(ttf_font, text, fgcolor);
	if(!srf)
	{
		DEBUG("Could not render text");
		return;
	}
	if (x <= 0 | x >= 640)
		x = (640 - srf->w) / 2; //temp zur sicherheit

	src.x = 0;
	src.y = 0;
	dest.x = x;
	dest.y = y;
	src.w = dest.w = srf->w;
	src.h = dest.h = srf->h;

	SDL_BlitSurface(srf, &src, m_screen, &dest);
	SDL_FreeSurface(srf);
}

int Font::get_width(const char *text)
{
	int w;
	if(TTF_SizeUTF8(ttf_font, text, &w, NULL) == 0)
		return(w);

	return(0);
}

// DISPLAYWINDOW -------------------------------------------------

DisplayWindow::DisplayWindow(const char *title, int width, int height, bool fullscreen, bool rezise)
{
	int result;
	SDL_Surface *icon;

	DEBUG("Init the SDL...");
	result = SDL_Init(SDL_INIT_VIDEO);
	if (result == -1)
	{
//		std::cerr << "Kann SDL nicht initialisieren!!!!!" << SDL_GetError() << std::endl;
		DEBUG("Failed: Initing SDL...");
		return;
	}
	m_screen = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_BGRA32);
	fullscreen = false;

	m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_RESIZABLE);
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_TARGETTEXTURE);
	SDL_RenderSetLogicalSize(m_renderer, width, height);
	m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_BGRA32,
		SDL_TEXTUREACCESS_STREAMING, width, height);

	SDL_SetWindowTitle(m_window, title);

	icon = IMG_Load(image_files[ICON]);
	if(icon) {
		SDL_SetWindowIcon(m_window, icon);
		SDL_FreeSurface(icon);
	}

	// Init ttf
	TTF_Init();

#ifndef NO_SOUND
	result = SDL_InitSubSystem(SDL_INIT_AUDIO);
	if (result != -1)
	{
		result = Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
	}
	if (result == -1)
	{
		DEBUG("Failed: Initing SDL_Sound...");
	}
#endif
	SDL_ShowCursor(SDL_FALSE);
}

DisplayWindow::~DisplayWindow()
{
#ifndef NO_SOUND
	// Close audio
	Mix_CloseAudio();
#endif
	TTF_Quit();
	SDL_ShowCursor(SDL_TRUE);
	SDL_FreeSurface(m_screen);
	SDL_DestroyTexture(m_texture);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void DisplayWindow::set_fullscreen(int width, int height, int bpp)
{
	fullscreen = true;
	SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void DisplayWindow::set_windowed()
{
	fullscreen = false;
	SDL_SetWindowFullscreen(m_window, 0);
}

void DisplayWindow::clear()
{
	SDL_FillRect(m_screen, NULL, 0);
}

void DisplayWindow::flip()
{
	SDL_RenderClear(m_renderer);
	SDL_UpdateTexture(m_texture, NULL, m_screen->pixels, m_screen->pitch);
	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
	SDL_RenderPresent(m_renderer);
}

SoundBuffer::SoundBuffer(const char* src)
{
	sample = Mix_LoadWAV(src);
	if (!sample)
		printf("Mix_LoadWAV: failed %s %s\n", Mix_GetError(), src);
}

SoundBuffer::~SoundBuffer()
{
	if(sample)
		Mix_FreeChunk(sample);
}

void SoundBuffer::play()
{
#ifndef NO_SOUND
	if(sample)
		Mix_PlayChannel(-1, sample, 0);
#endif
}
void SoundBuffer::set_volume(float vol)
{
	if(sample)
		sample->volume = (Uint8)(128.0 * vol);
}
