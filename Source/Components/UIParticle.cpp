/**********************************************************************************
*\file         UIParticle.cpp
*\brief        Contains definition of functions and variables used for
*			   the UIParticle Component
*
*\author	   Jun Pu, Lee, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/UIParticle.h"
#include "Manager/ComponentManager.h"
#include "Engine/Core.h"


UIParticle::UIParticle() :
	alive_{ false },
	lifetime_{ },
	initial_lifetime_{ },
	spawner_{}
{  }


UIParticle::~UIParticle() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<UIParticle>(Component::GetOwner()->GetID());
}


void UIParticle::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<UIParticle>(Component::GetOwner()->GetID(), this);
}


void UIParticle::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("UIParticle");

	writer->EndObject();
}


void UIParticle::DeSerialize(std::stringstream& data) {

	(void)data;
}


std::shared_ptr<Component> UIParticle::Clone() {

	return std::make_shared<UIParticle>();
}


bool UIParticle::IsAlive() const {

	return alive_;
}

void UIParticle::SetAlive(bool status) {

	alive_ = status;
}

void UIParticle::SetDestination(bool status) {

	has_destination_ = status;
}

float UIParticle::GetLifetime() {

	return lifetime_;
}

void UIParticle::SetLifetime(float lifetime) {

	lifetime_ = lifetime;
}