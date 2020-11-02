#pragma once
#ifndef _ENTITY_WINDOW_H_
#define _ENTITY_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"
#include "Manager/EntityManager.h"
#include "Manager/ComponentManager.h"

class EntityWindow : public IWindow
{
public:


	/******************************************************************************/
	/*!
		\fn Init()

		\brief Initializes the ImGui Viewport Window
	*/
	/******************************************************************************/
	void Init() override;

	/******************************************************************************/
	/*!
		\fn Update()

		\brief Updates the ImGui Viewport
	*/
	/******************************************************************************/
	void Update() override;

private:
	EntityManager* entities_;
	ImguiSystem* imgui_;
	bool b_addentity;
	ComponentManager* comp_mgr_;

	//std::vector<const char*> comp_types_{ "Name", "Motion", "Transform", "Health", "Camera",
	//"Controller", "Texture Renderer", "Animation Renderer", "Text Renderer", "AABB", "AI",
	//"Scale", "Status", "Point Light", "Cone Light", "Basic AI", "Clickable", "Input Controller" };
	//const char* component_types_[] = {  };

};

#endif 