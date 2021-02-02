/**********************************************************************************
*\file         Inventory.cpp
*\brief        Contains definition of functions and variables used for
*			   the Inventory Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/Inventory.h"
#include "MathLib/Vector2D.h"
#include "Manager/ComponentManager.h"
#include "Engine/Core.h"
#include <iostream> 

Inventory::Inventory() :
	current_capacity_{},
	max_capacity_{},
	inventory_{}
{}

Inventory::~Inventory() {

	// Wtf somehow there's a mem allocation somewhere o.o
	inventory_.clear();
	CORE->GetManager<ComponentManager>()->RemoveComponent<Inventory>(Component::GetOwner()->GetID());
}

void Inventory::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<Inventory>(Component::GetOwner()->GetID(), this);
}

void Inventory::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Inventory");

	writer->EndObject();
}

void Inventory::DeSerialize(std::stringstream& data) {
	
	data >> max_capacity_;
}

std::shared_ptr<Component> Inventory::Clone() {

	M_DEBUG->WriteDebugMessage("Cloning Inventory Component\n");
	std::shared_ptr<Inventory> cloned = std::make_shared<Inventory>();

	//cloned->current_capacity_ = current_capacity_;
	//cloned->max_capacity_ = max_capacity_;
	//cloned->inventory_ = inventory_;

	return cloned;
}

void Inventory::InsertItem(const Collectible& item) {

	auto it = inventory_.find(item.item_name_);

	// Item did not exist previously in inventory
	if (it == inventory_.end() && 
		((current_capacity_ + 1) <= max_capacity_)) {
		
		// After picking up an item, increment inventory counter
		++current_capacity_;

		inventory_[item.item_name_] = { 1, item.item_description_ };
	}
	// Item already existed before
	else {
		
		// Increment duplicate item counter
		++inventory_[item.item_name_].count_;
	}

	inventory_[item.item_name_].entities_.push_back(item.GetOwner()->GetID());
}

void Inventory::RemoveItem(const ItemName& name) {

	auto it = inventory_.find(name);

	if (it != inventory_.end()) {
		
		// Decrement item count and remove the first instance of the item picked up
		--inventory_[name].count_;
		inventory_[name].entities_.erase( inventory_[name].entities_.begin() );

		// If no more of the same item remains after removing, erase it
		if (inventory_[name].count_ == 0) {
			
			--current_capacity_;
			inventory_.erase(name);
		}
	}
}

bool Inventory::HasItem(const std::string& name) {

	auto it = inventory_.find(name);

	if (it != inventory_.end())
		return true;

	return false;
}

std::unordered_map<std::string, ItemDescription> Inventory::GetInventory()
{
	return inventory_;
}

size_t Inventory::GetInventorySize()
{
	return current_capacity_;
}
