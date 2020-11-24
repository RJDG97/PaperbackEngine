#include "Engine/Core.h"
#include "Systems/ParticleSystem.h"
#include "Manager/ComponentManager.h"


void ParticleSystem::Init() {
	
	component_manager_ = CORE->GetManager<ComponentManager>();
	particle_arr_ = component_manager_->GetComponentArray<Particle>();
	emitter_arr_ = component_manager_->GetComponentArray<Emitter>();
}


void ParticleSystem::Update(float frametime) {

	if (CORE->GetCorePauseStatus())
		return;

	// Update lifetime of emitter and deactivate spawning
	// if emitter is inactive
	for (auto& [id, emitter] : *emitter_arr_) {

		if (!emitter->IsAlive())
			continue;

		emitter->lifetime_ -= frametime;

		if (emitter->lifetime_ < 0.0f) {

			emitter->alive_ = false;
			return;
		}

		emitter->Spawn(frametime);
	}


	// Update lifetime of particle and deactivate so
	// graphics does not render
	for (auto& [id, particle] : *particle_arr_) {

		if (!particle->IsAlive())
			continue;

		particle->lifetime_ -= frametime;

		if (particle->lifetime_ < 0.0f) {

			particle->alive_ = false;
			particle->lifetime_ = 0.0f;
			component_manager_->GetComponent<Motion>(id)->alive_ = false;
			component_manager_->GetComponent<TextureRenderer>(id)->alive_ = false;

			// restore quota to emitter
			Emitter* spawner = component_manager_->GetComponent<Emitter>(particle->spawner_);
			--spawner->current_spawn_;
		}
	}
}


void ParticleSystem::Draw() {
	
}


void ParticleSystem::SendMessageD(Message* m) {
	
}