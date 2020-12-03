/**********************************************************************************
*\file         Scale.cpp
*\brief        Contains definition of functions and variables used for
*			   the Scale Component
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/Scale.h"
#include "Manager/ComponentManager.h"
#include "Engine/Core.h"
#include "Systems/Debug.h"

Scale::Scale() : scale_{}
{}

Scale::~Scale() {

	CORE->GetManager<ComponentManager>()->RemoveComponent<Scale>(Component::GetOwner()->GetID());
}

void Scale::Init() {
	// Create the map afterwards
	CORE->GetManager<ComponentManager>()->AddComponent<Scale>(Component::GetOwner()->GetID(), this);
}

Vector2D Scale::GetScale() const {
	return scale_;
}

void Scale::SetScale(const Vector2D& new_scale) {
	scale_ = new_scale;
}

void Scale::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("Scale");

	writer->Key("scale");
	writer->String((std::to_string(scale_.x) + " " + std::to_string(scale_.y)).c_str());

	writer->EndObject();
}

void Scale::DeSerialize(std::stringstream& data) {

	data >> scale_.x >> scale_.y;
}

void Scale::DeSerializeClone(std::stringstream& data) {
		
	DeSerialize((data));
}

std::shared_ptr<Component> Scale::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning Scale Component\n");	

	std::shared_ptr<Scale> cloned = std::make_shared<Scale>();

	cloned->scale_ = scale_;

	return cloned;
}