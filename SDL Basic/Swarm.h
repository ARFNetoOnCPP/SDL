#pragma once
#include "Particle.h"

namespace particles
{
	class Swarm
	{
	public:
		int NPARTICLES;

	private:
		Particle* m_pParticles;
		int			lastTime;

	public:
		Swarm(unsigned int);
		virtual ~Swarm();

		const Particle* const getParticles()
		{ 
			return m_pParticles;
		};

		void update(int);
	};

}	// namespace particles
