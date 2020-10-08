#pragma once
#ifndef LIGHTINGSYSTEM_H
#define LIGHTINGSYSTEM_H

#include "Systems/ISystem.h"
#include "Systems/WindowsSystem.h"
#include "Components/PointLight.h"
#include <unordered_map>
#include <windows.h>
#include <GL/glew.h>

class LightingSystem : public ISystem {

	bool debug_;

	GLuint lighting_buffer;
	GLuint lighting_texture;

	glm::vec2* cam_pos_;
	glm::vec2* cam_size_;

	WindowsSystem* windows_system;

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

	using PointLightIt = std::unordered_map<EntityID, PointLight*>::iterator;
	std::unordered_map<EntityID, PointLight*> point_light_arr_;

/******************************************************************************/
/*!
\fn AddLightComponent(EntityID id, PointLight* point_light)

\brief Adds an Light component to the Lighting Component's map
*/
/******************************************************************************/
	void AddLightComponent(EntityID id, PointLight* point_light);

/******************************************************************************/
/*!
\fn RemoveLightComponent(EntityID id)

\brief Removes a Light component from the Lighting Component's map
*/
/******************************************************************************/
	void RemoveLightComponent(EntityID id);

/******************************************************************************/
/*!
\fn UpdateLightPosition(PointLight* point_light, glm::vec2 cam_pos, glm::vec2 cam_size)

\brief Updates the light position of a Light component from the Lighting Component's map
*/
/******************************************************************************/
	void UpdateLightPosition(PointLight* point_light, glm::vec2 cam_pos, glm::vec2 cam_size);

/******************************************************************************/
/*!
\fn DrawPointLight(PointLight* point_light)

\brief Draws the Light component
*/
/******************************************************************************/
	void DrawPointLight(PointLight* point_light);

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