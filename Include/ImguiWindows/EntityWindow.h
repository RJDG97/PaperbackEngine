/**********************************************************************************
*\file         EntityWindow.h
*\brief        Contains declaration of functions and variables used for
*			   the EntityWindow to display/manage entities as well as their components

*\author	   Ee Ling Adele, Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#ifndef _ENTITY_WINDOW_H_
#define _ENTITY_WINDOW_H_

#include <Windows.h>
#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/WindowsSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/SoundSystem.h"
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

/******************************************************************************/
/*!
	\fn SetArrowButtons(int& componentVar)

	\brief Set the arrows buttons for the variable
*/
/******************************************************************************/
	void SetArrowButtons(int& componentVar);

/******************************************************************************/
/*!
	\fn SetButtonSize()

	\brief Set the button size for based on the size a single character 
*/	
/******************************************************************************/
	ImVec2 SetButtonSize();

/******************************************************************************/
/*!
	\fn RemoveComponent(const char* windowName, std::string objName, Entity* entity, 
		std::shared_ptr<Component> component)

	\brief Remove the chosen component from the specific archetype
*/
/******************************************************************************/
	void RemoveComponent(const char* windowName, std::string objName, Entity* entity, std::shared_ptr<Component> component);

/******************************************************************************/
/*!
	\fn DragEntity()

	\brief Handles the calculation and mouse state check for dragging entities
*/
/******************************************************************************/
	void DragEntity();

/******************************************************************************/
/*!
	\fn DragEntityCheckBox()

	\brief To enable or disable users to be able to drag entities around
*/
/******************************************************************************/
	void DragEntityCheckBox();

/******************************************************************************/
/*!
	\fn SelectEntityComponent()

	\brief Gets the selected entity/archetype's component
*/
/******************************************************************************/
	void SelectEntityComponent();

/******************************************************************************/
/*!
	\fn EmitterInput(float& emitter_var, float default_val, std::string input_label, 
		float start_val, float end_val, std::string button_label = ICON_FA_UNDO)

	\brief Input space for Emitter Component variables
*/
/******************************************************************************/
	void EmitterInput(float& emitter_var, float default_val, std::string input_label, float start_val, float end_val, std::string button_label = ICON_FA_UNDO);

/******************************************************************************/
/*!
	\fn NameComponent(Entity* entity)

	\brief Display the entity's Name Component
*/
/******************************************************************************/
	void NameComponent(Entity* entity);

/******************************************************************************/
/*!
	\fn MotionComponent(Entity* entity)

	\brief Displays the entity's motion component
*/
/******************************************************************************/
	void MotionComponent(Entity* entity);

/******************************************************************************/
/*!
	\fn TransformComponent(Entity* entity)

	\brief Displays the entity's transform component
*/
/******************************************************************************/
	void TransformComponent(Entity* entity);

/******************************************************************************/
/*!
	\fn HealthComponent(Entity* entity)

	\brief Displays the entity's health component
*/
/******************************************************************************/
	void HealthComponent(Entity* entity);

/******************************************************************************/
/*!
	\fn CameraComponent(Entity* entity)

	\brief Displays the entity's camera component
*/
/******************************************************************************/
	void CameraComponent(Entity* entity);

/******************************************************************************/
/*!
	\fn TextureRendererComponent(Entity* entity)

	\brief Displays the entity's texture renderer component
*/
/******************************************************************************/
	void TextureRendererComponent(Entity* entity);

/******************************************************************************/
/*!
	\fn AnimationRendererComponent(Entity* entity)

	\brief Displays the entity's animation component
*/
/******************************************************************************/
	void AnimationRendererComponent(Entity* entity);

/******************************************************************************/
/*!
	\fn AABBComponent(Entity* entity)

	\brief Displays the entity's AABB/Collider component
*/
/******************************************************************************/
	void AABBComponent(Entity* entity);

/******************************************************************************/
/*!
	\fn AIComponent(Entity* entity)

	\brief Displays the entity's AI component
*/
/******************************************************************************/
	void AIComponent(Entity* entity);

/******************************************************************************/
/*!
	\fn ScaleComponent(Entity* entity)

	\brief Displays the entity's scale component
*/
/******************************************************************************/
	void ScaleComponent(Entity* entity);

/******************************************************************************/
/*!
	\fn StatusComponent(Entity* entity)

	\brief Displays the entity's status component
*/
/******************************************************************************/
	void StatusComponent(Entity* entity);

/******************************************************************************/
/*!
	\fn PointLightComponent(Entity* entity)

	\brief Displays the entity's pointlight component
*/
/******************************************************************************/
	void PointLightComponent(Entity* entity);

/******************************************************************************/
/*!
	\fn ConeLightComponent(Entity* entity)

	\brief Displays the entity's conelight component
*/
/******************************************************************************/
	void ConeLightComponent(Entity* entity);

/******************************************************************************/
/*!
	\fn ParentChildComponent(Entity* entity)

	\brief Displays the entity's parent child component
*/
/******************************************************************************/
	void ParentChildComponent(Entity* entity);

/******************************************************************************/
/*!
	\fn ParticleComponent(Entity* entity)

	\brief Displays the entity's particle component

/******************************************************************************/
	void ParticleComponent(Entity* entity);

/******************************************************************************/
/*!
	\fn EmitterComponent(Entity* entity)

	\brief Displays the entity's emitter component
*/
/******************************************************************************/
	void EmitterComponent(Entity* entity);

/******************************************************************************/
/*!
	\fn SoundEmitterComponent(Entity* entity)

	\brief Displays the entity's soundemitter component
*/
/******************************************************************************/
	void SoundEmitterComponent(Entity* entity);

private:

	WindowsSystem* win_;
	InputSystem* input_;
	ImguiSystem* imgui_;
	Collision* collision_;
	CameraSystem* camera_;
	SoundSystem* sound_;
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

	bool b_draw, b_grid, b_light;

};
#endif
