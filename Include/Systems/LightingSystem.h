/**********************************************************************************
*\file         LightingSystem.h
*\brief        Contains declaration of functions and variables used for
*			   the Lighting System
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef LIGHTINGSYSTEM_H
#define LIGHTINGSYSTEM_H

#include "Systems/ISystem.h"
#include "Systems/WindowsSystem.h"
#include "Systems/CameraSystem.h"
#include "Components/PointLight.h"
#include "Components/ConeLight.h"
#include "Manager/ComponentManager.h"
#include <unordered_map>
#include <windows.h>
#include <GL/glew.h>

class LightingSystem : public ISystem {

	bool debug_;

	std::map<std::string, Shader*> lighting_shaders_;
	Model* point_light_model_;
	Model* cone_light_model_;

	GLuint lighting_buffer;
	GLuint lighting_texture;
	GLuint addition_buffer;
	GLuint addition_texture;
	GLuint darkness_texture;

	WindowsSystem* windows_system_;
	CameraSystem* camera_system_;
	GraphicsSystem* graphics_system_;
	ComponentManager* component_manager_;

	glm::vec2 win_size_;

	std::vector<glm::vec3> color_sent;
	std::vector<glm::vec2> pos_sent;
	std::vector<float> intensity_sent;
	std::vector<float> radius_sent;
	std::vector<glm::vec2> direction_sent;
	std::vector<float> angle_sent;

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution{ 0.0f, 2.0f };

	int batch_size;

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

/******************************************************************************/
/*!
	\fn GetAdditionTexture()

	\brief Gets the texture where the lights are rendered to and returns its
			texture handle
*/
/******************************************************************************/
	GLuint* GetAdditionTexture();

	using PointLightType = CMap<PointLight>;
	using PointLightIt = PointLightType::MapTypeIt;
	PointLightType* point_light_arr_;

	using ConeLightType = CMap<ConeLight>;
	using ConeLightIt = ConeLightType::MapTypeIt;
	ConeLightType* cone_light_arr_;

/******************************************************************************/
/*!
	\fn UpdateLightPulse(PointLight* point_light, float frametime)

	\brief Updates the light position of a Light component from the Lighting Component's map
*/
/******************************************************************************/
	void UpdateLightPulse(PointLight* point_light, float frametime);

/******************************************************************************/
/*!
	\fn UpdateLightPulse(ConeLight* cone_light, float frametime)

	\brief Updates the light position of a Light component from the Lighting Component's map
*/
/******************************************************************************/
	void UpdateLightPulse(ConeLight* cone_light, float frametime);

/******************************************************************************/
/*!
	\fn UpdateLightPosition(PointLight* point_light)

	\brief Updates the light position of a Light component from the Lighting Component's map
*/
/******************************************************************************/
	void UpdateLightPosition(PointLight* point_light);

/******************************************************************************/
/*!
	\fn UpdateLightPosition(ConeLight* cone_light)

	\brief Updates the light position of a Light component from the Lighting Component's map
*/
/******************************************************************************/
	void UpdateLightPosition(ConeLight* cone_light);

/******************************************************************************/
/*!
	\fn DrawPointLight(Shader* shader, PointLight* point_light, float cam_zoom)

	\brief Batches the Point Light component
*/
/******************************************************************************/
	void BatchPointLight(PointLight* point_light, float cam_zoom);

/******************************************************************************/
/*!
	\fn DrawPointLight()

	\brief Draws the Point Light batch
*/
/******************************************************************************/
	void DrawPointLight();

/******************************************************************************/
/*!
	\fn BatchConeLight(Shader* shader, ConeLight* cone_light)

	\brief Batches the Cone Light component
*/
/******************************************************************************/
	void BatchConeLight(ConeLight* cone_light, float cam_zoom);

/******************************************************************************/
/*!
	\fn DrawConeLight()

	\brief Draws the Cone Light batch
*/
/******************************************************************************/
	void DrawConeLight();

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

/******************************************************************************/
/*!
	\fn RandomizePulse(ConeLight* cone_light)

	\brief Randomizes starting pulse
*/
/******************************************************************************/
	void RandomizePulse(PointLight* point_light);

/******************************************************************************/
/*!
	\fn RandomizePulse(ConeLight* cone_light)

	\brief Randomizes starting pulse
*/
/******************************************************************************/
	void RandomizePulse(ConeLight* cone_light);

};

#endif