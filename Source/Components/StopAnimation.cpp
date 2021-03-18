/**********************************************************************************
*\file         StopAnimation.cpp
*\brief        Contains definition of functions and variables used for
*			   the StopAnimation Component
*
*\author	   Jun Pu, Lee, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Engine/Core.h"
#include "Components/StopAnimation.h"
#include "Manager/ComponentManager.h"

StopAnimation::StopAnimation()
{ }


StopAnimation::~StopAnimation() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<StopAnimation>(Component::GetOwner()->GetID());
}


void StopAnimation::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<StopAnimation>(Component::GetOwner()->GetID(), this);
}

void StopAnimation::DeSerialize(std::stringstream& data) {
	
	data >> name_;
}

void StopAnimation::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {
	
	writer->StartObject();

	writer->Key("component");
	writer->String("StopAnimation");

	writer->Key("stop_anim_name");
	writer->String(name_.c_str());

	writer->EndObject();
}

std::shared_ptr<Component> StopAnimation::Clone() {

	std::shared_ptr<StopAnimation> cloned = std::make_shared<StopAnimation>();
	cloned->name_ = name_;

	return cloned;
}

std::string StopAnimation::Name() const {
	
	return name_;
}