#include <math.h>
#include <stdlib.h>
#include "Particle.h"

#define M_PI 3.14159265358979323846264338327950288

namespace particles
{

	Particle::Particle():	m_x(0), 
							m_y(0)
	{
		// lecture 71 introduced initializers at screen center
		//	m_x = ((2.0 * rand()) / RAND_MAX) - 1;
		//	m_y = ((2.0 * rand()) / RAND_MAX) - 1;
		//	m_xspeed = 0.01 * (((2.0 * rand()) / RAND_MAX) - 1);
		//	m_yspeed = 0.01 * (((2.0 * rand()) / RAND_MAX) - 1);
		m_direction = (2 * M_PI * rand()) / RAND_MAX;
		m_speed = (0.4 * rand()) / RAND_MAX;
		m_speed = m_speed * m_speed;
	}

	Particle::~Particle()
	{
	}

	void Particle::update(int interval)
	{
		double xspeed = m_speed * cos(m_direction);
		double yspeed = m_speed * sin(m_direction);
		m_x += xspeed * interval;
		m_y += yspeed * interval;
		return;
	}	// end update()


}	// namespace particles
