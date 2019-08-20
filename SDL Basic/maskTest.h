#pragma once
#include <iostream>
#include <iomanip>
#include "SDL.h"

using namespace std;
class MaskTest
{
	unsigned const int alpha =	0xFF;
	unsigned const int R =		0x12;
	unsigned const int G =		0x34;
	unsigned const int B =		0x56;

	
public:
	static unsigned char  getRed(unsigned int color)
	{
		cout << "***** getRed(color)" << endl;
		std::cout <<
			"color: " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			color <<
			std::endl;
		color = color >> 24;
		std::cout <<
			"Red component: " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			color <<
			std::endl;
		return color;
	}	// end getRed()

	static unsigned char  getGreen(unsigned int color)
	{
		cout << "***** getGreen(color)" << endl;
		std::cout <<
			"color: " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			color <<
			std::endl;
		color = (color >> 16) & 0xFF;
		std::cout <<
			"Green component: " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			color <<
			std::endl;
		return color;
	}	// end getGreen()

	static unsigned char  getBlue(unsigned int color)
	{
		cout << "***** getBlue(color)" << endl;
		std::cout <<
			"color: " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			color <<
			std::endl;
		color = (color >> 8) & 0xFF;
		std::cout <<
			"Blue component: " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			color <<
			std::endl;
		return color;
	}	// end getBlue()

	MaskTest()
	{
		unsigned int color;
		cout << "***** mask test constructor" << endl;

		color = 0;
		std::cout << "color at start: " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			color <<
			std::endl;
		// red
		color += R;
		std::cout <<
			"color:        add red   \t= 0x12: " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			color <<
			std::endl;
		// green
		color <<= 8;
		color += G;
		std::cout <<
			"color: shift and add green \t= 0x34: " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			color <<
			std::endl;
		// blue
		color <<= 8;
		color += B;
		std::cout <<
			"color: shift and add blue \t= 0x56: " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			color <<
			std::endl;
		// alpha
		color <<= 8;
		color += alpha;
		std::cout <<
			"color: shift and add alpha \t= 0xFF: " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			color <<
			std::endl;
	}	// end MaskTest()

	void maskSet(Uint8 red, Uint8 green, Uint8 blue)
	{
		cout << "***** maskSet(red,green,blue)" << endl;
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

		Uint32 Lecture66 = alpha;
		Lecture66 <<= 8;
		Lecture66 += red;
		Lecture66 <<= 8;
		Lecture66 += green;
		Lecture66 <<= 8;
		Lecture66 += blue;

		cout <<
			"computed mask (Lecture 66): " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			Lecture66 <<
			std::endl;

		Uint32 Lecture67 = 0;
		Lecture67 += red;
		Lecture67 <<= 8;
		Lecture67 += green;
		Lecture67 <<= 8;
		Lecture67 += blue;
		Lecture67 <<= 8;
		Lecture67 += alpha;

		cout <<
			"computed mask (Lecture 67): " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			Lecture67 <<
			std::endl;

		Uint32 color = (red << 24) + (green << 16) + (blue << 8) + alpha;
		cout <<
			"computed mask (    Faster): " <<
			std::setfill('0') <<
			std::setw(8) <<
			std::hex <<
			color <<
			std::endl;

		return;

	}	// end MaskSet()

};	// end struct MaskTest

