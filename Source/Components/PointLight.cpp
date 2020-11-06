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
}

void PointLight::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	(void)writer;
	/*
	writer->StartObject();

	writer->Key("component");
	writer->String("AABB");

	writer->Key("scale");
	writer->String((std::to_string(scale_.x) + " " + std::to_string(scale_.y)).c_str());

	writer->EndObject();*/
}

void PointLight::DeSerialize(std::stringstream& data) {

	data >> color_.x >> color_.y >> color_.z
		 >> radius_ >> intensity_;
}

std::shared_ptr<Component> PointLight::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning PointLight Component\n");

	std::shared_ptr<PointLight> cloned = std::make_shared<PointLight>();

	cloned->color_ = color_;
	cloned->intensity_ = intensity_;
	cloned->model_ = model_;
	cloned->shdr_pgm_ = shdr_pgm_;
	cloned->radius_ = radius_;

	return cloned;
}

float PointLight::GetRadius(){

	return radius_;
}

float PointLight::GetIntensity(){

	return intensity_;
}

void PointLight::SetRadius(float newRadius){

	radius_ = newRadius;
}

void PointLight::SetIntensity(float newIntensity){

	intensity_ = newIntensity;
}
