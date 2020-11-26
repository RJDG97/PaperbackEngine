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