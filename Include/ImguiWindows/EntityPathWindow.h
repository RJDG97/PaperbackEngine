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

};
#endif