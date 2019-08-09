#include "Screen.h"
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

		m_buffer = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT];
		memset(m_buffer, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

	}	// end Screen()

	void Screen::close()
	{
		delete[] m_buffer;
		SDL_DestroyTexture(m_texture);
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return;
	}	// end close()
	
	void Screen::fillScreen(Uint8 red, Uint8 green, Uint8 blue)
	{
		std::cout <<
			"----- fillScreen(" << (Uint32) red <<
			", " << (Uint32) green <<
			", " << (Uint32) blue <<
			") -----" <<
			std::endl;

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
			std::cout << "windows created" << std::endl;
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

		fillScreen((Uint8)0, (Uint8)0, (Uint8)255);

		SDL_UpdateTexture(m_texture, NULL, m_buffer, SCREEN_WIDTH * sizeof(Uint32));
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
		Uint32 color = (red<<24) + (green<<16) + (blue<<8) + 0xFF;
		m_buffer[(y * SCREEN_WIDTH) + x] = color;
		return;
	};	// end setPixel()

}	// end namespace particles
