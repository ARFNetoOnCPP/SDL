#include "Swarm.h"
#include "Particle.h"

namespace particles
{

	Swarm::Swarm(unsigned int n):
		lastTime(0)
	{
		NPARTICLES = n;
		m_pParticles = new Particle[NPARTICLES];

	}

	Swarm::~Swarm()
	{
		delete[] m_pParticles;
	}

	void Swarm::update(int elapsed)
	{
		int interval = elapsed - lastTime;
		for (int i = 0; i < Swarm::NPARTICLES; i++)
		{
			m_pParticles[i].update(interval);
		}	// end for
		lastTime = elapsed;
	}	// end update()

} // namespace particles
