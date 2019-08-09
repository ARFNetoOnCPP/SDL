#include <iostream>
#define SDL_MAIN_HANDLED
#include "SDL.h"

using namespace std;

SDL_version SDL_compile_version;
SDL_version SDL_dll_version;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


int main63(int argc, char** argv)
{

	SDL_GetVersion(&SDL_dll_version);
	printf("SDL runtime %d.%d.%d\n",
		SDL_dll_version.major,
		SDL_dll_version.minor,
		SDL_dll_version.patch);

	SDL_VERSION(&SDL_compile_version);
	printf("SDL source %d.%d.%d\n",
		SDL_compile_version.major,
		SDL_compile_version.minor,
		SDL_compile_version.patch);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL Error: " << SDL_GetError() << endl;
		return 1;
	}
	else
	{
		cout << "SDL Ready!" << SDL_GetError() << endl;
	}

	SDL_Window* m_window = SDL_CreateWindow(
		"Particle Fire Explosion",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	if (m_window == NULL)
	{
		SDL_Quit();
		return 2;
	}

	SDL_Event event;

	SDL_Renderer* m_renderer = 
		SDL_CreateRenderer( m_window, -1, SDL_RENDERER_PRESENTVSYNC );
	if (m_renderer == NULL)
	{
		cout << "Could not create renderer" << endl;
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return 3;
	}	// end if

	SDL_Texture* m_texture =
		SDL_CreateTexture(
			m_renderer,
			SDL_PIXELFORMAT_RGBA8888, 
			SDL_TEXTUREACCESS_STATIC,
			SCREEN_WIDTH,
			SCREEN_HEIGHT
		);
	if (m_texture == NULL)
	{
		cout << "Could not create texture" << endl;
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return 4;
	}	// end if

	Uint32* m_buffer = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
	memset(m_buffer, 0xFF, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
	SDL_UpdateTexture(m_texture, NULL, m_buffer, SCREEN_WIDTH * sizeof(Uint32));
	SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
	SDL_RenderPresent(m_renderer);

	bool quit = false;

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quit = true;
			}	// end if
		}	// end while
	}	// end while

	delete[] m_buffer;
	SDL_DestroyTexture(m_texture);
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
	return EXIT_SUCCESS;
}	// end main()