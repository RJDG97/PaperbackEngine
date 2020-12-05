/**********************************************************************************
*\file         TransitionManager.cpp
*\brief        Contains definition of functions and variables used for
*			   the Transition Manager
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Engine/Core.h"
#include "Systems/Game.h"
#include "Manager/TransitionManager.h"


TransitionManager::TransitionManager() : 
	transition_speed_{ },
	max_size_{ 800.0f, 450.0f },
	max_clear_size_{ 320.0f, 180.0f },
	current_size_{ },
	current_transition_{ nullptr },
	next_state_{ nullptr },
	begin_{ false },
	end_{ false }
{ 

}


void TransitionManager::Init() {
	
	current_transition_ = nullptr;
	begin_ = false;

	graphics_system_ = CORE->GetSystem<GraphicsSystem>();
	component_manager_ = CORE->GetManager<ComponentManager>();
	texture_arr_ = component_manager_->GetComponentArray<TextureRenderer>();

	DeSerialize("Resources/AssetsLoading/Scene_transition.json");
}


void TransitionManager::ResetTransition(const std::string& id, GameState* next_state) {

	if (current_transition_)
		return;
	
	SceneTransitionsIt it = transition_map_.find(id);

	// This works based on the assumption that there is at least 1 texture name loaded in from the json
	// and there is only 1 texture renderer component (Single entity)

	if (it != transition_map_.end()) {
	
		current_size_ = {};
		clear_current_size_ = {};
		graphics_system_->SetVignetteSize(clear_current_size_);
		graphics_system_->SetMaxVignetteSize(current_size_);
		current_transition_ = &*it->second;
		transition_speed_ = max_size_ / current_transition_->default_transition_timer_;

		if (current_transition_->skip_) {
			
			end_ = true;
			next_state_ = next_state;
			current_transition_->current_texture_ = current_transition_->texture_sequence_.begin();
			return;
		}

		current_transition_->current_texture_ = current_transition_->texture_sequence_.begin();
		begin_ = true;
		next_state_ = next_state;

		// Set initial texture
		graphics_system_->ChangeTexture(texture_arr_->GetComponent(1), *current_transition_->current_texture_);
	}
}


bool TransitionManager::DelayTransition(const float& frametime) {
	
	if (current_transition_->dark_timer_ > 0.0f) {

		current_transition_->dark_timer_ -= frametime;

		if (current_transition_->dark_timer_ < 0.0f) {

			current_transition_->dark_timer_ = 0.0f;
			begin_ = true;
		}

		return true;
	}

	return false;
}


void TransitionManager::OpenTransition(const float& frametime) {

	current_size_ += transition_speed_ * frametime;
	clear_current_size_ = { std::max(0.0f, current_size_.x - 320.0f), std::max(0.0f, current_size_.y - 180.0f) };

	if (clear_current_size_.x > max_clear_size_.x && clear_current_size_.y > max_clear_size_.y) {

		clear_current_size_ = max_clear_size_;
	}

	if (current_size_.x > max_size_.x && current_size_.y > max_size_.y) {

		begin_ = false;
		current_size_ = max_size_;
		clear_current_size_ = max_clear_size_;

		// If the current transition is null, reset
		if (current_transition_->current_texture_ == current_transition_->texture_sequence_.end()) {
		
			current_transition_ = nullptr;
			next_state_ = nullptr;
		}
	}

	graphics_system_->SetMaxVignetteSize(current_size_);
	graphics_system_->SetVignetteSize(clear_current_size_);
}


void TransitionManager::CloseTransition(const float& frametime) {

	current_size_ -= transition_speed_ * frametime;
	clear_current_size_ = { std::max(0.0f, current_size_.x - 320.0f), std::max(0.0f, current_size_.y - 180.0f) };

	if (clear_current_size_.x > max_clear_size_.x && clear_current_size_.y > max_clear_size_.y) {

		clear_current_size_ = max_clear_size_;
	}

	if (current_size_.x < 0.0f && current_size_.y < 0.0f) {

		end_ = false;
		current_size_ = { 0.0f, 0.0f };
		clear_current_size_ = { 0.0f, 0.0f };

		if (current_transition_->current_texture_ != current_transition_->texture_sequence_.end() &&
			++current_transition_->current_texture_ != current_transition_->texture_sequence_.end()) {
			
			graphics_system_->ChangeTexture(texture_arr_->GetComponent(1), *current_transition_->current_texture_);
			current_transition_->dark_timer_ = current_transition_->default_dark_timer_;
		}

		else {
			
			// change state
			CORE->GetSystem<Game>()->ChangeState(next_state_);

			// Last scene is completed
			/*current_transition_ = nullptr;
			next_state_ = nullptr;*/
			begin_ = true;
		}
	}

	graphics_system_->SetMaxVignetteSize(current_size_);
	graphics_system_->SetVignetteSize(clear_current_size_);
}


void TransitionManager::LoadTransition(std::string name, std::stringstream& data) {
	
	std::shared_ptr<SceneTransition> scene = std::make_shared<SceneTransition>();

	// Read in data from the stream
	data >> scene->size_ >> scene->default_timer_ >> scene->default_transition_timer_ >> scene->default_dark_timer_;
	scene->timer_ = scene->default_timer_;
	scene->skip_ = scene->size_ <= 0 ? 1 : 0;

	// Initialize vector of textures to be changed in sequence
	for (size_t i = 0; i < scene->size_; ++i) {
		
		std::string texture_name{};
		data >> texture_name;

		scene->texture_sequence_.push_back(texture_name);
	}

	transition_map_[name] = scene;
}


void TransitionManager::DeSerialize(const std::string& filepath) {

	// Parse the stringstream into document (DOM) format
	rapidjson::Document doc;
	DeSerializeJSON(filepath, doc);

	// Treats entire filestream at index as array and ensure that it is an array
	const rapidjson::Value& transition_arr_ = doc;
	DEBUG_ASSERT(transition_arr_.IsObject(), "Entry does not exist in JSON");

	// Iterate through each prefab
	for (rapidjson::Value::ConstMemberIterator var_it = transition_arr_.MemberBegin(); var_it != transition_arr_.MemberEnd(); ++var_it) {

		const rapidjson::Value& value_arr = var_it->value;

		std::string state_name{ var_it->name.GetString() };

		M_DEBUG->WriteDebugMessage("Loading transition details: " + state_name + "\n");

		// Iterate through the body of the prefab that contains components
		for (rapidjson::Value::ConstValueIterator it = value_arr.Begin(); it != value_arr.End(); ++it) {

			// Each value is essentially a container for multiple members
			// IsObject enforces that the member is an object that will contain data:key pairs
			const rapidjson::Value& member = *it;

			DEBUG_ASSERT(member.IsObject(), "Entry does not exist in JSON");
			//stores the data into a stream that is easier to read data from
			std::stringstream stream;

			for (rapidjson::Value::ConstMemberIterator it2 = member.MemberBegin(); it2 != member.MemberEnd(); ++it2) {

				stream << it2->value.GetString() << " ";
			}

			LoadTransition(state_name, stream);
		}
	}
}