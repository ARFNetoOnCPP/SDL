#include <iostream>
#define SDL_MAIN_HANDLED
#include "Screen.h"
#include "SDL.h"
#include "maskTest.h"
#include "math.h"

using namespace std;
using namespace particles;

int main65(int argc, char** argv)
{
	Screen screen;

	if (screen.init() == false)
	{
		cout << "Error at init()" << endl;
		return EXIT_FAILURE;
	}
	else
	{
		cout << "init() ok" << endl;
	}

	while (!screen.processEvents())
	{
		int elapsed = SDL_GetTicks();
		unsigned char green = (unsigned char)((1 + sin(elapsed * 0.001)) * 128);
		screen.fillScreen(0, green, 0);
		screen.update();
	}	// end while

	screen.close();
	return EXIT_SUCCESS;

}	// end main()