/**********************************************************************************
*\file         EntityPathWindow.h
*\brief        Contains declaration of functions and variables used for
*			   the EntityPathWindow to set the saving json path for each
*			   archetype type

*\author	   Ee Ling Adele, Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#ifndef _ENTITY_PATH_WINDOW_H_
#define _ENTITY_PATH_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"
#include "GameStates/Levels.h"
#include "Manager/EntityManager.h"
#include "Systems/Factory.h"

class EntityPathWindow : public IWindow {
  
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
	\fn ManagePaths(Level* editor)

	\brief Manages the deletion and update of Entity filepath
*/
/******************************************************************************/
	void ManagePaths(Level* editor);

/******************************************************************************/
/*!
	\fn AddPaths(Level* editor)

	\brief Set filepath for the entities save location
*/
/******************************************************************************/
	void AddPaths(Level* editor);

private:

	ImguiSystem* imgui_;
	EntityManager* entities_;
	EntityFactory* factory_;

	bool b_wrong_loc;
};
#endif