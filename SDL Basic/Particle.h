#pragma once
namespace particles
{

	struct Particle
	{
		double	m_x;
		double	m_y;
		double	m_speed;
		double	m_direction;

	public:
		Particle();
		virtual ~Particle();

		void update(int);
	};	// end struct Particle

}	// namespace particles
