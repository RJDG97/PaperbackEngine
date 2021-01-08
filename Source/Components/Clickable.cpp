/**********************************************************************************
*\file         Clickable.cpp
*\brief        Contains definition of functions and variables used for
*			   the Clickable Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/Clickable.h"
#include "Engine/Core.h"
#include "Manager/ComponentManager.h"
#include "Systems/Debug.h"
#include "Systems/Collision.h"
#include <iostream>

Clickable::Clickable() : 
	scale_{},
	top_right_{},
	bottom_left_{},
	collided_{ false },
	active_{ true },
	index_{},
	group_{}
{}

size_t Clickable::GetIndex() {

	return index_;
}

bool Clickable::GetActive() {
	
	return active_;
}

Clickable::~Clickable() {
	//CORE->GetSystem<Collision>()->RemoveClickableComponent(Component::GetOwner()->GetID());
	CORE->GetManager<ComponentManager>()->RemoveComponent<Clickable>(Component::GetOwner()->GetID());
}

void Clickable::Init() {

	//CORE->GetSystem<Collision>()->AddClickableComponent(Component::GetOwner()->GetID(), this);
	CORE->GetManager<ComponentManager>()->AddComponent<Clickable>(Component::GetOwner()->GetID(), this);
}

void Clickable::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Clickable");

	writer->Key("scale");
	writer->String((std::to_string(scale_.x) + " " + std::to_string(scale_.y)).c_str());

	writer->EndObject();
}

void Clickable::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Clickable");

	writer->Key("index");
	writer->String(std::to_string(index_).c_str());
	
	writer->Key("active");
	writer->String(std::to_string(active_).c_str());

	writer->Key("group");
	writer->String(std::to_string(group_).c_str());

	writer->EndObject();
}

void Clickable::DeSerialize(std::stringstream& data) {
	// Not required since it's going to be computed
	std::cout << "Serializing Clickable Component" << std::endl;
	data >> scale_.x >> scale_.y >> active_ >> group_;
}

void Clickable::DeSerializeClone(std::stringstream& data) {
	
	data >> index_ >> active_ >> scale_.x >> scale_.y >> group_;
}

std::shared_ptr<Component> Clickable::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning Clickable Component\n");
	
	std::shared_ptr<Clickable> cloned = std::make_shared<Clickable>();

	cloned->bottom_left_ = bottom_left_;
	cloned->top_right_ = top_right_;
	cloned->active_ = active_;
	cloned->scale_ = scale_;
	cloned->group_ = group_;

	return cloned;
}