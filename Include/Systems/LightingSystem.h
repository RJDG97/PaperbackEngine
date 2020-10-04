#pragma once

#ifndef LIGHTINGSYSTEM_H
#define LIGHTINGSYSTEM_H

#include <GL/glew.h>
#include "Systems/ISystem.h"
#include "Systems/WindowsSystem.h"
#include "Components/PointLight.h"
#include <unordered_map>

class LightingSystem : public ISystem
{
	bool debug_;

	GLuint lighting_buffer;
	GLuint lighting_texture;

	glm::vec2* cam_pos_;
	glm::vec2* cam_size_;

	WindowsSystem* windows_system;

public:

	LightingSystem();
	~LightingSystem();

	void Init();
	void Update(float frametime);
	void Draw();
	void Cleanup();
	GLuint* GetLightingTexture();

	using PointLightIt = std::unordered_map<EntityID, PointLight*>::iterator;
	std::unordered_map<EntityID, PointLight*> point_light_arr_;
	void AddLightComponent(EntityID id, PointLight* point_light);
	void RemoveLightComponent(EntityID id);

	void UpdateLightPosition(PointLight* point_light, glm::vec2 cam_pos, glm::vec2 cam_size);
	void DrawPointLight(PointLight* point_light);

	//returns the name of the system for debug use
	std::string GetName();

	//function more akin to "What to do when message is received" for internal logic
	void SendMessageD(Message* m);
};

#endif