#include "Systems/LightingSystem.h"
#include "Systems/GraphicsSystem.h"
#include "Engine/Core.h"
#include "Systems/Debug.h"
#include "Entity/ComponentTypes.h"
#include "Entity/ComponentCreator.h"
#include "Entity/Entity.h"
#include "Components/Transform.h"

LightingSystem::LightingSystem()
{
}

LightingSystem::~LightingSystem()
{
}

void LightingSystem::Init()
{
	windows_system = CORE->GetSystem<WindowsSystem>();

	GLint width = windows_system->getWinWidth();
	GLint height = windows_system->getWinHeight();

	CORE->GetManager<ShaderManager>()->AddShdrpgm("Shaders/lighting.vert", "Shaders/lighting.frag", "LightShader");
	//Temporary before camera is component
	GraphicsSystem* graphics_system = CORE->GetSystem<GraphicsSystem>();

	cam_pos_ = &graphics_system->cam_pos_;
	cam_size_ = &graphics_system->cam_size_;

	glGenFramebuffers(1, &lighting_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, lighting_buffer);

	glGenTextures(1, &lighting_texture);
	glBindTexture(GL_TEXTURE_2D, lighting_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width/2, height/2, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lighting_texture, 0);

	assert((glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE,
		   "Lighting framebuffer is not complete!"));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	M_DEBUG->WriteDebugMessage("Lighting System Init\n");
}

void LightingSystem::Update(float frametime)
{
	for (PointLightIt it = point_light_arr_.begin(); it != point_light_arr_.end(); ++it)
	{
		if (debug_) {
			// Log id of entity and it's updated components that are being updated
			M_DEBUG->WriteDebugMessage("Updating entity: " + std::to_string(it->first) + " (Point light position updated)\n");
		}

		UpdateLightPosition(it->second, *cam_pos_, *cam_size_);
	}
}

void LightingSystem::Draw()
{
	//reset the lighting texture
	glBindFramebuffer(GL_FRAMEBUFFER, lighting_buffer);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.1f, 0.05f, 0.2f, 1.0f);
	
	glBlendFunc(GL_ONE, GL_ONE);

	for (PointLightIt it = point_light_arr_.begin(); it != point_light_arr_.end(); ++it)
	{
		if (debug_) {
			// Log id of entity and its updated components that are being updated
			M_DEBUG->WriteDebugMessage("Drawing point light for entity: " + std::to_string(it->first) + "\n");
		}

		DrawPointLight(it->second);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightingSystem::Cleanup()
{
	glDeleteFramebuffers(1, &lighting_buffer);
	glDeleteTextures(1, &lighting_texture);
}

GLuint* LightingSystem::GetLightingTexture()
{
	return &lighting_texture;
}

void LightingSystem::AddLightComponent(EntityID id, PointLight* point_light)
{
	M_DEBUG->WriteDebugMessage("Adding Renderer Component to entity: " + std::to_string(id) + "\n");

	point_light_arr_[id] = point_light;
}

void LightingSystem::RemoveLightComponent(EntityID id)
{
	PointLightIt it = point_light_arr_.find(id);

	if (it != point_light_arr_.end()) {

		M_DEBUG->WriteDebugMessage("Removing Renderer Component from entity: " + std::to_string(id) + "\n");
		point_light_arr_.erase(it);
	}
}

void LightingSystem::UpdateLightPosition(PointLight* point_light, glm::vec2 cam_pos, glm::vec2 cam_size)
{
	Vector2D obj_pos_ = dynamic_cast<Transform*>(
		point_light->GetOwner()->GetComponent(ComponentTypes::TRANSFORM))->position_;
	point_light->pos_ = 0.5f * (glm::vec2(obj_pos_.x, obj_pos_.y) + cam_pos + 0.5f * cam_size);
}

void LightingSystem::DrawPointLight(PointLight* point_light)
{
	point_light->shdr_pgm_.Use();
	glBindVertexArray(point_light->model_.vaoid_);

	point_light->shdr_pgm_.SetUniform("light_color", point_light->color_);
	point_light->shdr_pgm_.SetUniform("light_center", point_light->pos_);
	point_light->shdr_pgm_.SetUniform("intensity", point_light->intensity_);
	point_light->shdr_pgm_.SetUniform("radius", point_light->radius_);

	glDrawElements(GL_TRIANGLE_STRIP, point_light->model_.draw_cnt_, GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(0);
	point_light->shdr_pgm_.UnUse();
}

std::string LightingSystem::GetName()
{
	return "LightingSystem";
}

void LightingSystem::SendMessageD(Message* m)
{
	UNREFERENCED_PARAMETER(m);
}
