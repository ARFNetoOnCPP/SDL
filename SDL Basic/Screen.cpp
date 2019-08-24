#include "Screen.h"
#include <iostream>
namespace particles
{
	Screen::Screen(int width, int height):
		m_window(NULL),
		m_renderer(NULL),
		m_texture(NULL)
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

		SCREEN_WIDTH = width;
		SCREEN_HEIGHT = height;
		SCREEN_HALF_WIDTH = SCREEN_WIDTH / 2;
		SCREEN_HALF_HEIGHT = SCREEN_HEIGHT / 2;

		m_buffer1 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
		m_buffer2 = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
		memset(m_buffer1, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
		memset(m_buffer2, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
		minBlurTime = INT_MAX;
		maxBlurTime = 0;

	}	// end Screen()

	Screen::~Screen()
	{
		std::cout << 
			"boxBlur() time min: " <<
			minBlurTime << 
			"ms max: " <<
			maxBlurTime <<
			"ms" <<
			std::endl;;
		// average blur time
		std::cout <<
			"Average blur time for " <<
			SCREEN_HEIGHT * SCREEN_WIDTH <<
			" particles and " <<
			blurCycles <<
			" cycles was " <<
			(blurAverageTime / blurCycles) << " ms" <<
			std::endl;
	}	// end ~Screen()

		/*
				 1 2 3
				 4 5 6
				 7 8 9

				 this is the mapping from [x,y] to buffer
				(x,y) = y*SCREEN_WIDTH + x

				blur for the pixels that are in a "box"

					3 are at the row above pixel (x,y)

				 y*(SCREEN_WIDTH-1) + (x-1)
				 y*(SCREEN_WIDTH-1) + (x)
				 y*(SCREEN_WIDTH-1) + (x+1)

					3 are at the same line

				 y*(SCREEN_WIDTH) + (x-1)
				 y*(SCREEN_WIDTH) + (x)		// this is the central one at (x,y)
				 y*(SCREEN_WIDTH) + (x+1)

					3 are at the row above pixel (x,y)

				 y*(SCREEN_WIDTH+1) + (x-1)
				 y*(SCREEN_WIDTH+1) + (x)
				 y*(SCREEN_WIDTH+1) + (x+1)

				 [x,y] is the position 5, at the center of the box, and this is why
					   we call this BoxBlur algorithm

					NOTE that baseY points to the starting pixel of the row above 5
						 so we need not to multiply or calculate this over and over

		*/

	void Screen::boxBlur()
	{
		// swap
		Uint32* temp = m_buffer1;
		m_buffer1 = m_buffer2;
		m_buffer2 = temp;

		// start timer
		Uint32 blurTime = SDL_GetTicks();
		//
		// code for the inner pixels, that are inside a 9px box
		// 
		// 1 2 3
		// 4 5 6
		// 7 8 9
		//
		Uint32 baseY;
		Uint32 l;	// to point inside buffer
		Uint32 r;
		Uint32 z;
		Uint32 t;
		Uint32 red;
		Uint32 green;
		Uint32 blue;

		int test = 1;

		baseY = 0;
		for (int y = 0; y < SCREEN_HEIGHT - 2; y++)
		{
			for (int x = 0; x < SCREEN_WIDTH - 2; x++)
			{
				l = baseY + x;	// left pixel on this row: 1
				z = l + 1;
				t = l + 2;
				// 1 2 3
				red =  (Uint8)(m_buffer2[l] >> 24) + (Uint8)(m_buffer2[z] >> 24) + (Uint8)(m_buffer2[t] >> 24);
				green =(Uint8)(m_buffer2[l] >> 16) + (Uint8)(m_buffer2[z] >> 16) + (Uint8)(m_buffer2[t] >> 16);
				blue = (Uint8)(m_buffer2[l] >>  8) + (Uint8)(m_buffer2[z] >>  8) + (Uint8)(m_buffer2[t] >> 8);
				// 4 5 6
				l = l + SCREEN_WIDTH;		// now we are at pixel 4
				z = l + 1;
				t = l + 2;
				red += (Uint8)(m_buffer2[l] >> 24) + (Uint8)(m_buffer2[z] >> 24) + (Uint8)(m_buffer2[t] >> 24);
				green+=(Uint8)(m_buffer2[l] >> 16) + (Uint8)(m_buffer2[z] >> 16) + (Uint8)(m_buffer2[t] >> 16);
				blue +=(Uint8)(m_buffer2[l] >>  8) + (Uint8)(m_buffer2[z] >>  8) + (Uint8)(m_buffer2[t] >>  8);
				// 7 8 9
				l = l + SCREEN_WIDTH;
				z = l + 1;
				t = l + 2;
				red += (Uint8)(m_buffer2[l] >> 24) + (Uint8)(m_buffer2[z] >> 24) + (Uint8)(m_buffer2[t] >> 24);
				green += (Uint8)(m_buffer2[l] >> 16) + (Uint8)(m_buffer2[z] >> 16) + (Uint8)(m_buffer2[t] >> 16);
				blue += (Uint8)(m_buffer2[l] >> 8) + (Uint8)(m_buffer2[z] >> 8) + (Uint8)(m_buffer2[t] >> 8);
				// if we did it ok, just divide by 9 to get the average color for the pixel 5 (x+1,y+1)
				setPixel((x + 1), (y + 1), (Uint8)(red / 9), (Uint8)(green / 9), (Uint8)(blue / 9));
			}	// end for x
			baseY += SCREEN_WIDTH;	// shift to next row
		}	// end for y

		//
		//	code for the outer pixels:
		//
		// we have four cases:
		//
		//  - the 4 corners, pixels with only 3 neighbors
		//	- the first and last rows, 2X3 boxes
		//  0 0 0          1 2 3
		//  4 5 6          4 5 6
		//  7 8 9          0 0 0
		//	- the first and last columns, 3x2 boxes
		//  0 2 3          1 2 0
		//  0 5 6          4 5 0
		//  0 8 9          7 8 9
		//
		//===================================================================
		// 
		// first row: only 6 px to blur. (x,0)
		//
		for (int x=0; x<(SCREEN_WIDTH-2); x++)
		{
			// no pixels above: start at 4
			red =
				(Uint8)(m_buffer2[x] >> 24) +
				(Uint8)(m_buffer2[x + 1] >> 24) +
				(Uint8)(m_buffer2[x + 2] >> 24);		// 4 5 6
			green =
				(Uint8)(m_buffer2[x] >> 16) +
				(Uint8)(m_buffer2[x + 1] >> 16) +
				(Uint8)(m_buffer2[x + 2] >> 16);
			blue =
				(Uint8)(m_buffer2[x] >> 8) +
				(Uint8)(m_buffer2[x + 1] >> 8) +
				(Uint8)(m_buffer2[x + 2] >> 8);
			// now add the values from the second line, 1
			red +=
				(Uint8)(m_buffer2[x+SCREEN_WIDTH] >> 24) +
				(Uint8)(m_buffer2[x+SCREEN_WIDTH + 1] >> 24) +
				(Uint8)(m_buffer2[x+SCREEN_WIDTH + 2] >> 24);	// 7 8 9
			green +=
				(Uint8)(m_buffer2[x+SCREEN_WIDTH] >> 16) +
				(Uint8)(m_buffer2[x+SCREEN_WIDTH + 1] >> 16) + (Uint8)
				(m_buffer2[x+SCREEN_WIDTH + 2] >> 16);
			blue += 
				(Uint8)(m_buffer2[x+SCREEN_WIDTH] >> 8) +
				(Uint8)(m_buffer2[x+SCREEN_WIDTH + 1] >> 8) +
				(Uint8)(m_buffer2[x+SCREEN_WIDTH + 2] >> 8);
			// average for 6px only, we are the border
			setPixel((x+1), 0, (Uint8)(red / 9), (Uint8)(green / 9), (Uint8)(blue / 9));
		}	// end for x
		//
		// last row: ( x,(SCREEN_WIDTH-1) )
		//
		//	1 2 3
		//  4 5 6
		//  0 0 0
		//
		// only 6 px to blur. ( x,(SCREEN_WIDTH-1) )
		//
		r = SCREEN_WIDTH * (SCREEN_HEIGHT - 2);	// start of the row before last one
		z = r + 1;
		t = r + 2;
		for (Uint32 x = 0; x < (Uint32)(SCREEN_WIDTH - 2); x++)
		{
			// 1 2 3
			red =	(Uint8)(m_buffer2[x+r] >> 24) + (Uint8)(m_buffer2[x+z] >> 24) + (Uint8)(m_buffer2[x+t] >> 24);
			green =	(Uint8)(m_buffer2[x+r] >> 16) + (Uint8)(m_buffer2[x+z] >> 16) +	(Uint8)(m_buffer2[x+t] >> 16);
			blue =  (Uint8)(m_buffer2[x+r] >>  8) +	(Uint8)(m_buffer2[x+z] >>  8) + (Uint8)(m_buffer2[x+t] >>  8);
			// shift to the next line for 4 5 6
			r = r + SCREEN_WIDTH;
			z = r + 1;
			t = r + 2;
			red +=  (Uint8)(m_buffer2[r+x] >> 24) + (Uint8)(m_buffer2[z+x] >> 24) + (Uint8)(m_buffer2[t+x] >> 24);
			green +=(Uint8)(m_buffer2[r+x] >> 16) + (Uint8)(m_buffer2[x+z] >> 16) + (Uint8)(m_buffer2[x+t] >> 16);
			blue +=	(Uint8)(m_buffer2[r+x] >>  8) +	(Uint8)(m_buffer2[x+z] >>  8) +	(Uint8)(m_buffer2[x+t] >>  8);
			// average for 6px, we are the border
			setPixel((x+1), (SCREEN_HEIGHT-1), (Uint8)(red / 9), (Uint8)(green / 9), (Uint8)(blue / 9));
			r = r - SCREEN_WIDTH;
			z = r + 1;
			t = r + 2;

		}	// end for (x)
		// 
		// first column: (0,y)
		//
		//	0 2 3
		//  0 5 6
		//  0 8 9
		//
		// only 6 px to blur. (0,y)
		//
		for (int y=0; y<(SCREEN_HEIGHT-2); y++)
		{
			z = y + 1;	// next pixel: 3
			red =	(Uint8)(m_buffer2[y] >> 24) +	(Uint8)(m_buffer2[z] >> 24);	
			green =	(Uint8)(m_buffer2[y] >> 16) +	(Uint8)(m_buffer2[z] >> 16);
			blue =	(Uint8)(m_buffer2[y] >> 8)  +	(Uint8)(m_buffer2[z] >> 8);	

			l = y + SCREEN_WIDTH;		// now we are at pixel 5
			z = l + 1;
			red +=	(Uint8)(m_buffer2[l] >> 24) +	(Uint8)(m_buffer2[z] >> 24);
			green +=(Uint8)(m_buffer2[l] >> 16) +	(Uint8)(m_buffer2[z] >> 16);
			blue +=	(Uint8)(m_buffer2[l] >> 8)  +	(Uint8)(m_buffer2[z] >> 8);

			l = l + SCREEN_WIDTH;		// now we are at pixel 8
			z = l + 1;
			red +=	(Uint8)(m_buffer2[l] >> 24) +	(Uint8)(m_buffer2[z] >> 24);
			green +=(Uint8)(m_buffer2[l] >> 16) +	(Uint8)(m_buffer2[z] >> 16);
			blue +=	(Uint8)(m_buffer2[l] >> 8)  +	(Uint8)(m_buffer2[z] >> 8);

			setPixel(0, (y+1), (Uint8)(red / 9), (Uint8)(green / 9), (Uint8)(blue / 9));
		}	// end for y

		// 
		// last column: ((SCREEN_WIDTH-1),y)
		//
		// only 6 px to blur. 
		//
		//	1 2 0
		//  4 5 0
		//  7 8 0
		//
		Uint32 y = (SCREEN_WIDTH - 2);	// top left of box
		// start at (0, SCREEN_WIDTH-2)
		// loop thru n points
		for ( int n=0; n<(SCREEN_HEIGHT-2); n++)
		{
			// 1 2
			z = y + 1;
			red =		(Uint8)(m_buffer2[y] >> 24) +	(Uint8)(m_buffer2[z] >> 24);	
			green =		(Uint8)(m_buffer2[y] >> 16) +	(Uint8)(m_buffer2[z] >> 16);
			blue =		(Uint8)(m_buffer2[y] >> 8)  +	(Uint8)(m_buffer2[z] >> 8);
			// 4 5
			y = y + SCREEN_WIDTH;
			z = y + 1;
			red+=		(Uint8)(m_buffer2[y] >> 24) + (Uint8)(m_buffer2[z] >> 24);
			green+=		(Uint8)(m_buffer2[y] >> 16) + (Uint8)(m_buffer2[z] >> 16);
			blue+=		(Uint8)(m_buffer2[y] >> 8)  + (Uint8)(m_buffer2[z] >> 8);
			// 7 8
			y = y + SCREEN_WIDTH;
			z = y + 1;
			red+=		(Uint8)(m_buffer2[y] >> 24) + (Uint8)(m_buffer2[z] >> 24);
			green+=		(Uint8)(m_buffer2[y] >> 16) + (Uint8)(m_buffer2[z] >> 16);
			blue+=		(Uint8)(m_buffer2[y] >> 8)  + (Uint8)(m_buffer2[z] >> 8);
			// rool back one line, we are at the 7, center is at 5, we are in a loop
			y = y - SCREEN_WIDTH;
			setPixel((SCREEN_WIDTH-1), (n + 1), (Uint8)(red / 9), (Uint8)(green / 9), (Uint8)(blue / 9));
		}	// end for y

		//
		//===================================================================
		//	code for the corners: 4 boxes of 4 pixels
		//===================================================================
		// Top Left (0,0)
		l = 0;	// top left pixel at the corner
		red =	(Uint8)(m_buffer2[0] >> 24) +	(Uint8)(m_buffer2[1] >> 24);
		green =	(Uint8)(m_buffer2[0] >> 16) +	(Uint8)(m_buffer2[1] >> 16);
		blue =	(Uint8)(m_buffer2[0] >> 8) +	(Uint8)(m_buffer2[1] >> 8);
		l = SCREEN_WIDTH;
		red +=	(Uint8)(m_buffer2[l] >> 24) +	(Uint8)(m_buffer2[l+1] >> 24);
		green +=(Uint8)(m_buffer2[l] >> 16) +	(Uint8)(m_buffer2[l+1] >> 16);
		blue +=	(Uint8)(m_buffer2[l] >> 8) +	(Uint8)(m_buffer2[l+1] >> 8);
		setPixel(0, 0, (Uint8)(red / 9), (Uint8)(green / 9), (Uint8)(blue / 9));
		// Top Right ((SCREEN_WIDTH-2),0)
		l = SCREEN_WIDTH - 2;	// top right pixel at the box corner
		red =   (Uint8)(m_buffer2[l] >> 24) + (Uint8)(m_buffer2[l+1] >> 24);
		green = (Uint8)(m_buffer2[l] >> 16) + (Uint8)(m_buffer2[l+1] >> 16);
		blue =  (Uint8)(m_buffer2[l] >>  8) + (Uint8)(m_buffer2[l+1] >>  8);
		l = l + SCREEN_WIDTH;
		red +=   (Uint8)(m_buffer2[l] >> 24) + (Uint8)(m_buffer2[l+1] >> 24);
		green += (Uint8)(m_buffer2[l] >> 16) + (Uint8)(m_buffer2[l+1] >> 16);
		blue +=  (Uint8)(m_buffer2[l] >>  8) + (Uint8)(m_buffer2[l+1] >>  8);
		setPixel((SCREEN_WIDTH - 1), 0, (Uint8)(red / 9), (Uint8)(green / 9), (Uint8)(blue / 9));
		// Bottom Left (0,(SCREEN_HEIGHT-2))
		l = (SCREEN_HEIGHT - 2) * SCREEN_WIDTH;	// top left pixel at the corner
		red =   (Uint8)(m_buffer2[l] >> 24) + (Uint8)(m_buffer2[l+1] >> 24);
		green = (Uint8)(m_buffer2[l] >> 16) + (Uint8)(m_buffer2[l+1] >> 16);
		blue =  (Uint8)(m_buffer2[l] >>  8) + (Uint8)(m_buffer2[l+1] >>  8);
		l = SCREEN_WIDTH;
		red +=   (Uint8)(m_buffer2[l] >> 24) + (Uint8)(m_buffer2[l+1] >> 24);
		green += (Uint8)(m_buffer2[l] >> 16) + (Uint8)(m_buffer2[l+1] >> 16);
		blue +=  (Uint8)(m_buffer2[l] >>  8) + (Uint8)(m_buffer2[l+1] >>  8);
		setPixel(0, (SCREEN_HEIGHT - 1), (Uint8)(red / 9), (Uint8)(green / 9), (Uint8)(blue / 9));
		// Bottom Right ((SCREEN_WIDTH-2),(SCREEN_HEIGHT-2))
		l = (SCREEN_HEIGHT - 2) * SCREEN_WIDTH + SCREEN_WIDTH - 2;	// top left pixel at the corner
		red =   (Uint8)(m_buffer2[l] >> 24) + (Uint8)(m_buffer2[l+1] >> 24);
		green = (Uint8)(m_buffer2[l] >> 16) + (Uint8)(m_buffer2[l+1] >> 16);
		blue =  (Uint8)(m_buffer2[l] >>  8) + (Uint8)(m_buffer2[l+1] >>  8);
		l = SCREEN_WIDTH;
		red +=   (Uint8)(m_buffer2[l] >> 24) + (Uint8)(m_buffer2[l+1] >> 24);
		green += (Uint8)(m_buffer2[l] >> 16) + (Uint8)(m_buffer2[l+1] >> 16);
		blue +=  (Uint8)(m_buffer2[l] >> 8) +  (Uint8)(m_buffer2[l+1] >> 8);
		setPixel((SCREEN_WIDTH - 1), (SCREEN_HEIGHT - 1), (Uint8)(red / 9), (Uint8)(green / 9), (Uint8)(blue / 9));
		// stop timer
		blurTime = SDL_GetTicks() - blurTime;
		// gather statistics
		minBlurTime = (blurTime < minBlurTime) ? blurTime : minBlurTime;
		maxBlurTime = (blurTime > maxBlurTime) ? blurTime : maxBlurTime;
		if (blurCycles < MaxBlurCycles)
		{
			blurAverageTime += blurTime;
			blurCycles += 1;
		}
		else
		{
			if (timerON)
			{
				timerON = false;
				std::cout <<
					"boxBlur() time min: " <<
					minBlurTime <<
					"ms max: " <<
					maxBlurTime <<
					"ms" <<
					std::endl;;
				// average blur time
				std::cout <<
					"Average blur time for " <<
					SCREEN_HEIGHT * SCREEN_WIDTH <<
					" particles and " <<
					blurCycles <<
					" cycles was " <<
					(blurAverageTime / blurCycles) << " ms" <<
					std::endl;
			}	// end if
		}	// end if

	}	// end boxBlur()	



	void Screen::boxBlurJ()
	{
		// Swap the buffers, so pixel is in m_buffer2 and we are drawing to m_buffer1.

		Uint32* temp = m_buffer1;
		m_buffer1 = m_buffer2;
		m_buffer2 = temp;

		blurTime = SDL_GetTicks();
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			for (int x = 0; x < SCREEN_WIDTH; x++) {

				/*
					* 0 0 0
					* 0 1 0
					* 0 0 0
					*/

				int redTotal = 0;
				int greenTotal = 0;
				int blueTotal = 0;

				for (int row = -1; row <= 1; row++) {
					for (int col = -1; col <= 1; col++) {
						int currentX = x + col;
						int currentY = y + row;

						if (currentX >= 0 && currentX < SCREEN_WIDTH && currentY >= 0 && currentY < SCREEN_HEIGHT) {
							Uint32 color = m_buffer2[currentY * SCREEN_WIDTH + currentX];

							Uint8 red = color >> 24;
							Uint8 green = color >> 16;
							Uint8 blue = color >> 8;

							redTotal += red;
							greenTotal += green;
							blueTotal += blue;
						}
					}
				}

				Uint8 red = redTotal / 9;
				Uint8 green = greenTotal / 9;
				Uint8 blue = blueTotal / 9;

				setPixel(x, y, red, green, blue);
			}
		}
		blurTime = SDL_GetTicks() - blurTime;
		//std::cout <<
		//	"[" << blurCycles << "/" <<
		//	MaxBlurCycles << "] blur Time was " <<
		//	blurTime << "ms" << 
		//	std::endl;
		minBlurTime = (blurTime < minBlurTime) ? blurTime : minBlurTime;
		maxBlurTime = (blurTime > maxBlurTime) ? blurTime : maxBlurTime;
		if (blurCycles < MaxBlurCycles)
		{
			blurAverageTime += blurTime;
			blurCycles += 1;
		}

	}	// end boxBlurJ()


