#pragma once
#include <iostream>
#include "SDL.h"

namespace particles
{
	class Screen
	{
	public:
		const static int SCREEN_WIDTH = 800;
		const static int SCREEN_HEIGHT = 600;

	private:
		SDL_version		SDL_compile_version;
		SDL_version		SDL_dll_version;

		SDL_Window* m_window;
		SDL_Renderer* m_renderer;
		SDL_Texture* m_texture;
		Uint32* m_buffer;

	public:
		Screen();

		void close();
		void fillScreen(Uint8 red, Uint8 green, Uint8 blue);
		bool init();
		bool processEvents();
		void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);

	};	// end class Screen

}	// end namespace particles

