#pragma once
#ifndef _PARTICLE_MANAGER_H_
#define _PARTICLE_MANAGER_H_

#include "Manager/IManager.h"
#include "Manager/ComponentManager.h"
#include "Entity/Entity.h"
#include "Components/Particle.h"

class ParticleManager : public IManager
{
public:
	using ParticleType = CMap<Particle>;
	using ParticleIt = ParticleType::MapTypeIt;


/******************************************************************************/
/*!
  \fn Init()

  \brief Init particle manager
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
  \fn Update()

  \brief Update all particles
*/
/******************************************************************************/
	void Update(float frametime);

/******************************************************************************/
/*!
  \fn GetParticle()

  \brief Assigns a set number of particles to emitter to use
*/
/******************************************************************************/
	void GetParticles(std::vector<EntityID>& vec, size_t count);


private:
	std::shared_ptr<ComponentManager> component_manager_;
	ParticleType* particle_arr_;

};


#endif