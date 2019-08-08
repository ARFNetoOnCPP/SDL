#include <iostream>
#define SDL_MAIN_HANDLED
#include "SDL.h"

using namespace std;


int main(int argc, char** argv)
{
	SDL_version SDL_compile_version;
	SDL_version SDL_dll_version;
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
		return EXIT_FAILURE;
	}
	else
	{
		cout << "SDL Ready!" << SDL_GetError() << endl;
	}
	SDL_Quit();
	return EXIT_SUCCESS;
}	// end main()