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

void PointLight::Serialize(std::stringstream& data) {

	std::string model;
	std::string shdr_pgm;

	data >> model >> shdr_pgm
		 >> color_.x >> color_.y >> color_.z
		 >> radius_ >> intensity_;

	model_ = *CORE->GetManager<ModelManager>()->GetModel(model);
	shdr_pgm_ = *CORE->GetManager<ShaderManager>()->GetShdrpgm(shdr_pgm);
}
