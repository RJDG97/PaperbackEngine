/**********************************************************************************
*\file         ArchetypeWindow.h
*\brief        Contains declaration of functions and variables used for
*			   the ArchetypeWindow for the management of archetypes
*			   (Creating, deleting, amending existing archetypes)

*\author	   Ee Ling Adele, Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#ifndef _ARCHETYPE_WINDOW_H_
#define _ARCHETYPE_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"
#include "Manager/EntityManager.h"
#include "Manager/ComponentManager.h"
#include "GameStates/Levels.h"
#include "Systems/Factory.h"
#include "ImguiWindows/EntityWindow.h"

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

/******************************************************************************/
/*!
	\fn AddArchetype(std::string component)

	\brief Adds a new archetype with the name component
*/
/******************************************************************************/
	void AddArchetype(std::string component);

/******************************************************************************/
/*!
	\fn AddComponent()

	\brief Adds a new component to the entity/archetype
*/
/******************************************************************************/
	void AddComponent();

/******************************************************************************/
/*!
	\fn AvaliableArchetypes()

	\brief Shows the list of avaliable archetype 
*/
/******************************************************************************/
	void AvaliableArchetypes();

/******************************************************************************/
/*!
	\fn NoCameraPopUp()

	\brief Pop to prevent users from spawning entities without an active camera
*/
/******************************************************************************/
	void NoCameraPopUp();

/******************************************************************************/
/*!
	\fn MissingComponentPopUp()

	\brief Pop to prevent users from spawning entities with texture
		   but missing transform and scale component
*/
/******************************************************************************/
	void MissingComponentPopUp();

/******************************************************************************/
/*!
	\fn AddSingleComponent(std::string archetype, ComponentTypes component)

	\brief Allow users to add in specific individual components
*/
/******************************************************************************/
	void AddSingleComponent(std::string archetype, ComponentTypes component);

/******************************************************************************/
/*!
	\fn AddNewArchetypePopup()

	\brief Pop to allow users to add in a new Archetype with a name component
*/
/******************************************************************************/
	void AddNewArchetypePopup();

/******************************************************************************/
/*!
	\fn ArchetypeMenuBar()

	\brief Menu bar of the archetype window 
*/
/******************************************************************************/
	void ArchetypeMenuBar();

/******************************************************************************/
/*!
	\fn CheckArchetypeTexture(std::string path)

	\brief Check if textures are being used by certain archetypes
*/
/******************************************************************************/
	std::vector<std::string> CheckArchetypeTexture(std::string path);

private:
	EntityManager* entities_;
	ImguiSystem* imgui_;
	ComponentManager* comp_mgr_;
	Levels levels_;
	EntityFactory* factory_;

	bool b_nocam, b_missingcomp, b_noscale, b_notrans, b_new_archetype, b_add;

	std::string archetype_name;
};

#endif 
