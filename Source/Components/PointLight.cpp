/**********************************************************************************
*\file         PointLight.cpp
*\brief        Contains definition of functions and variables used for
*			   the PointLight Component
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Components/PointLight.h"
#include "Manager/ComponentManager.h"
#include "Systems/LightingSystem.h"
#include "Engine/Core.h"
#include "Components/Transform.h"
#include <glm/gtc/type_ptr.hpp>

PointLight::PointLight() {

}

PointLight::~PointLight() {

	//CORE->GetSystem<LightingSystem>()->RemoveLightComponent(Component::GetOwner()->GetID());
	CORE->GetManager<ComponentManager>()->RemoveComponent<PointLight>(Component::GetOwner()->GetID());
}

void PointLight::Init() {

	//CORE->GetSystem<LightingSystem>()->AddLightComponent(Component::GetOwner()->GetID(), this);
	CORE->GetManager<ComponentManager>()->AddComponent<PointLight>(Component::GetOwner()->GetID(), this);
	CORE->GetSystem<LightingSystem>()->RandomizePulse(this);
}

void PointLight::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("PointLight");

	writer->Key("color");
	writer->String((std::to_string(color_.x) + " " +
					std::to_string(color_.y) + " " +
					std::to_string(color_.y)).c_str());

	writer->Key("radius");
	writer->String(std::to_string(radius_).c_str());

	writer->Key("intensity");
	writer->String(std::to_string(intensity_).c_str());

	writer->Key("alive");
	writer->String(std::to_string(alive_).c_str());

	writer->EndObject();
}

void PointLight::DeSerialize(std::stringstream& data) {

	data >> color_.x >> color_.y >> color_.z
		 >> radius_ >> intensity_ >> alive_
		 >> pulse_ >> max_radius_ >> min_radius_
		 >> max_intensity_ >> min_intensity_ >> cycle_duration_;
}

void PointLight::DeSerializeClone(std::stringstream& data) {

	DeSerialize(data);
}

void PointLight::SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

    Serialize(writer);
}

std::shared_ptr<Component> PointLight::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning PointLight Component\n");

	std::shared_ptr<PointLight> cloned = std::make_shared<PointLight>();

	cloned->color_ = color_;
	cloned->intensity_ = intensity_;
	cloned->model_ = model_;
	cloned->shdr_pgm_ = shdr_pgm_;
	cloned->radius_ = radius_;
	cloned->alive_ = alive_;
	cloned->pulse_ = pulse_;
	cloned->max_radius_ = max_radius_;
	cloned->min_radius_ = min_radius_;
	cloned->max_intensity_ = max_intensity_;
	cloned->min_intensity_ = min_intensity_;
	cloned->cycle_duration_ = cycle_duration_;

	return cloned;
}

float PointLight::GetRadius(){

	return radius_;
}

float PointLight::GetIntensity(){

	return intensity_;
}

glm::vec3 PointLight::GetColor() {

	return color_;
}

bool PointLight::GetAlive()
{
	return alive_;
}

void PointLight::SetRadius(float new_radius){

	radius_ = new_radius;
}

void PointLight::SetIntensity(float new_intensity){

	intensity_ = new_intensity;

}
void PointLight::SetColor(glm::vec3 new_color) {

	color_ = new_color;
}

void PointLight::SetAlive(bool alive)
{
	alive_ = alive;
}
