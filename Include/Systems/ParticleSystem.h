/**********************************************************************************
*\file         ParticleSystem.h
*\brief        Contains declaration of functions and variables used for
*			   the Particle System
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#include "Systems/ISystem.h"
#include "Manager/ComponentManager.h"
#include "Components/Particle.h"
#include "Components/Emitter.h"

class ParticleSystem : public ISystem
{
public:
	using ParticleType = CMap<Particle>;
	using ParticleIt = ParticleType::MapTypeIt;

	using UIParticleType = CMap<UIParticle>;
	using UIParticleIt = UIParticleType::MapTypeIt;

	using EmitterType = CMap<Emitter>;
	using EmitterIt = EmitterType::MapTypeIt;


/******************************************************************************/
/*!
  \fn Init()

  \brief Used to initialise a system if required;
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
  \fn Update()

  \brief Used to run logic that is required every game loop.
*/
/******************************************************************************/
	void Update(float frametime);

/******************************************************************************/
/*!
  \fn Draw()

  \brief Used to contain logic for rendering activities, to use if necessary.
*/
/******************************************************************************/
	void Draw();

/******************************************************************************/
/*!
  \fn GetName()

  \brief Returns the name of the system
*/
/******************************************************************************/
	std::string GetName() { return "ParticleSystem"; }

/******************************************************************************/
/*!
  \fn SendMessageD()

  \brief Handles incoming messages and sorts based on message id
*/
/******************************************************************************/
	void SendMessageD(Message* m);


private:
	std::shared_ptr<ComponentManager> component_manager_;
	UIParticleType* ui_particle_arr_;
	ParticleType* particle_arr_;
	EmitterType* emitter_arr_;

};


#endif