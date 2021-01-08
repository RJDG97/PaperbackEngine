/**********************************************************************************
*\file         ParticleManager.cpp
*\brief        Contains definition of functions and variables used for
*			   the Particle Manager
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Manager/ParticleManager.h"
#include "Engine/Core.h"


void ParticleManager::Init() {
	
	component_manager_ = CORE->GetManager<ComponentManager>();
	particle_arr_ = component_manager_->GetComponentArray<Particle>();
}


void ParticleManager::Update(float frametime) {

	(void)frametime;
}


void ParticleManager::GetParticles(std::vector<EntityID>& vec, size_t count) {
	
	size_t counter = count;

	for (auto& [id, particle] : *particle_arr_) {
		
		if (particle->IsAlive())
			continue;

		if (!counter)
			return;

		particle->alive_ = true;
		vec.push_back(id);

		--counter;
	}
}