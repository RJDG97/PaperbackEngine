#include "Components/PointLight.h"
#include "Systems/LightingSystem.h"
#include "Engine/Core.h"
#include "Components/Transform.h"
#include <glm/gtc/type_ptr.hpp>

PointLight::PointLight() {

}

PointLight::~PointLight() {

	CORE->GetSystem<LightingSystem>()->RemoveLightComponent(Component::GetOwner()->GetID());
}

void PointLight::Init() {

	CORE->GetSystem<LightingSystem>()->AddLightComponent(Component::GetOwner()->GetID(), this);
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

	std::string model;
	std::string shdr_pgm;

	data >> model >> shdr_pgm
		 >> color_.x >> color_.y >> color_.z
		 >> radius_ >> intensity_;

	model_ = *CORE->GetManager<ModelManager>()->GetModel(model);
	shdr_pgm_ = *CORE->GetManager<ShaderManager>()->GetShdrpgm(shdr_pgm);
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
