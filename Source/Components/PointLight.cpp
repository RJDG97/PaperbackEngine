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
    Vector2D obj_pos_ = dynamic_cast<Transform*>(GetOwner()->GetComponent(ComponentTypes::TRANSFORM))->position_;
    pos_ = 0.5f * (glm::vec2(obj_pos_.x, obj_pos_.y) + cam_pos + 0.5f * cam_size_);
}

void PointLight::Draw()
{
    shdr_pgm_.Use();
    glBindVertexArray(model_.vaoid_);

    GLint uniform_var_light_color =
        glGetUniformLocation(shdr_pgm_.GetHandle(), "light_color");

    if (uniform_var_light_color >= 0) {
        glUniform3fv(uniform_var_light_color, 1, glm::value_ptr(color_));
    }

    else {
        std::cout << "Uniform variable doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
    }

    GLint uniform_var_light_center =
        glGetUniformLocation(shdr_pgm_.GetHandle(), "light_center");

    if (uniform_var_light_center >= 0) {
        glUniform2fv(uniform_var_light_center, 1, glm::value_ptr(pos_));
    }

    else {
        std::cout << "Uniform variable doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
    }

    GLint uniform_var_intensity =
        glGetUniformLocation(shdr_pgm_.GetHandle(), "intensity");

    if (uniform_var_intensity >= 0) {
        glUniform1f(uniform_var_intensity, intensity_);
    }

    else {
        std::cout << "Uniform variable doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
    }

    GLint uniform_var_radius =
        glGetUniformLocation(shdr_pgm_.GetHandle(), "radius");

    if (uniform_var_radius >= 0) {
        glUniform1f(uniform_var_radius, radius_);
    }

    else {
        std::cout << "Uniform variable doesn't exist!!!\n";
        std::exit(EXIT_FAILURE);
    }

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
