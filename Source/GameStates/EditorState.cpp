
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
	std::cout << "press SPACE to PAUSE" << std::endl;
	std::cout << "press ESCAPE to return to MAIN MENU" << std::endl << std::endl;
	std::cout << "-----------------------------" << std::endl << std::endl;
	
	CORE->ResetCorePauseStatus();
	FACTORY->LoadLevel("Editor");

	CORE->GetManager<AMap>()->InitAMap(CORE->GetManager<EntityManager>()->GetEntities());
	CORE->GetSystem<PartitioningSystem>()->InitPartition();
}

void EditorState::Free()
{
	std::cout << "EditorState clean Successful" << std::endl;

	FACTORY->DestroyAllEntities();
}

void EditorState::Update(Game* game, float frametime)
{
	if (!CORE->GetCorePauseStatus()) 
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

	m_PlayState.StateInputHandler(msg, game);
}