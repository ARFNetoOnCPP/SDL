#include <iostream>
#define SDL_MAIN_HANDLED
#include "Screen.h"
#include "SDL.h"
#include "maskTest.h"
#include "math.h"

using namespace std;
using namespace particles;

int main(int argc, char** argv)
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
		unsigned char red = (unsigned char)((1 + sin(elapsed * 0.0001)) * 128);
		unsigned char green = (unsigned char)((1 + sin(elapsed * 0.0002)) * 128);
		unsigned char blue = (unsigned char)((1 + sin(elapsed * 0.0003)) * 128);
		screen.fillScreen(red, green, blue);
		screen.update();

	}	// end while
	screen.close();
	return EXIT_SUCCESS;

}	// end main()