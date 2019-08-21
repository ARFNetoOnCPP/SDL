#include "Screen.h"
#include <iostream>
namespace particles
{
	Screen::Screen():
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

				 so these are the 9 points we are considering

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
		Uint32 baseY = 0;
		for (int y = 1; y < SCREEN_HEIGHT - 1; y++)
		{
			for (int x = 1; x < SCREEN_WIDTH - 1; x++)
			{
				Uint32 l = baseY + x - 1;	// left pixel on this row: 1
				Uint32 red =	
					(Uint8) (m_buffer2[l] >> 24) + 
					(Uint8) (m_buffer2[l + 1] >> 24) + 
					(Uint8) (m_buffer2[l + 2] >> 24);	// 1 2 3
				Uint32 green =
					(Uint8) (m_buffer2[l] >> 16) + 
					(Uint8) (m_buffer2[l + 1] >> 16) + 
					(Uint8) (m_buffer2[l + 2] >> 16);
				Uint32 blue =  
					(Uint8) (m_buffer2[l] >>  8) + 
					(Uint8) (m_buffer2[l + 1] >>  8) + 
					(Uint8) (m_buffer2[l + 2] >>  8);

				l = l + SCREEN_WIDTH;		// now we are at pixel 4
				red   += 
					(Uint8) (m_buffer2[l] >> 24) + 
					(Uint8) (m_buffer2[l + 1] >> 24) + 
					(Uint8) (m_buffer2[l + 2] >> 24);		// 4 5 6
				green += 
					(Uint8) (m_buffer2[l] >> 16) + 
					(Uint8) (m_buffer2[l + 1] >> 16) + 
					(Uint8) (m_buffer2[l + 2] >> 16);	
				blue  += 
					(Uint8) (m_buffer2[l] >>  8) + 
					(Uint8) (m_buffer2[l + 1] >>  8) + 
					(Uint8) (m_buffer2[l + 2] >>  8);

				l = l + SCREEN_WIDTH;		// now we are at pixel 7
				red   += 
					(Uint8) (m_buffer2[l] >> 24) + 
					(Uint8) (m_buffer2[l + 1] >> 24) + 
					(Uint8) (m_buffer2[l + 2] >> 24);	// 7 8 9
				green += 
					(Uint8) (m_buffer2[l] >> 16) + 
					(Uint8) (m_buffer2[l + 1] >> 16) + (Uint8) 
					(m_buffer2[l + 2] >> 16);
				blue  += 
					(Uint8) (m_buffer2[l] >>  8) + 
					(Uint8) (m_buffer2[l + 1] >>  8) + 
					(Uint8) (m_buffer2[l + 2] >>  8);
				//
				// if we did it ok, just divide by 9 to get the average color for the pixel 5 (x,y)
				setPixel(x, y, (Uint8) (red/9), (Uint8) (green/9), (Uint8)(blue/9) );

			}	// end for x
			baseY += SCREEN_WIDTH;
		}	// end for y

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

	}	// end boxBlurB	

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
