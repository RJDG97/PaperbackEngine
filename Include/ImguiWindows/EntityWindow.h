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
#include "Engine/Core.h"

class EntityWindow : public IWindow{

public:
/******************************************************************************/
/*!
	\fn Init()

	\brief Initializes the ImGui System
*/
/******************************************************************************/
	void Init() override;

/******************************************************************************/
/*!
	\fn Update()

	\brief Updates all ImGui Windows in the system
*/
/******************************************************************************/
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

	\brief Prints out ImGui input space for Float components
*/
/******************************************************************************/
	void ComponentInputFloat(const char* componentLabel, const char* inputLabel, float& componentVar, 
    float inputWidth = 100.0f, float startVal = 0.1f, float endVal = 1.0f);

/******************************************************************************/
/*!
	\fn ComponentInput(const char* componentLabel, const char* inputLabel,
	int& componentVar, int startVal = 1, int endVal = 5,
	float inputWidth = 110.0f)

	\brief Prints out ImGui input space for components
*/
/******************************************************************************/
	void ComponentInputInt(const char* componentLabel, const char* inputLabel, int& componentVar, 
	float inputWidth = 100.0f, int startVal = 1, int endVal = 5);

/******************************************************************************/
/*!
	\fn ComponentDisplayFLoat(ImVec4 color, const char* label,
	float componentVal, const char* format = "%.2f")

	\brief Prints out text for the float components
*/
/******************************************************************************/
	void ComponentDisplayFloat(ImVec4 color, const char* label, float componentVal, const char* format = "%.2f");

/******************************************************************************/
/*!
	\fn ComponentDisplayInt(ImVec4 color, const char* label,
	int componentVal, const char* format = "%d")

	\brief Prints out text for the float components
*/
/******************************************************************************/
	void ComponentDisplayInt(ImVec4 color, const char* label, int componentVal, const char* format = "%d");

/******************************************************************************/
/*!
	\fn ComponentDisplayVec(ImVec4 color, const char* label,
	Vector2D componentVec = { 0.0f, 0.0f })

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

	const char* GetAIType(int aiType);

	const char* GetPlayerStatus(int playerState);

	void SetArrowButtons(int& componentVar);
	ImVec2 SetButtonSize();

	void Vec2Input(Vector2D& componentVar, float defaultVal = 0.0f);
	void FloatInput(float& componentVar, const char* label = "X##hey", float defaultVal = 0.0f);

private:
	ImguiSystem* imgui_;
	EntityManager* entities_;

	EntityManager::EntityIdMapTypeIt entityIT;

	const char* AIstates_[5]{ "Patrol", "Detected", "Chase", "Attack", "Return" };
	const char* AItype_[3]{ "StagBeetle", "Mite", "Hornet"};
	const char* Playerstatus_[4]{ "None", "Invisible", "Hit", "Burrow"};

	bool b_draw = false;

};
#endif
