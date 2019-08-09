#pragma once
#include <iostream>
#include <iomanip>

using namespace std;
struct maskTest
{
	unsigned const static int alpha = 0xFF;
	unsigned const static int red = 0x12;
	unsigned const static int green = 0x34;
	unsigned const static int blue = 0x56;

	unsigned int color;
	
	maskTest()
	{
		color = 0;
		std::cout << "color at start: " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			color <<
			std::endl;
		// alpha
		color += alpha;
		std::cout <<
			"color:        add alpha \t= 0xff: " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			color <<
			std::endl;
		// Red
		color <<= 8;
		color += red;
		std::cout <<
			"color: shift and add red \t= 0x12: " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			color <<
			std::endl;
		// Green
		color <<= 8;
		color += green;
		std::cout <<
			"color: shift and add green \t= 0x34: " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			color <<
			std::endl;
		// Blue
		color <<= 8;
		color += blue;
		std::cout <<
			"color: shift and add blue \t= 0x56: " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			color <<
			std::endl;
	}

	void maskSet(Uint8 red, Uint8 green, Uint8 blue)
	{
		const int alpha = 255;

		cout <<
			"red   component:" <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			(Uint32)red <<
			std::endl;

		cout <<
			"green component:" <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<  
			(Uint32)green <<
			std::endl;

		cout <<
			"blue  component:" <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			(Uint32)blue <<
			std::endl;

		cout <<
			"alpha component:" <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			alpha <<
			std::endl;

		Uint32 stdColor = red;
		stdColor <<= 8;
		stdColor += green;
		stdColor <<= 8;
		stdColor += blue;
		stdColor <<= 8;
		stdColor += alpha;

		cout <<
			"computed mask (A): " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			stdColor <<
			std::endl;

		Uint32 altColor = (alpha<<24) + (red<<16) + (green<<8) + blue;
		cout <<
			"computed mask (B): " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			altColor <<
			std::endl;

		return;

	}	// end maskSet()
};

