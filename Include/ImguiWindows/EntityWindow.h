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

	void AddComponents(std::string component);
	void CloneArchetypes();
	bool DeletePopUp();

private:
	EntityManager* entities_;
	ImguiSystem* imgui_;
	bool b_addentity;
	ComponentManager* comp_mgr_;
	bool b_delete;
	std::string selectionName{};
};

#endif 
