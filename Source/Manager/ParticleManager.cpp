/**********************************************************************************
*\file         ParticleManager.cpp
*\brief        Contains definition of functions and variables used for
*			   the Particle Manager
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Manager/ParticleManager.h"
#include "Engine/Core.h"


void ParticleManager::Init() {
	
	component_manager_ = CORE->GetManager<ComponentManager>();
	particle_arr_ = component_manager_->GetComponentArray<Particle>();
	ui_particle_arr_ = component_manager_->GetComponentArray<UIParticle>();
}


void ParticleManager::Update(float frametime) {

	(void)frametime;
}


void ParticleManager::GetParticles(std::vector<EntityID>& vec, size_t count, bool ui) {
	
	size_t counter = count;

	if (ui) {
		
		for (auto& [id, ui_particle] : *ui_particle_arr_) {

			if (ui_particle->IsAlive())
				continue;

			if (!counter)
				return;

			ui_particle->alive_ = true;
			vec.push_back(id);

			--counter;
		}
	}
	else {

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
}