/**********************************************************************************
*\file         ForcesManager.cpp
*\brief        Contains definition of functions and variables used for
*			   the Forces Manager
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Engine/Core.h"
#include "Manager/ForcesManager.h"
#include "Manager/ComponentManager.h"
#include "Systems/Debug.h"
#include <iostream>
#include <algorithm>
#include <functional>

void RemoveExpiredCollectibles(std::map<EntityID, ForcesManager::EntityForces>::iterator it) {
	
	ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
	auto& [id, ptr] = *it;

	Collectible* collectible = component_mgr->GetComponent<Collectible>(id);

	if (collectible)
	{
		AnimationRenderer* animation_renderer = component_mgr->GetComponent<AnimationRenderer>(id);
		TextureRenderer* texture_renderer = component_mgr->GetComponent<TextureRenderer>(id);
		PointLight* point_light = component_mgr->GetComponent<PointLight>(id);
		AABB* aabb = component_mgr->GetComponent<AABB>(id);

		if (animation_renderer)
			animation_renderer->SetAlive(false);
		if (texture_renderer)
			texture_renderer->SetAlive(false);
		if (point_light)
			point_light->SetAlive(false);
		if (aabb)
			aabb->SetAlive(false);
	}
}

auto RemoveVecCheck = [](ForcesManager::ForceVecIt it) {
	if (it->age_ >= it->lifespan_) {
		return true;
	}
	return false;
};

auto RemoveEntryCheck = [](ForcesManager::ForceMapIt it) {
	if (it->second.vec_.empty()) {
		return true;
	}
	return false;
};

bool ForcesManager::EntityForces::VerifyForce(const std::string& name) {

	for (ForceVecIt begin = vec_.begin(); begin != vec_.end(); ++begin) {
		
		if (begin->name_ == name) {

			return false;
		}
	}

	return true;
}

ForcesManager::Force::Force(const std::string& name, const float& lifespan, const Vector2D& force) :
	name_{ name },
	age_{ },
	lifespan_{ lifespan },
	force_{ force }
{
}

void ForcesManager::AddForce(EntityID id, const std::string& name, const float& lifespan, const Vector2D& force) {
	
	//access vector
	EntityForces* entity = &force_map_[id];
	
	//ensure that force does not already exist
	if (entity->VerifyForce(name)) {
		//create new entry
		entity->vec_.push_back({ name, lifespan, force });
	}
}

void ForcesManager::Init() {
	// Empty body
}

void ForcesManager::Update(float frametime) {
	for (ForceMapIt begin_map = force_map_.begin(); begin_map != force_map_.end();) {
		// Reset forces for entity
		Vector2D sum{};

		for (ForceVecIt begin = begin_map->second.vec_.begin(); begin != begin_map->second.vec_.end();) {

			begin->age_ += frametime;

			// Recompute forces for entity
			sum += begin->force_;

			if (RemoveVecCheck(begin)) {

				RemoveExpiredCollectibles(begin_map);
				begin = begin_map->second.vec_.erase(begin);
				continue;
			}

			++begin;
		}

		begin_map->second.sum_of_forces_ = sum;

		++begin_map;
	}
}

Vector2D ForcesManager::GetForce(EntityID id) {

	ForceMapIt it = force_map_.find(id);

	if (it != force_map_.end()) {

		M_DEBUG->WriteDebugMessage("Force obtained: " + std::to_string(it->second.sum_of_forces_.x) + "\n");
		return it->second.sum_of_forces_;
	}

	return Vector2D{};
}

void ForcesManager::Purge() {

	force_map_.clear();
}