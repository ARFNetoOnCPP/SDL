#include <iostream>
#define SDL_MAIN_HANDLED
#include "SDL.h"

using namespace std;


int main62(int argc, char** argv)
{
	SDL_version SDL_compile_version;
	SDL_version SDL_dll_version;
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;

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

	SDL_Window* window = SDL_CreateWindow(
		"Particle Fire Explosion",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN
	);

	if (window == NULL)
	{
		SDL_Quit();
		return 2;
	}

	SDL_Event event;
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

	SDL_DestroyWindow(window);
	SDL_Quit();
	return EXIT_SUCCESS;
}	// end main()