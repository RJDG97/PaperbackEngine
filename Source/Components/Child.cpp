/**********************************************************************************
*\file         Child.cpp
*\brief        Contains definition of functions and variables used for
*			   the Child Component
*
*\author	   Jun Pu, Lee, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Engine/Core.h"
#include "Systems/Factory.h"
#include "Systems/Debug.h"
#include "Components/Child.h"

void Child::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<Child>(Component::GetOwner()->GetID(), this);
}

Child::~Child() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<Child>(Component::GetOwner()->GetID());
}


void Child::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Child");

	writer->Key("parent_name");
	writer->String(parent_name_.c_str());

	writer->EndObject();
}


void Child::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	Serialize(writer); // Required because of potential ""
}


void Child::DeSerialize(std::stringstream& data) {

	data >> parent_name_;
}

void Child::DeSerializeClone(std::stringstream& data) {

	DeSerialize(data);
}

std::shared_ptr<Component> Child::Clone() {

	std::shared_ptr<Child> cloned = std::make_shared<Child>();
	cloned->parent_name_ = parent_name_;

	return cloned;
}

std::string Child::ParentName() const {

	return parent_name_;
}