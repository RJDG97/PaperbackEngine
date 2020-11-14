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

	ComponentManager* comp_mgr = &*CORE->GetManager<ComponentManager>();

	point_light_arr_ = comp_mgr->GetComponentArray<PointLight>();
	transform_arr_ = comp_mgr->GetComponentArray<Transform>();

	windows_system_ = &*CORE->GetSystem<WindowsSystem>();
	camera_system_ = &*CORE->GetSystem<CameraSystem>();

	win_size_.x = static_cast<float>(windows_system_->GetWinWidth());
	win_size_.y = static_cast<float>(windows_system_->GetWinHeight());

	lighting_shaders_["PointLightShader"] = CORE->GetManager<ShaderManager>()->AddShdrpgm("Shaders/point_light.vert",
																						  "Shaders/point_light.frag",
																						  "PointLightShader");
	
	light_model_ = CORE->GetManager<ModelManager>()->AddTristripsModel(1, 1, "LightModel");

	//Temporary before camera is component
	std::shared_ptr<GraphicsSystem> graphics_system = CORE->GetSystem<GraphicsSystem>();

	cam_pos_ = &camera_system_->cam_pos_;
	cam_zoom_ = &camera_system_->cam_zoom_;

	glGenFramebuffers(1, &lighting_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, lighting_buffer);

	glGenTextures(1, &lighting_texture);
	glBindTexture(GL_TEXTURE_2D, lighting_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				 static_cast<GLsizei>(win_size_.x/2), static_cast<GLsizei>(win_size_.y/2),
				 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lighting_texture, 0);

	assert((glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE,
		   "Lighting framebuffer is not complete!"));

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	M_DEBUG->WriteDebugMessage("Lighting System Init\n");
}

void LightingSystem::Update(float frametime) {

	UNREFERENCED_PARAMETER(frametime);
	for (PointLightIt it = point_light_arr_->begin(); it != point_light_arr_->end(); ++it) {

		if (debug_) {
			// Log id of entity and it's updated components that are being updated
			M_DEBUG->WriteDebugMessage("Updating entity: " + std::to_string(it->first) + " (Point light position updated)\n");
		}

		UpdateLightPosition(it->second);
	}
}

void LightingSystem::Draw() {

	//reset the lighting texture
	glBindFramebuffer(GL_FRAMEBUFFER, lighting_buffer);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.3f, 0.25f, 0.4f, 1.0f);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBlendFunc(GL_ONE, GL_ONE);
	
	Shader* point_light_shader = lighting_shaders_["PointLightShader"];
	glBindVertexArray(light_model_->vaoid_);
	point_light_shader->Use();

	for (PointLightIt it = point_light_arr_->begin(); it != point_light_arr_->end(); ++it) {

		if (debug_) {
			// Log id of entity and its updated components that are being updated
			M_DEBUG->WriteDebugMessage("Drawing point light for entity: " + std::to_string(it->first) + "\n");
		}

		DrawPointLight(point_light_shader, it->second);
	}
}

void LightingSystem::Cleanup() {

	glDeleteFramebuffers(1, &lighting_buffer);
	glDeleteTextures(1, &lighting_texture);
}

GLuint* LightingSystem::GetLightingTexture() {

	return &lighting_texture;
}

void LightingSystem::UpdateLightPosition(PointLight* point_light) {

	//if (point_light)
	//ComponentManager* comp_mgr = &*CORE->GetManager<ComponentManager>();

	Transform* xform = transform_arr_->GetComponent(point_light->GetOwner()->GetID());
	const float global_scale = CORE->GetGlobalScale();

	if (!xform) {

		return;
	}

	Vector2D obj_pos_ = xform->position_;

	point_light->pos_ = glm::vec2(obj_pos_.x * global_scale, obj_pos_.y * global_scale) * *cam_zoom_ +
							(*cam_pos_ * *cam_zoom_ + 0.5f * win_size_);
	point_light->pos_ *= 0.5f;
}

void LightingSystem::DrawPointLight(Shader* shader, PointLight* point_light) {

	shader->SetUniform("light_color", point_light->color_);
	shader->SetUniform("light_center", point_light->pos_);
	shader->SetUniform("intensity", point_light->intensity_);
	shader->SetUniform("radius", point_light->radius_ * camera_system_->cam_zoom_);

	glDrawElements(GL_TRIANGLE_STRIP, light_model_->draw_cnt_, GL_UNSIGNED_SHORT, NULL);
}

std::string LightingSystem::GetName() {

	return "LightingSystem";
}

void LightingSystem::SendMessageD(Message* m) {

	UNREFERENCED_PARAMETER(m);
}
