/* Copyright (c) 2010-2011 Andrée Ekroth
 * See the file LICENSE.txt for copying permission.
 */

#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include "BaseTypes.hpp"
#include "GameObject.hpp"
#include <glm/glm.hpp>

typedef unsigned int uint;

namespace kke
{
class ResourceManager;

/**
 * @brief Manages particles, small objects.
 **/
class ParticleSystem : public GameObject
{
public:
	ParticleSystem(uint maxParticles, const glm::vec2& position);
	virtual ~ParticleSystem();

	virtual void LoadContent();
	virtual void UnloadContent();
	virtual void Update(float elapsed) = 0;
	virtual void Draw(const kke::IRenderer& renderer) = 0;

	/**
	 * @brief Emits an amount of particles by calling abstract function InitParticle.
	 *
	 * @param amount Amount of particles.
	 * @return void
	 **/
	virtual void Emit(uint amount);

	/**
	 * @brief Get position.
	 *
	 * @return const kke::Vector2f&
	 **/
	const glm::vec2& GetPosition() const;

	/**
	 * @brief Set position.
	 *
	 * @param position Position.
	 * @return void
	 **/
	void SetPosition(const glm::vec2& position);

	/**
	 * @brief Get force.
	 *
	 * @return const kke::Vector2f&
	 **/
	const glm::vec2& GetForce() const;

	/**
	 * @brief Set force.
	 *
	 * @param force Force.
	 * @return void
	 **/
	void SetForce(const glm::vec2 &force);

	/**
	 * @brief Amount of active particles.
	 *
	 * @return int
	 **/
	int GetActive() const;

	/**
	 * @brief Amount of inactive particles.
	 *
	 * @return int
	 **/
	int GetInactive() const;

	/**
	 * @brief Amount of inactive and active particles.
	 *
	 * @return int
	 **/
	int GetAll() const;
protected:

	/**
	 * @brief Initialize a particle.
	 *
	 * @param index Index of particle to initialize.
	 * @return void
	 **/
	virtual void InitParticle(uint index) = 0;

	/**
	 * @brief Remove a particle.
	 *
	 * @param index Index of particle to remove.
	 * @return void
	 **/
	virtual void RemoveParticle(uint index) = 0;

	glm::vec2 position;
	uint maxParticles;
	uint numParticles;
	glm::vec2 force;
	bool isLoaded;
};
}

#endif
