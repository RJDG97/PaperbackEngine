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

	batch_size = 2;

	point_light_model_ = CORE->GetManager<ModelManager>()->AddPointLightBatchModel(batch_size, "PointLightModel");
	cone_light_model_ = CORE->GetManager<ModelManager>()->AddConeLightBatchModel(batch_size, "ConeLightModel");

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

	for (PointLightIt it = point_light_arr_->begin(); it != point_light_arr_->end(); ++it) {

		if (!it->second->alive_)
		{
			continue;
		}

		if (debug_) {
			// Log id of entity and it's updated components that are being updated
			M_DEBUG->WriteDebugMessage("Updating entity: " + std::to_string(it->first) + " (Point light position updated)\n");
		}

		UpdateLightPosition(it->second);
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

		UpdateLightPosition(it->second);
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

	glBindVertexArray(cone_light_model_->vaoid_);
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

		BatchConeLight(it->second, cam_zoom);

		if (pos_sent.size() / 4 == batch_size)
		{
			DrawConeLight();
		}
	}

	DrawConeLight();

	glBindVertexArray(point_light_model_->vaoid_);
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

		BatchPointLight(it->second, cam_zoom);

		if (pos_sent.size() / 4 == batch_size)
		{
			DrawPointLight();
		}
	}

	DrawPointLight();

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

void LightingSystem::UpdateLightPosition(PointLight* point_light) {

	Transform* transform =
		component_manager_->GetComponent<Transform>(point_light->GetOwner()->GetID());

	const float global_scale = CORE->GetGlobalScale();

	if (!transform) {

		return;
	}

	Vector2D light_pos = camera_system_->GameCoordsToUI(transform->position_);
	point_light->pos_ = { light_pos.x, light_pos.y };
}

void LightingSystem::UpdateLightPosition(ConeLight* cone_light) {

	Transform* transform =
		component_manager_->GetComponent<Transform>(cone_light->GetOwner()->GetID());

	const float global_scale = CORE->GetGlobalScale();

	if (!transform) {

		return;
	}

	Vector2D light_pos = camera_system_->GameCoordsToUI(transform->position_);
	cone_light->pos_ = { light_pos.x, light_pos.y };
}

void LightingSystem::BatchPointLight(PointLight* point_light, float cam_zoom) {

	if (point_light->radius_ * cam_zoom <= 0.0f || point_light->intensity_ <= 0.0f)
	{
		return;
	}

	for (int i = 0; i < 4; ++i)
	{
		color_sent.push_back(point_light->color_);
		pos_sent.push_back(point_light->pos_);
		intensity_sent.push_back(point_light->intensity_);
		radius_sent.push_back(point_light->radius_ * cam_zoom);
	}
}

void LightingSystem::DrawPointLight() {

	size_t offset = sizeof(glm::vec2) * 4 * batch_size;

	glNamedBufferSubData(point_light_model_->vboid_, offset,
						 sizeof(glm::vec2) * pos_sent.size(),
						 pos_sent.data());

	offset += sizeof(glm::vec2) * 4 * batch_size;

	glNamedBufferSubData(point_light_model_->vboid_, offset,
						 sizeof(glm::vec3) * color_sent.size(),
						 color_sent.data());

	offset += sizeof(glm::vec3) * 4 * batch_size;

	glNamedBufferSubData(point_light_model_->vboid_, offset,
						 sizeof(float) * intensity_sent.size(),
						 intensity_sent.data());

	offset += sizeof(float) * 4 * batch_size;

	glNamedBufferSubData(point_light_model_->vboid_, offset,
						 sizeof(float) * radius_sent.size(),
						 radius_sent.data());

	glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(6 * pos_sent.size() / 4 - 2), GL_UNSIGNED_SHORT, NULL);

	color_sent.clear();
	pos_sent.clear();
	intensity_sent.clear();
	radius_sent.clear();
}

void LightingSystem::BatchConeLight(ConeLight* cone_light, float cam_zoom) {

	if (cone_light->radius_ * cam_zoom <= 0.0f || cone_light->intensity_ <= 0.0f)
	{
		return;
	}

	Child* child = component_manager_->GetComponent<Child>(cone_light->GetOwner()->GetID());
	
	if (!child) {

		return;
	}

	Motion* motion = component_manager_->GetComponent<Motion>(child->ParentID());


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

	for (int i = 0; i < 4; ++i)
	{
		color_sent.push_back(cone_light->color_);
		pos_sent.push_back(cone_light->pos_);
		intensity_sent.push_back(cone_light->intensity_);
		radius_sent.push_back(cone_light->radius_ * cam_zoom);
		direction_sent.push_back(cone_light->direction_);
		angle_sent.push_back(static_cast<float>(cone_light->angle_ / 180 * M_PI));
	}
}

void LightingSystem::DrawConeLight() {

	size_t offset = sizeof(glm::vec2) * 4 * batch_size;

	glNamedBufferSubData(cone_light_model_->vboid_, offset,
						 sizeof(glm::vec2) * pos_sent.size(),
						 pos_sent.data());

	offset += sizeof(glm::vec2) * 4 * batch_size;

	glNamedBufferSubData(cone_light_model_->vboid_, offset,
						 sizeof(glm::vec3) * color_sent.size(),
						 color_sent.data());

	offset += sizeof(glm::vec3) * 4 * batch_size;

	glNamedBufferSubData(cone_light_model_->vboid_, offset,
						 sizeof(float) * intensity_sent.size(),
						 intensity_sent.data());

	offset += sizeof(float) * 4 * batch_size;

	glNamedBufferSubData(cone_light_model_->vboid_, offset,
						 sizeof(float) * radius_sent.size(),
						 radius_sent.data());

	offset += sizeof(float) * 4 * batch_size;

	glNamedBufferSubData(cone_light_model_->vboid_, offset,
						 sizeof(glm::vec2) * direction_sent.size(),
						 direction_sent.data());

	offset += sizeof(glm::vec2) * 4 * batch_size;

	glNamedBufferSubData(cone_light_model_->vboid_, offset,
						 sizeof(float) * angle_sent.size(),
						 angle_sent.data());

	glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(6 * pos_sent.size() / 4 - 2), GL_UNSIGNED_SHORT, NULL);

	color_sent.clear();
	pos_sent.clear();
	intensity_sent.clear();
	radius_sent.clear();
	direction_sent.clear();
	angle_sent.clear();
}

std::string LightingSystem::GetName() {

	return "LightingSystem";
}

void LightingSystem::SendMessageD(Message* m) {

	UNREFERENCED_PARAMETER(m);
}
