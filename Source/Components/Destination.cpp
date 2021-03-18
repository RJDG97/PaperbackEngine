/**********************************************************************************
*\file         Destination.cpp
*\brief        Contains definition of functions and variables used for
*			   the Destination Component
*
*\author	   Jun Pu, Lee, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Engine/Core.h"
#include "Manager/ComponentManager.h"
#include "Components\Destination.h"

Destination::Destination() 
{ }

Destination::~Destination() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<Destination>(Component::GetOwner()->GetID());
}

void Destination::Init() {

	CORE->GetManager<ComponentManager>()->AddComponent<Destination>(Component::GetOwner()->GetID(), this);
}

void Destination::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Destination");

	writer->Key("destination");
	writer->String((std::to_string(destination_.x) + " " + std::to_string(destination_.y)).c_str());

	writer->Key("is_ui_layer");
	writer->String(std::to_string(ui_layer_).c_str());

	writer->EndObject();
}

void Destination::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	Serialize(writer);
}

void Destination::DeSerialize(std::stringstream& data) {

	data >> destination_.x >> destination_.y >> ui_layer_;
}

void Destination::DeSerializeClone(std::stringstream& data) {

	DeSerialize(data);
}

std::shared_ptr<Component> Destination::Clone() {

	std::shared_ptr<Destination> cloned = std::make_shared<Destination>();

	cloned->destination_ = destination_;
	cloned->ui_layer_ = ui_layer_;

	return cloned;
}

const Vector2D& Destination::GetDestination() const {

	return destination_;
}