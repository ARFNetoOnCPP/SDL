#include <iostream>
#define SDL_MAIN_HANDLED
#include "maskTest.h"
#include "Screen.h"
#include "SDL.h"
#include "swarm.h"
#include "MaskTest.h"
#include "math.h"
#include "time.h"

using namespace std;
using namespace particles;

int main(int argc, char** argv)
{
	int j = 0;
	//MaskTest mask;
	j = 0x123456FF;
	MaskTest::getRed(j);
	MaskTest::getGreen(j);
	MaskTest::getBlue(j);
	j = 0;
	if (j == 0) return 0;

	srand((unsigned int)time(NULL));

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

	Swarm swarm;

	while (!screen.processEvents())
	{
		int elapsed = SDL_GetTicks();
		screen.clear();
		swarm.update(elapsed);

		unsigned char red = (unsigned char)((1 + sin(elapsed * 0.0001)) * 128);
		unsigned char green = (unsigned char)((1 + sin(elapsed * 0.0002)) * 128);
		unsigned char blue = (unsigned char)((1 + sin(elapsed * 0.0003)) * 128);

		const Particle* const pParticles = swarm.getParticles();

		for (int i = 0; i < Swarm::NPARTICLES; i++)
		{
			Particle particle = pParticles[i];
			int x = (int)((particle.m_x + 1) * Screen::SCREEN_WIDTH / 2);
			int y = (int)(particle.m_y * Screen::SCREEN_WIDTH / 2) + Screen::SCREEN_HEIGHT / 2;
			screen.setPixel(x, y, red, green, blue);
		}	// end for
		screen.boxBlur();
		screen.update();

	}	// end while
	screen.close();
	return EXIT_SUCCESS;

}	// end main()