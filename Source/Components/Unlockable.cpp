/**********************************************************************************
*\file         Unlockable.cpp
*\brief        Contains definition of functions and variables used for
*			   the Unlockable Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/Unlockable.h"
#include "MathLib/Vector2D.h"
#include "Manager/ComponentManager.h"
#include "Engine/Core.h"
#include <iostream> 

Unlockable::Unlockable() : 
	count_{}
{}

Unlockable::~Unlockable() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<Unlockable>(Component::GetOwner()->GetID());

}

void Unlockable::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<Unlockable>(Component::GetOwner()->GetID(), this);
}

void Unlockable::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Collectible");

	writer->Key("count");
	writer->String(std::to_string(count_).c_str());

	for (size_t i = 0; i < count_; ++i) {

		writer->Key("item name");
		writer->String(required_items_[i].c_str());
	}

	writer->EndObject();
}

void Unlockable::DeSerialize(std::stringstream& data) {

	// Check whether to change type
	float count;
	data >> count;
	count_ = static_cast<size_t>(count);

	for (size_t i = 0; i < count_; ++i) {
		
		std::string name;
		data >> name;

		required_items_.push_back(name);
	}
}

std::shared_ptr<Component> Unlockable::Clone() {

	M_DEBUG->WriteDebugMessage("Cloning Inventory Component\n");
	std::shared_ptr<Unlockable> cloned = std::make_shared<Unlockable>();

	cloned->count_ = count_;
	cloned->required_items_ = required_items_;

	return cloned;
}

const std::vector<std::string>& Unlockable::GetRequiredItems() const {

	return required_items_;
}
