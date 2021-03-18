/**********************************************************************************
*\file         Collectible.cpp
*\brief        Contains definition of functions and variables used for
*			   the Collectible Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/Collectible.h"
#include "MathLib/Vector2D.h"
#include "Manager/ComponentManager.h"
#include "Engine/Core.h"
#include <iostream> 

Collectible::Collectible() :
	item_name_{},
	item_description_{},
	item_type_{ CollectibleType::NONE }
{}

Collectible::~Collectible() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<Collectible>(Component::GetOwner()->GetID());

}

void Collectible::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<Collectible>(Component::GetOwner()->GetID(), this);
}

void Collectible::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Collectible");

	writer->Key("item name");
	writer->String(item_name_.c_str());

	writer->Key("item description");
	writer->String(item_description_.c_str());

	writer->EndObject();
}

void Collectible::DeSerialize(std::stringstream& data) {

	data >> item_name_ >> item_description_;
	item_type_ = StringToCollectible(item_name_);
}

std::shared_ptr<Component> Collectible::Clone() {

	M_DEBUG->WriteDebugMessage("Cloning Inventory Component\n");
	std::shared_ptr<Collectible> cloned = std::make_shared<Collectible>();

	cloned->item_name_ = item_name_;
	cloned->item_description_ = item_description_;
	cloned->item_type_ = item_type_;

	return cloned;
}

const CollectibleType& Collectible::GetItemType() const {

	return item_type_;
}

std::string Collectible::GetItemName() const {

	return item_name_;
}



const CollectibleType StringToCollectible(const std::string& type) {

	if (type == "Key")
		return CollectibleType::KEY;
	else if (type == "Puddle")
		return CollectibleType::PUDDLE;
	else if (type == "Spore")
		return CollectibleType::SPORE;

	else
		return CollectibleType::NONE;
}