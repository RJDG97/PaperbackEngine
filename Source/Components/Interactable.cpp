/**********************************************************************************
*\file         Interactable.cpp
*\brief        Contains definition of functions and variables used for
*			   the Interactable Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/Interactable.h"
#include "MathLib/Vector2D.h"
#include "Manager/ComponentManager.h"
#include "Engine/Core.h"
#include <iostream> 

Interactable::Interactable() :
	event_animations_{ }
{}

Interactable::~Interactable() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<Interactable>(Component::GetOwner()->GetID());

}

void Interactable::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<Interactable>(Component::GetOwner()->GetID(), this);
}

void Interactable::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Interactable");

	writer->Key("size");
	writer->String( std::to_string(event_animations_.size()).c_str() );

	for (auto& [event, anim_name] : event_animations_) {
		
		writer->Key("event");
		writer->String(event.c_str());
		writer->Key("animation");
		writer->String(anim_name.c_str());
	}

	writer->EndObject();
}

void Interactable::DeSerialize(std::stringstream& data) {

	(void)data;
}

void Interactable::DeSerializeClone(std::stringstream& data) {

	size_t sz{};
	std::string key{}, val{};
	data >> sz;

	for (size_t i = 0; i < sz; ++i) {

		data >> key >> val;
		event_animations_[key] = val;
	}
}

std::shared_ptr<Component> Interactable::Clone() {

	M_DEBUG->WriteDebugMessage("Cloning Inventory Component\n");
	std::shared_ptr<Interactable> cloned = std::make_shared<Interactable>();

	return cloned;
}

std::string Interactable::GetAnimationName(const std::string& event) {

	auto event_it = event_animations_.find(event);

	if (event_it != event_animations_.end()) {
		
		return event_it->second;
	}

	return {};
}