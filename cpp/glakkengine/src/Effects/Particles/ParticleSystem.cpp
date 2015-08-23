/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#include "Effects/Particles/ParticleSystem.hpp"
#include "Effects/Particles/Particle.hpp"
#include "ResourceFactory.hpp"

using namespace kke;

ParticleSystem::ParticleSystem(uint maxParticles, const Vector2f& position) :
	position(position),
	maxParticles(maxParticles),
	numParticles(0),
	force(0.0f, 0.0f),
	isLoaded(false)
{
}

ParticleSystem::~ParticleSystem()
{
	// If system is loaded, unload resources
	if (isLoaded)
	{
		UnloadContent();
	}
}

void ParticleSystem::LoadContent()
{
	isLoaded = true;
}

void ParticleSystem::UnloadContent()
{
	isLoaded = false;
}

void ParticleSystem::Emit(uint amount)
{
	while (amount > 0 && numParticles < maxParticles)
	{
		// <var>++ = ++ after being sent (TODO: Remove this comment)
		InitParticle(numParticles++);
		amount--;
	}
}

const Vector2f& ParticleSystem::GetPosition() const
{
	return position;
}

void ParticleSystem::SetPosition(const Vector2f& position)
{
	this->position = position;
}

const Vector2f& ParticleSystem::GetForce() const
{
	return force;
}

void ParticleSystem::SetForce(const Vector2f& force)
{
	this->force = force;
}

int ParticleSystem::GetActive() const
{
	return numParticles;
}

int ParticleSystem::GetInactive() const
{
	return maxParticles - numParticles;
}

int ParticleSystem::GetAll() const
{
	return maxParticles;
}
