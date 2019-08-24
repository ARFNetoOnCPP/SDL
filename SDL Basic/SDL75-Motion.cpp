#include <iostream>

#define SDL_MAIN_HANDLED
#define DEFAULT_N_PARTICLES		5000
#define DEFAULT_SCREEN_WIDTH	800
#define DEFAULT_SCREEN_HEIGHT	600

#include "maskTest.h"
#include "Screen.h"
#include "SDL.h"
#include "swarm.h"
#include "MaskTest.h"
#include "math.h"
#include "time.h"

using namespace std;
using namespace particles;



//
// particles [nparticles screen_width screen heigth]
//     defaults to 5000 particles and a 800x600 screen
//
int main(int argc, char** argv)
{
	unsigned int v_nParticles = DEFAULT_N_PARTICLES;
	unsigned int v_Screen_Width = DEFAULT_SCREEN_WIDTH;
	unsigned int v_Screen_Height = DEFAULT_SCREEN_HEIGHT;
	//========== command line arguments
	if (argc > 1)
	{
		v_nParticles = atoi(argv[1]);
		if (argc > 2)
		{
			v_Screen_Width = atoi(argv[2]);
			if (argc > 3)
			{
				v_Screen_Height = atoi(argv[3]);
			}	// end if
		}	// end if
	}
	else
	{
		cout << "Using default values" << endl;
	}	// end if
	cout << 
		"Explosion of " <<
		v_nParticles << " particles in a (" <<
		v_Screen_Width << "," <<
		v_Screen_Height << ") screen" <<
		endl;
	//==========	
	srand((unsigned int)time(NULL));
	Screen screen(v_Screen_Width, v_Screen_Height);

	if (screen.init() == false)
	{
		cout << "Error at init()" << endl;
		return EXIT_FAILURE;
	}
	else
	{
		cout << "init() ok" << endl;
	}

	Swarm swarm(v_nParticles);;

	while (!screen.processEvents())
	{
		int elapsed = SDL_GetTicks();
		swarm.update(elapsed);

		unsigned char red =   (unsigned char)((1 + sin(elapsed * 0.0001)) * 128);
		unsigned char green = (unsigned char)((1 + sin(elapsed * 0.0002)) * 128);
		unsigned char blue =  (unsigned char)((1 + sin(elapsed * 0.0003)) * 128);

		const Particle* const pParticles = swarm.getParticles();

		for (int i = 0; i < swarm.NPARTICLES; i++)
		{
			Particle particle = pParticles[i];
			int x = (int)((particle.m_x + 1) * screen.SCREEN_HALF_WIDTH);
			int y = (int)(particle.m_y * screen.SCREEN_HALF_WIDTH) + screen.SCREEN_HALF_HEIGHT;
			screen.setPixel(x, y, red, green, blue);
		}	// end for
		screen.boxBlur();
		screen.update();

	}	// end while
	screen.close();
	return EXIT_SUCCESS;

}	// end main()