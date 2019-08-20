#pragma once
#include "Particle.h"

namespace particles
{
	class Swarm
	{
	public:
		const static int NPARTICLES = 5000;

	private:
		Particle* m_pParticles;
		int			lastTime;

	public:
		Swarm();
		virtual ~Swarm();

		const Particle* const getParticles()
		{ 
			return m_pParticles;
		};

		void update(int);
	};

}	// namespace particles
