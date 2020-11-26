#pragma once
#ifndef _ENTITY_WINDOW_H_
#define _ENTITY_WINDOW_H_

#include <Windows.h>
#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/WindowsSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/Collision.h"

#include "Components/Transform.h"
#include "Components/Scale.h"
#include "Components/Motion.h"
#include "Components/PointLight.h"
#include "Components/AI.h"

#include "Manager/EntityManager.h"
#include "Manager/TextureManager.h"
#include "Manager/AnimationManager.h"
#include "Manager/ComponentManager.h"
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
    float inputWidth = 100.0f, float startVal = 0.0f, float endVal = 0.0f);

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
	\fn  Vec2Input(Vector2D& componentVar, float defaultVal = 0.0f, 
	const char* Xlabel = "##X", const char* Ylabel = "##Y")

	\brief Displays and handles the user input for vec2 variables
*/
/******************************************************************************/
	void Vec2Input(Vector2D& componentVar, float defaultVal = 0.0f, const char* Xlabel = "##X", const char* Ylabel = "##Y");

/******************************************************************************/
/*!
	\fn FloatInput(float& componentVar, const char* label = "X##float", float defaultVal = 0.0f)

	\brief Displays and handles the user input for float variables
*/
/******************************************************************************/
	void FloatInput(float& componentVar, const char* label = "X##float", float defaultVal = 0.0f);

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

/******************************************************************************/
/*!
	\fn GetAIState(int aiState)

	\brief Get the name of the AI State base on the Enum
*/
/******************************************************************************/
	const char* GetAIState(int aiState);

/******************************************************************************/
/*!
	\fn GetAIType(int aiType)

	\brief Get the name of the AI Type base on the Enum
*/
/******************************************************************************/
	const char* GetAIType(int aiType);

/******************************************************************************/
/*!
	\fn GetPlayerStatus(int playerState)

	\brief Get the name of the AI State base on the Enum
*/
/******************************************************************************/
	const char* GetPlayerStatus(int playerState);

	void SetArrowButtons(int& componentVar);

	ImVec2 SetButtonSize();

	void RemoveComponent(const char* windowName, std::string objName, Entity* entity, std::shared_ptr<Component> component);

	void DragEntity();

	void DragEntityCheckBox();

	void SelectEntityComponent();

	void EmitterInput(float& emitter_var, float default_val, std::string input_label, float start_val, float end_val, std::string button_label = ICON_FA_UNDO);

private:

	WindowsSystem* win_;
	InputSystem* input_;
	ImguiSystem* imgui_;
	Collision* collision_;
	CameraSystem* camera_;
	EntityManager* entities_;
	TextureManager* texture_;
	GraphicsSystem* graphics_;
	AnimationManager* animation_;
	ComponentManager* component_;
	EntityManager::EntityIdMapTypeIt entityIT;

	Vector2D originalVec_;

	const char* AIstates_[5]{ "Patrol", "Detected", "Chase", "Attack", "Return" };
	const char* AItype_[3]{ "StagBeetle", "Mite", "Hornet"};
	const char* Playerstatus_[4]{ "None", "Invisible", "Hit", "Burrow"};
	const char* emiiterstatus_[2] = {"Dead", "Alive" };

	bool b_draw = false;

};
#endif
