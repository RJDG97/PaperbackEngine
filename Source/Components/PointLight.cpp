#include "Components/PointLight.h"
#include "Systems/LightingSystem.h"
#include "Engine/Core.h"
#include "Components/Transform.h"
#include <glm/gtc/type_ptr.hpp>

PointLight::PointLight()
{
}

PointLight::~PointLight()
{
	CORE->GetSystem<LightingSystem>()->RemoveLightComponent(Component::GetOwner()->GetID());
}

void PointLight::Init()
{
	CORE->GetSystem<LightingSystem>()->AddLightComponent(Component::GetOwner()->GetID(), this);
}

void PointLight::Update(float frametime, glm::vec2 cam_pos, glm::vec2 cam_size_)
{
    UNREFERENCED_PARAMETER(frametime);

    Vector2D obj_pos_ = dynamic_cast<Transform*>(GetOwner()->GetComponent(ComponentTypes::TRANSFORM))->position_;
    pos_ = 0.5f * (glm::vec2(obj_pos_.x, obj_pos_.y) + cam_pos + 0.5f * cam_size_);
}

void PointLight::Draw()
{
    shdr_pgm_.Use();
    glBindVertexArray(model_.vaoid_);

    shdr_pgm_.SetUniform("light_color", color_);
    shdr_pgm_.SetUniform("light_center", pos_);
    shdr_pgm_.SetUniform("intensity", intensity_);
    shdr_pgm_.SetUniform("radius", radius_);

    glDrawElements(GL_TRIANGLE_STRIP, model_.draw_cnt_, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(0);
    shdr_pgm_.UnUse();
}

void PointLight::Serialize(std::stringstream& data)
{
	std::string model;
	std::string shdr_pgm;

	data >> model >> shdr_pgm
		 >> color_.x >> color_.y >> color_.z
		 >> radius_ >> intensity_;

	model_ = CORE->GetManager<ModelManager>()->GetModel(model);
	shdr_pgm_ = CORE->GetManager<ShaderManager>()->GetShdrpgm(shdr_pgm);
}
