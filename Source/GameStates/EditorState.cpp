/**********************************************************************************
*\file         EditorState.cpp
*\brief        Contains definition of Editor State
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include <iostream>

#include "Systems/Game.h"
#include "GameStates/EditorState.h"
#include "GameStates/PlayState.h"

#include "Systems/InputSystem.h"

#include "Engine/Core.h"
#include "Systems/Factory.h"

#include "Components/Transform.h"
#include "Components/Motion.h"
#include "Components/Name.h"

#include "Entity/ComponentTypes.h"

#include "Manager/ForcesManager.h"
#include "Manager/AMap.h"

#include "Systems/Partitioning.h"

#include <memory>

EditorState m_EditorState;

//demo pointer to player
//Entity* player;

void EditorState::Init(std::string)
{
	std::cout << "-----------------------------" << std::endl << std::endl;
	std::cout << "EditorState init Successful" << std::endl;
	std::cout << "-----------------------------" << std::endl << std::endl;
	
	CORE->ResetCorePauseStatus();
	CORE->GetManager<LayerManager>()->GetRenderLayers()->clear();
	CORE->GetManager<LayerManager>()->LoadLevelLayers("Play");
	FACTORY->LoadLevel("Editor");

	CORE->GetManager<AMap>()->InitAMap(CORE->GetManager<EntityManager>()->GetEntities());
	CORE->GetSystem<PartitioningSystem>()->InitPartition();

	m_PlayState.Init();

	imgui_ = &*CORE->GetSystem<ImguiSystem>();

	CORE->SetGodMode(true);
}

void EditorState::Free()
{
	std::cout << "EditorState clean Successful" << std::endl;
	CORE->GetSystem<SoundSystem>()->StopSound("All", true);
	FACTORY->DestroyAllEntities();
}

void EditorState::Update(Game* game, float frametime)
{
	m_PlayState.Update(game, frametime);
}

void EditorState::Draw(Game* game)
{
	m_PlayState.Draw(game);
}

std::string EditorState::GetStateName() {

	return "Editor";
}

void EditorState::StateInputHandler(Message* msg, Game* game) {

	if (!imgui_->EditorMode())
		m_PlayState.StateInputHandler(msg, game);
}