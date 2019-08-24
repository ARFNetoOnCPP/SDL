#pragma once
#include <iostream>
#include "SDL.h"

namespace particles
{
	class Screen
	{
	public:
		int SCREEN_WIDTH;
		int SCREEN_HEIGHT;
		int SCREEN_HALF_WIDTH;
		int SCREEN_HALF_HEIGHT;

	private:

		const Uint32 MaxBlurCycles = 500;
		bool timerON = true;

		SDL_version		SDL_compile_version;
		SDL_version		SDL_dll_version;

		SDL_Window* m_window;
		SDL_Renderer* m_renderer;
		SDL_Texture* m_texture;
		Uint32* m_buffer1;
		Uint32* m_buffer2;

		Uint32	blurTime;
		Uint32	minBlurTime;
		Uint32	maxBlurTime;
		Uint32  blurCycles;

		double  blurAverageTime;

	public:
		Screen(int, int);
		~Screen();

		void boxBlur();
		void boxBlurJ();
		void clear();
		void close();
		void fillScreen(Uint8 red, Uint8 green, Uint8 blue);
		bool init();
		bool processEvents();
		void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
		void update();

	};	// end class Screen

}	// end namespace particles

