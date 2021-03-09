/**********************************************************************************
*\file         Puzzle.cpp
*\brief        Contains definition of functions and variables used for
*			   the Puzzle Component
*
*\author	   Jun Pu, Lee, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include <string>

#include "Components/Puzzle.h"
#include "Engine/Core.h"
#include "Manager/ComponentManager.h"

Puzzle::Puzzle() : 
	current_{ },
	requirement_{ }
{ }


Puzzle::~Puzzle() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<Puzzle>(Component::GetOwner()->GetID());
}


void Puzzle::Init() {
	
	CORE->GetManager<ComponentManager>()->AddComponent<Puzzle>(Component::GetOwner()->GetID(), this);
}


void Puzzle::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	writer->StartObject();

	writer->Key("component");
	writer->String("Puzzle");

	writer->Key("requirement");
	writer->String(std::to_string(requirement_).c_str());

	writer->EndObject();
}


void Puzzle::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	Serialize(writer);
}


void Puzzle::DeSerialize(std::stringstream& data) {
	
	data >> requirement_;
}


void Puzzle::DeSerializeClone(std::stringstream& data) {
	
	DeSerialize(data);
}


std::shared_ptr<Component> Puzzle::Clone() {

	std::shared_ptr<Puzzle> cloned = std::make_shared<Puzzle>();
	cloned->requirement_ = requirement_;

	return cloned;
}