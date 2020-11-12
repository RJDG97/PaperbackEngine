#pragma once
#ifndef LIGHTINGSYSTEM_H
#define LIGHTINGSYSTEM_H

#include "Systems/ISystem.h"
#include "Systems/WindowsSystem.h"
#include "Systems/CameraSystem.h"
#include "Components/PointLight.h"
#include "Manager/ComponentManager.h"
#include <unordered_map>
#include <windows.h>
#include <GL/glew.h>

class LightingSystem : public ISystem {

	bool debug_;

	std::map<std::string, Shader*> lighting_shaders_;
	Model* light_model_;

	GLuint lighting_buffer;
	GLuint lighting_texture;

	glm::vec2* cam_pos_;
	float* cam_zoom_;

	WindowsSystem* windows_system_;
	CameraSystem* camera_system_;

	glm::vec2 win_size_;

public:

/******************************************************************************/
/*!
	\fn LightingSystem()

	\brief Default constructor for lighting system
*/
/******************************************************************************/
	LightingSystem();

/******************************************************************************/
/*!
	\fn ~LightingSystem()

	\brief Default destructor for lighting system
*/
/******************************************************************************/
	~LightingSystem();

/******************************************************************************/
/*!
	\fn Init()

	\brief Initializes the lighting system
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
	\fn Update(float frametime)

	\brief Updates the position of lights
*/
/******************************************************************************/
	void Update(float frametime);

/******************************************************************************/
/*!
	\fn Draw()

	\brief Renders all light components
*/
/******************************************************************************/
	void Draw();

/******************************************************************************/
/*!
	\fn Cleanup()

	\brief Cleans up the lighting system
*/
/******************************************************************************/
	void Cleanup();


/******************************************************************************/
/*!
	\fn GetLightingTexture()

	\brief Gets the texture where the lights are rendered to and returns its
		   texture handle
*/
/******************************************************************************/
	GLuint* GetLightingTexture();

	using PointLightType = CMap<PointLight>;
	using PointLightIt = PointLightType::MapTypeIt;
	PointLightType* point_light_arr_;

	using TransformType = CMap<Transform>;
	using TransformIt = TransformType::MapTypeIt;
	TransformType* transform_arr_;

/******************************************************************************/
/*!
	\fn UpdateLightPosition(PointLight* point_light)

	\brief Updates the light position of a Light component from the Lighting Component's map
*/
/******************************************************************************/
	void UpdateLightPosition(PointLight* point_light);

/******************************************************************************/
/*!
	\fn DrawPointLight(Shader* shader, PointLight* point_light)

	\brief Draws the Light component
*/
/******************************************************************************/
	void DrawPointLight(Shader* shader, PointLight* point_light);

/******************************************************************************/
/*!
	\fn GetName()

	\brief Returns the name of the system for debug use
*/
/******************************************************************************/
	std::string GetName();

/******************************************************************************/
/*!
	\fn SendMessageD(Message* m)

	\brief Function more akin to "What to do when message is received" for internal logic
*/
/******************************************************************************/
	void SendMessageD(Message* m);
};

#endif