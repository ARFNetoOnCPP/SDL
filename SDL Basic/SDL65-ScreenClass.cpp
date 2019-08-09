#include <iostream>
#define SDL_MAIN_HANDLED
#include "SDL.h"
#include "Screen.h"
#include "maskTest.h"

using namespace std;
using namespace particles;

int main(int argc, char** argv)
{
	Screen screen;
	maskTest mask;	// mask test lesson 66
	bool justTest = false;

	mask.maskSet(0x12, 0x34, 0x56);


	if (screen.init() == false)
	{
		cout << "Error at init()" << endl;
		return EXIT_FAILURE;
	}
	else
	{
		cout << "init() ok" << endl;
	}
	if (justTest)
	{
		screen.close();
		return 0;
	}

	while (!screen.processEvents()){}
	screen.close();
	return EXIT_SUCCESS;

}	// end main()