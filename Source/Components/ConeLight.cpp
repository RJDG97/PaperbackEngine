#include "Components/PointLight.h"
#include "Manager/ComponentManager.h"
#include "Systems/LightingSystem.h"
#include "Engine/Core.h"
#include "Components/Transform.h"
#include <glm/gtc/type_ptr.hpp>

ConeLight::ConeLight() {

}

ConeLight::~ConeLight() {

	//CORE->GetSystem<LightingSystem>()->RemoveLightComponent(Component::GetOwner()->GetID());
	CORE->GetManager<ComponentManager>()->RemoveComponent<ConeLight>(Component::GetOwner()->GetID());
}

void ConeLight::Init() {

	//CORE->GetSystem<LightingSystem>()->AddLightComponent(Component::GetOwner()->GetID(), this);
	CORE->GetManager<ComponentManager>()->AddComponent<ConeLight>(Component::GetOwner()->GetID(), this);
}

void ConeLight::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {

	writer->StartObject();

	writer->Key("component");
	writer->String("ConeLight");

	writer->Key("color");
	writer->String((std::to_string(color_.x) + " " +
		std::to_string(color_.y) + " " +
		std::to_string(color_.y)).c_str());

	writer->Key("radius");
	writer->String(std::to_string(radius_).c_str());

	writer->Key("intensity");
	writer->String(std::to_string(intensity_).c_str());

	writer->Key("angle");
	writer->String(std::to_string(angle_).c_str());

	writer->EndObject();
}

void ConeLight::DeSerialize(std::stringstream& data) {

	data >> angle_
		 >> color_.x >> color_.y >> color_.z
		 >> radius_ >> intensity_;
}

void ConeLight::DeSerializeClone(std::stringstream& data) {

	DeSerialize(data);
}

std::shared_ptr<Component> ConeLight::Clone() {
	M_DEBUG->WriteDebugMessage("Cloning PointLight Component\n");

	std::shared_ptr<ConeLight> cloned = std::make_shared<ConeLight>();

	cloned->color_ = color_;
	cloned->intensity_ = intensity_;
	cloned->model_ = model_;
	cloned->shdr_pgm_ = shdr_pgm_;
	cloned->radius_ = radius_;
	cloned->angle_ = angle_;

	return cloned;
}

float ConeLight::GetRadius() {

	return radius_;
}

float ConeLight::GetIntensity() {

	return intensity_;
}

void ConeLight::SetRadius(float new_radius) {

	radius_ = new_radius;
}

void ConeLight::SetIntensity(float new_intensity) {

	intensity_ = new_intensity;
}
