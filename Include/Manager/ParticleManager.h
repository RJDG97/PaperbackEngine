/**********************************************************************************
*\file         ParticleManager.h
*\brief        Contains declaration of functions and variables used for
*			   the Particle Manager
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


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

	using UIParticleType = CMap<UIParticle>;
	using UIParticleIt = UIParticleType::MapTypeIt;


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
	void GetParticles(std::vector<EntityID>& vec, size_t count, bool ui);


private:
	std::shared_ptr<ComponentManager> component_manager_;
	ParticleType* particle_arr_;
	UIParticleType* ui_particle_arr_;
};


#endif