	void Screen::clear()
	{
		memset(m_buffer1, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
		memset(m_buffer2, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
		return;
	}

	void Screen::close()
	{
		delete[] m_buffer1;
		delete[] m_buffer2;
		SDL_DestroyTexture(m_texture);
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return;
	}	// end close()
	
	void Screen::fillScreen(Uint8 red, Uint8 green, Uint8 blue)
	{
		for (int y = 0; y < SCREEN_HEIGHT; y++)
		{
			for (int x = 0; x < SCREEN_WIDTH; x++)
			{
				setPixel(x, y, red, green, blue);
			}	// end for
		}	// end for

	};	// end fillScreen

	bool Screen::init()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cout << "SDL Error: " << SDL_GetError() << std::endl;
			return false;
		}
		else
		{
			std::cout << "SDL Ready!!" << std::endl;
		}

		m_window = SDL_CreateWindow(
			"*Particle Fire Explosion",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN
		);
		if (m_window == NULL)
		{
			SDL_Quit();
			return false;
		}
		else
		{
			std::cout << "window created" << std::endl;
		}

		m_renderer =
			SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
		if (m_renderer == NULL)
		{
			std::cout << "Could not create renderer" << std::endl;
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			return false;
		}
		else
		{
			std::cout << "renderer created" << std::endl;
		}	// end if

		m_texture =
			SDL_CreateTexture(
				m_renderer,
				SDL_PIXELFORMAT_RGBA8888,
				SDL_TEXTUREACCESS_STATIC,
				SCREEN_WIDTH,
				SCREEN_HEIGHT
			);
		if (m_texture == NULL)
		{
			std::cout << "Could not create texture" << std::endl;
			SDL_DestroyRenderer(m_renderer);
			SDL_DestroyWindow(m_window);
			SDL_Quit();
			return false;
		}
		else
		{
			std::cout << "texture created" << std::endl;
		}	// end if

		SDL_UpdateTexture(m_texture, NULL, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));
		SDL_RenderClear(m_renderer);
		SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
		SDL_RenderPresent(m_renderer);
		bool quit = false;

		return true;
	}	// end init()
	
	bool Screen::processEvents()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				return true;
			}	// end if
			return false;
		}	// end while

		return false;
	}	// end processEvents()

	void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue)
	{
		const int alpha = 0xFF;
		// test if pixel coordinates is off screen
		if (x < 0) return;
		if (x >= SCREEN_WIDTH) return;
		if (y < 0) return;
		if (y >= SCREEN_HEIGHT) return;

		Uint32 color = (red<<24) + (green<<16) + (blue<<8) + alpha;
		m_buffer1[(y * SCREEN_WIDTH) + x] = color;
		return;
	};	// end setPixel()

	void Screen::update()
	{
		SDL_UpdateTexture(m_texture, NULL, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));
		SDL_RenderClear(m_renderer);
		SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
		SDL_RenderPresent(m_renderer);
	}

}	// end namespace particles
