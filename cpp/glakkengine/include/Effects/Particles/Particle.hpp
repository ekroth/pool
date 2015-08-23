/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include "BaseTypes.hpp"

namespace kke
{
class ParticleSystem;

struct Particle
{
	glm::vec2 Position,
			  Velocity,
			  Acceleration,
			  Scale;
	float Weight;
	float Energy;
	Colorf Color;

	Particle() :
		Position(0, 0), Velocity(0, 0), Acceleration(0, 0), Scale(0, 0),
			Weight(0), Energy(0), Color(0.0f, 0.0f, 0.0f, 1.0f)
	{
	}
};
}

#endif
