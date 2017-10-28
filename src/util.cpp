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
	SDL_Surface *temp;

	temp = IMG_Load(filename);
	if (!temp)
		printf("sdl_Msg: Can't load '%s'\n", filename);
	else
		printf("sdl_Msg: Loaded image '%s'\n", filename);

	screen = SDL_DisplayFormat(temp);

	SDL_FreeSurface(temp);
	m_screen = screen;
}

Surface::Surface(const char *filename, int alpha)
{
	SDL_Surface *temp;

	temp = IMG_Load(filename);
	if (!temp)
		printf("sdl_Msg: Can't load '%s'\n", filename);
	else
		printf("sdl_Msg: Loaded image '%s'\n", filename);

/*
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY | SDL_RLEACCEL,
	SDL_MapRGB(temp->format, 255,255,255));
	SDL_SetAlpha (temp, SDL_SRCALPHA | SDL_RLEACCEL, alpha);
*/
	SDL_SetAlpha(temp, SDL_SRCALPHA, alpha);
	SDL_SetColorKey(temp, SDL_SRCCOLORKEY, SDL_MapRGB(temp->format, 100, 100, 50));
	screen = SDL_DisplayFormatAlpha(temp);

	SDL_FreeSurface(temp);
	m_screen = screen;
}

int Surface::get_width()
{
	return(screen->w);
}

void Surface::draw(int x, int y)
{
	SDL_Rect src, dest;

	src.x = 0;
	src.y = 0;
	dest.x = x;
	dest.y = y;
	src.w = dest.w = screen->w;
	src.h = dest.h = screen->h;

	SDL_BlitSurface(screen, &src, m_screen, &dest);
}

void Surface::draw(SDL_Rect src, SDL_Rect dest)
{
	SDL_BlitSurface(screen, &src, m_screen, &dest);
}

void Surface::set_alpha(int alpha)
{
/*
	SDL_SetColorKey( screen, SDL_SRCCOLORKEY, 255);
	SDL_SetColorKey(screen, SDL_SRCCOLORKEY, SDL_MapRGB(screen->format, 255, 0, 0));
	SDL_SetAlpha(screen, SDL_SRCALPHA | SDL_RLEACCEL, trans);
*/
	SDL_SetAlpha(screen, SDL_SRCALPHA, alpha);
}

// FONT -------------------------------------------------

Font::Font(const char *filename, int size)
{
	ttf_font = TTF_OpenFont( filename, size);
	fgcolor.r = fgcolor.g = fgcolor.b = 0xff; // font white
//	bgcolor.r = bgcolor.g = bgcolor.b = 0x00; // background black
}

Font::~Font()
{
	TTF_CloseFont( ttf_font );
}

void Font::draw(int x, int y, const char *text)
{
	SDL_Rect src, dest;

	screen = TTF_RenderUTF8_Blended(ttf_font, text, fgcolor);
/*
	screen = TTF_RenderText_Blended(ttf_font, text, fgcolor);
	screen = TTF_RenderText_Shaded(ttf_font, text, fgcolor, bgcolor);
	SDL_SetColorKey(screen, SDL_SRCCOLORKEY,0);
	screen = TTF_RenderText_Solid(ttf_font, text, fgcolor);
*/
	if (x <= 0 | x >= 640)
		x = (640 - screen->w) / 2; //temp zur sicherheit

	src.x = 0;
	src.y = 0;
	dest.x = x;
	dest.y = y;
	src.w = dest.w = screen->w;
	src.h = dest.h = screen->h;

	SDL_BlitSurface(screen, &src, m_screen, &dest);
	SDL_FreeSurface(screen);
}

int Font::get_width(const char *text)
{
	int w;
	TTF_SizeText(ttf_font, text, &w, NULL);
	return(w);
}

// DISPLAY -------------------------------------------------

Display::Display()
{
	VideoInfo = SDL_GetVideoInfo();
	VideoMem = VideoInfo->video_mem;
	std::cout << "Die Grafikkarte besitzt einen Videospeicher von " << VideoMem << " kB" << std::endl;

	hw_available = VideoInfo->hw_available;
	if (!hw_available)
	{
		std::cerr << "SDL_HWSURFACE wird nicht unterstützt" << std::endl;
	}

	bpp = VideoInfo->vfmt->BitsPerPixel;
	std::cout << "Die beste Farbtiefe ist " << bpp << " Bit" << std::endl;
};

// DISPLAYWINDOW -------------------------------------------------

DisplayWindow::DisplayWindow(){
};

DisplayWindow::DisplayWindow(const char *title, int width, int height, bool fullscreen, bool rezise)
{
	int result;
//	SDL_Surface *temp;

	DEBUG("Init the SDL...");
	result = SDL_Init(SDL_INIT_VIDEO);
	if (result == -1)
	{
//		std::cerr << "Kann SDL nicht initialisieren!!!!!" << SDL_GetError() << std::endl;
		DEBUG("Failed: Initing SDL...");
		return;
	}
	m_screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);
	fullscreen = false;

	SDL_WM_SetCaption( title, NULL );
/*
	temp = IMG_Load(image_files[ICON]);
	if( !temp )
		printf("sdl_Msg: Can't load '%s'\n", image_files[ICON]);
	else
		printf("sdl_Msg: Loaded image '%s'\n", image_files[ICON]);
	SDL_WM_SetIcon(temp, NULL);
	delete temp;
*/
	// Init ttf
	TTF_Init();

#ifndef NO_SOUND
	result = SDL_InitSubSystem(SDL_INIT_AUDIO);
	if (result == -1)
	{
		DEBUG("Failed: Initing SDL_Sound...");
		return;
	}
	result = Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
	if (result == -1)
	{
		DEBUG("Failed: Initing Sound...");
		return;
	}
#endif
//	SDL_ShowCursor(SDL_FALSE);
}

DisplayWindow::~DisplayWindow()
{
#ifndef NO_SOUND
	// Close audio
	Mix_CloseAudio();
#endif
	TTF_Quit();
//	SDL_ShowCursor(SDL_TRUE);
	SDL_Quit();
}

void DisplayWindow::set_fullscreen(int width, int height, int bpp)
{
	fullscreen = true;
	m_screen = SDL_SetVideoMode(640, 416, 32, SDL_SWSURFACE | SDL_FULLSCREEN);
}

void DisplayWindow::set_windowed()
{
	fullscreen = false;
	m_screen = SDL_SetVideoMode(640, 416, 32, SDL_SWSURFACE);
	SDL_Flip(m_screen);
}

void DisplayWindow::clear()
{
	SDL_FillRect(m_screen, NULL, 0);
}

SoundBuffer::SoundBuffer(const char* src)
{
	sample = Mix_LoadWAV(src);
	if (!sample)
		printf("Mix_LoadWAV: failed %s %s\n", Mix_GetError(), src);
	// handle error
}

SoundBuffer::~SoundBuffer()
{
	delete sample;
	Mix_CloseAudio();
}

void SoundBuffer::play()
{
#ifndef NO_SOUND
	Mix_PlayChannel(-1, sample, 0);
#endif
}
void SoundBuffer::set_volume(float vol)
{
	sample->volume = (Uint8)(128.0 * vol);
}
