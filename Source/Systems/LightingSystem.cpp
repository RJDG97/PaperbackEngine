/**********************************************************************************
*\file         LightingSystem.cpp
*\brief        Contains definition of functions and variables used for
*			   the Lighting System
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#define _USE_MATH_DEFINES

#include "Systems/LightingSystem.h"
#include "Systems/GraphicsSystem.h"
#include "Engine/Core.h"
#include "Systems/Debug.h"
#include "Entity/ComponentTypes.h"
#include "Entity/ComponentCreator.h"
#include "Entity/Entity.h"
#include "Components/Transform.h"

LightingSystem::LightingSystem() {

}

LightingSystem::~LightingSystem() {

}

void LightingSystem::Init() {

	component_manager_ = &*CORE->GetManager<ComponentManager>();

	point_light_arr_ = component_manager_->GetComponentArray<PointLight>();
	cone_light_arr_ = component_manager_->GetComponentArray<ConeLight>();

	windows_system_ = &*CORE->GetSystem<WindowsSystem>();
	camera_system_ = &*CORE->GetSystem<CameraSystem>();
	graphics_system_ = &*CORE->GetSystem<GraphicsSystem>();

	win_size_.x = static_cast<float>(windows_system_->GetWinWidth());
	win_size_.y = static_cast<float>(windows_system_->GetWinHeight());

	TextureManager* texture_manager_ = &*CORE->GetManager<TextureManager>();
	texture_manager_->LoadMiscTextures();
	darkness_texture = texture_manager_->GetTexture("DarknessTexture")->GetTilesetHandle();

	ShaderManager* shader_manager = &*CORE->GetManager<ShaderManager>();
	lighting_shaders_["PointLightShader"] = shader_manager->AddShdrpgm("Shaders/point_light.vert",
																	   "Shaders/point_light.frag",
																	   "PointLightShader");

	lighting_shaders_["ConeLightShader"] = shader_manager->AddShdrpgm("Shaders/cone_light.vert",
																	  "Shaders/cone_light.frag",
																	  "ConeLightShader");


	light_model_ = CORE->GetManager<ModelManager>()->AddTristripsModel(1, 1, "LightModel");

	//Temporary before camera is component
	std::shared_ptr<GraphicsSystem> graphics_system = CORE->GetSystem<GraphicsSystem>();

	glGenFramebuffers(1, &lighting_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, lighting_buffer);

	glGenTextures(1, &lighting_texture);
	glBindTexture(GL_TEXTURE_2D, lighting_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				 static_cast<GLsizei>(win_size_.x), static_cast<GLsizei>(win_size_.y),
				 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lighting_texture, 0);

	assert((glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE,
		   "Lighting framebuffer is not complete!"));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(1, &addition_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, addition_buffer);

	glGenTextures(1, &addition_texture);
	glBindTexture(GL_TEXTURE_2D, addition_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				 static_cast<GLsizei>(win_size_.x), static_cast<GLsizei>(win_size_.y),
				 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, addition_texture, 0);

	assert((glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE,
		"Addition framebuffer is not complete!"));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	M_DEBUG->WriteDebugMessage("Lighting System Init\n");

}

void LightingSystem::Update(float frametime) {

	if (camera_system_->GetMainCamera() == nullptr)
	{
		return;
	}

	UNREFERENCED_PARAMETER(frametime);

	float cam_zoom = (*camera_system_->GetMainCamera()->GetCameraZoom());
	glm::vec2 cam_pos = (*camera_system_->GetMainCamera()->GetCameraPosition());

	for (PointLightIt it = point_light_arr_->begin(); it != point_light_arr_->end(); ++it) {

		if (!it->second->alive_)
		{
			continue;
		}

		if (debug_) {
			// Log id of entity and it's updated components that are being updated
			M_DEBUG->WriteDebugMessage("Updating entity: " + std::to_string(it->first) + " (Point light position updated)\n");
		}

		UpdateLightPosition(it->second, cam_zoom, cam_pos);
	}

	for (ConeLightIt it = cone_light_arr_->begin(); it != cone_light_arr_->end(); ++it) {

		if (!it->second->alive_)
		{
			continue;
		}

		if (debug_) {
			// Log id of entity and it's updated components that are being updated
			M_DEBUG->WriteDebugMessage("Updating entity: " + std::to_string(it->first) + " (Point light position updated)\n");
		}

		UpdateLightPosition(it->second, cam_zoom, cam_pos);
	}
}

void LightingSystem::Draw() {

	if (camera_system_->GetMainCamera() == nullptr)
	{
		return;
	}
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glBindFramebuffer(GL_FRAMEBUFFER, addition_buffer);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	float cam_zoom = (*camera_system_->GetMainCamera()->GetCameraZoom());
	
	Shader* point_light_shader = lighting_shaders_["PointLightShader"];
	Shader* cone_light_shader = lighting_shaders_["ConeLightShader"];

	glBindVertexArray(light_model_->vaoid_);
	cone_light_shader->Use();

	for (ConeLightIt it = cone_light_arr_->begin(); it != cone_light_arr_->end(); ++it) {

		if (!it->second->alive_)
		{
			continue;
		}

		if (debug_) {
			// Log id of entity and its updated components that are being updated
			M_DEBUG->WriteDebugMessage("Drawing cone light for entity: " + std::to_string(it->first) + "\n");
		}

		DrawConeLight(cone_light_shader, it->second, cam_zoom);
	}

	point_light_shader->Use();

	for (PointLightIt it = point_light_arr_->begin(); it != point_light_arr_->end(); ++it) {

		if (!it->second->alive_)
		{
			continue;
		}

		if (debug_) {
			// Log id of entity and its updated components that are being updated
			M_DEBUG->WriteDebugMessage("Drawing point light for entity: " + std::to_string(it->first) + "\n");
		}

		DrawPointLight(point_light_shader, it->second, cam_zoom);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, lighting_buffer);
	glClear(GL_COLOR_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	graphics_system_->DrawFinalTexture(&darkness_texture, 1.0f);
	glBlendFunc(GL_ONE, GL_ONE);
	graphics_system_->DrawFinalTexture(&addition_texture, 1.0f);
}

void LightingSystem::Cleanup() {

	glDeleteFramebuffers(1, &lighting_buffer);
	glDeleteTextures(1, &lighting_texture);
}

GLuint* LightingSystem::GetLightingTexture() {

	return &lighting_texture;
}

GLuint* LightingSystem::GetAdditionTexture() {

	return &addition_texture;
}

void LightingSystem::UpdateLightPosition(PointLight* point_light, float cam_zoom, glm::vec2 cam_pos) {

	Transform* transform =
		component_manager_->GetComponent<Transform>(point_light->GetOwner()->GetID());

	const float global_scale = CORE->GetGlobalScale();

	if (!transform) {

		return;
	}

	Vector2D obj_pos_ = transform->position_;

	point_light->pos_ = glm::vec2(obj_pos_.x * global_scale, obj_pos_.y * global_scale) * cam_zoom +
						(cam_pos * cam_zoom + 0.5f * win_size_);
}

void LightingSystem::UpdateLightPosition(ConeLight* cone_light, float cam_zoom, glm::vec2 cam_pos) {

	Transform* transform =
		component_manager_->GetComponent<Transform>(cone_light->GetOwner()->GetID());

	const float global_scale = CORE->GetGlobalScale();

	if (!transform) {

		return;
	}

	Vector2D obj_pos_ = transform->position_;

	cone_light->pos_ = glm::vec2(obj_pos_.x * global_scale, obj_pos_.y * global_scale) * cam_zoom +
					   (cam_pos * cam_zoom + 0.5f * win_size_);
}

void LightingSystem::DrawPointLight(Shader* shader, PointLight* point_light, float cam_zoom) {

	shader->SetUniform("light_color", point_light->color_);
	shader->SetUniform("light_center", point_light->pos_);
	shader->SetUniform("intensity", point_light->intensity_);
	shader->SetUniform("radius", point_light->radius_ * cam_zoom);

	glDrawElements(GL_TRIANGLE_STRIP, light_model_->draw_cnt_, GL_UNSIGNED_SHORT, NULL);
}

void LightingSystem::DrawConeLight(Shader* shader, ConeLight* cone_light, float cam_zoom) {

	Motion* motion =
		component_manager_->GetComponent<Motion>(cone_light->GetOwner()->GetID());

	if (motion == nullptr)
	{
		M_DEBUG->WriteDebugMessage("Cannot draw cone light as entity has no motion component");
	}

	else
	{
		Vector2D direction = motion->GetVelocity();

		if (Vector2DLength(direction) > 0.0f)
		{
			cone_light->direction_ = glm::vec2{ direction.x, direction.y };
		}
	}

	Vector2D direction = motion->GetVelocity();
	shader->SetUniform("light_color", cone_light->color_);
	shader->SetUniform("light_center", cone_light->pos_);
	shader->SetUniform("intensity", cone_light->intensity_);
	shader->SetUniform("radius", cone_light->radius_ * cam_zoom);
	shader->SetUniform("direction", cone_light->direction_);
	shader->SetUniform("angle", static_cast<float>(cone_light->angle_ / 180 * M_PI));

	glDrawElements(GL_TRIANGLE_STRIP, light_model_->draw_cnt_, GL_UNSIGNED_SHORT, NULL);
}

std::string LightingSystem::GetName() {

	return "LightingSystem";
}

void LightingSystem::SendMessageD(Message* m) {

	UNREFERENCED_PARAMETER(m);
}
