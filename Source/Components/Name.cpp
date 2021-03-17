/**********************************************************************************
*\file         Name.cpp
*\brief        Contains definition of functions and variables used for
*			   the Name Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/Name.h"
#include "Manager/ComponentManager.h"
#include "MathLib/Vector2D.h"
#include "Engine/Core.h"
#include <iostream> 

Name::Name() : 
	name_{ }
{}

Name::~Name() {
	
	CORE->GetManager<ComponentManager>()->RemoveComponent<Name>(Component::GetOwner()->GetID());
}

void Name::Init() {
	
	CORE->GetManager<ComponentManager>()->AddComponent<Name>(Component::GetOwner()->GetID(), this);
}

void Name::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Name");

	writer->Key("name");
	writer->String(name_.c_str());

	writer->EndObject();
}

void Name::DeSerialize(std::stringstream& data) {
	
	data >> name_;
}

std::string Name::GetEntityName() const {

	return name_;
}

std::shared_ptr<Component> Name::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning Name Component\n");
	std::shared_ptr<Name> cloned = std::make_shared<Name>();

	cloned->name_ = name_;

	return cloned;
}

std::string Name::GetName()
{
	return name_;
}
