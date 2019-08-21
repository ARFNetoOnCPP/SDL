#include <math.h>
#include <stdlib.h>
#include "Particle.h"
#include "SDL.h"

#define M_PI 3.14159265358979323846264338327950288

namespace particles
{

	Particle::Particle()
	{
		init();
	}

	Particle::~Particle()
	{
		// destructor
	}

	void Particle::init()
	{
		m_x = 0;
		m_y = 0;
		m_direction = (2 * M_PI * rand()) / RAND_MAX;
		m_speed = (0.04 * rand()) / RAND_MAX;
		m_speed = m_speed * m_speed;
	}	// end init()

	void Particle::update(int interval)
	{
		//75
		m_direction += interval * 0.0002;
		double xspeed = m_speed * cos(m_direction);
		double yspeed = m_speed * sin(m_direction);
		m_x += xspeed * interval;
		m_y += yspeed * interval;

		if
		( 
			(m_x<= -1) || 
			(m_x>=1) || 
			(m_y<= -1) || 
			(m_y>= 1) ||
			((SDL_GetTicks() % 100) < 2)
		) init();

		return;
	}	// end update()


}	// namespace particles
