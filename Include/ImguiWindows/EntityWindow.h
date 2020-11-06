#pragma once
#ifndef _ENTITY_WINDOW_H_
#define _ENTITY_WINDOW_H_

#include <Windows.h>
#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"

#include "Components/Transform.h"
#include "Components/Scale.h"
#include "Components/Motion.h"
#include "Components/PointLight.h"
#include "Components/AI.h"

#include "Manager/EntityManager.h"
#include "Systems/Game.h"
#include "Engine/Core.h"


class EntityWindow : public IWindow{

public:
	void Init() override;
	void Update() override;

/******************************************************************************/
/*!
	\fn ComponentType(std::pair<Entity*, std::vector<ComponentTypes>> entitycomponents)

	\brief Prints out the different ImGui widgets required for each component type
*/
/******************************************************************************/
	void CheckComponentType(std::pair<Entity*, std::vector<ComponentTypes>> entitycomponents);

/******************************************************************************/
/*!
	\fn ComponentInput(const char* componentLabel, const char* inputLabel,
	float& componentVar, float startVal = 0.1f, float endVal = 1.0f,
	float inputWidth = 110.0f)

	\brief Prints out ImGui input space for components
*/
/******************************************************************************/
	void ComponentInputFloat(const char* componentLabel, const char* inputLabel, float& componentVar, float inputWidth = 100.0f, float startVal = 0.1f, float endVal = 1.0f);
	void ComponentInputInt(const char* componentLabel, const char* inputLabel, int& componentVar, float inputWidth = 100.0f, int startVal = 1, int endVal = 5);

/******************************************************************************/
/*!
	\fn ComponentInput(ImVec4 color, const char* label,
	float componentVal = 0.0f, const char* format = "%.2f")

	\brief Prints out text for the float components
*/
/******************************************************************************/
	void ComponentDisplayFloat(ImVec4 color, const char* label, float componentVal = 0.0f, const char* format = "%.2f");

/******************************************************************************/
/*!
	\fn ComponentInput(ImVec4 color, const char* label,
	float componentVal = 0.0f, const char* format = "%.2f")

	\brief Prints out text for the Vec2D components
*/
/******************************************************************************/
	void ComponentDisplayVec(ImVec4 color, const char* label, Vector2D componentVec = { 0.0f, 0.0f });

/******************************************************************************/
/*!
	\fn GetEntityComponents(Entity* entity)

	\brief Retrieves the components that the entity owns
*/
/******************************************************************************/
	std::pair<Entity*, std::vector<ComponentTypes>> GetEntityComponents(Entity* entity);

/******************************************************************************/
/*!
	\fn ShowEntityList()

	\brief Shows the list of entities in the scene
*/
/******************************************************************************/
	void ShowEntityList();

	const char* GetAIState(int aiState);

private:
	ImguiSystem* imgui_system_;
	EntityManager* entities_;
	Entity* selection;

	EntityManager::EntityIdMapTypeIt entityIT;

	const char* AIstates[5]{ "Patrol", "Detected", "Chase", "Attack", "Return" };

};
#endif
