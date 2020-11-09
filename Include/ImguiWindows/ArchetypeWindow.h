#pragma once
#ifndef _ARCHETYPE_WINDOW_H_
#define _ARCHETYPE_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"
#include "Manager/EntityManager.h"
#include "Manager/ComponentManager.h"

class ArchetypeWindow : public IWindow
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

	void AddArchetype(std::string component);

	void AvaliableArchetypes();
	
	void AddComponents(ComponentTypes type, const char* typeName, const char* buttonLabel);

private:
	EntityManager* entities_;
	ImguiSystem* imgui_;
	ComponentManager* comp_mgr_;

	bool b_editcomp;

};

#endif 
