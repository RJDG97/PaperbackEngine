/**********************************************************************************
*\file         MenuState.cpp
*\brief        Contains definition of Menu State
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
#include "GameStates/MenuState.h"
#include "GameStates/CutSceneState.h"
#include "GameStates/CreditsState.h"

#include "Systems/InputSystem.h"
#include "Systems/WindowsSystem.h"
#include "Systems/SoundSystem.h"
#include "Systems/PauseSystem.h"
#include "Manager/TextureManager.h"
#include "Manager/AnimationManager.h"
#include "Manager/AMap.h"
#include "Manager/TransitionManager.h"

#include "Engine/Core.h"

#include "Systems/GraphicsSystem.h"
#include "Systems/Factory.h"
#include "Systems/Collision.h"
#include "Systems/Parenting.h"

#include "Components/Status.h"

#include "Components/Transform.h"

//SAMPLE MENU STATE

MenuState m_MenuState;

void MenuState::Init(std::string)
{
	help_ = false;

	CORE->GetManager<LayerManager>()->LoadLevelLayers("Menu");
	FACTORY->LoadLevel("Menu");
	CORE->GetSystem<GraphicsSystem>()->EnableLighting(false);

	CORE->ResetGodMode();
	CORE->ResetCorePauseStatus();
	CORE->ResetGamePauseStatus();

	MessageBGM_Play msg{ "Game_BGM" };
	CORE->BroadcastMessage(&msg);

	CORE->GetManager<AMap>()->InitAMap(CORE->GetManager<EntityManager>()->GetEntities());
	CORE->GetSystem<PartitioningSystem>()->InitPartition();
	CORE->GetSystem<ParentingSystem>()->LinkParentAndChild();
	CORE->GetSystem<CameraSystem>()->CameraZoom(CORE->GetSystem<CameraSystem>()->GetMainCamera(), 0.8f);
	CORE->GetSystem<PauseSystem>()->InitializeClickables();
	CORE->GetManager<TransitionManager>()->ResetVignetteScale();

	component_mgr_ = &*CORE->GetManager<ComponentManager>();
	logic_mgr_ = &*CORE->GetManager<LogicManager>();
	logic_arr_ = component_mgr_->GetComponentArray<LogicComponent>();
}

void MenuState::Free()
{
	std::cout << "MenuState clean Successful" << std::endl;

	CORE->GetSystem<SoundSystem>()->StopSound("All", true);

	CORE->ResetGodMode();
	CORE->ResetCorePauseStatus();
	CORE->ResetGamePauseStatus();

	FACTORY->DestroyAllEntities();

	help_ = false;
}

void MenuState::Update(Game* game, float frametime)
{
	UNREFERENCED_PARAMETER(game);
	UNREFERENCED_PARAMETER(frametime);

	for (LogicIt it = logic_arr_->begin(); it != logic_arr_->end(); ++it) {

		std::string scr = it->second->GetLogic("Title");
		logic_mgr_->Exec(scr, it->first);
	}
}

void MenuState::Draw(Game* game)
{
	UNREFERENCED_PARAMETER(game);
}

std::string MenuState::GetStateName() {

	return "Menu";
}

void MenuState::StateInputHandler(Message* msg, Game* game) {

	if (game && !game->debug_) {

		switch (msg->message_id_)
		{
			//check for collision between button & mouse
			case MessageIDTypes::BUTTON: {

				if (CORE->GetManager<TransitionManager>()->CheckInTransition())
					return;

				Message_Button* m = dynamic_cast<Message_Button*>(msg);

				switch (m->button_index_)
				{
					// Case 1: Enter playstate
					case 1:
					{
						MessageBGM_Play button{ "Click_Btn" };
						CORE->BroadcastMessage(&button);

						//CORE->GetSystem<Game>()->ChangeState(&m_CutSceneState);
						CORE->GetManager<TransitionManager>()->ResetTransition("Default", &m_CutSceneState, false, true);
						return;
						break;
					}
					// Case 2: How to play menu
					case 2:
					{
						MessageBGM_Play button{ "Click_Btn" };
						CORE->BroadcastMessage(&button);

						CORE->GetSystem<PauseSystem>()->EnableNextLayer();

						return;
						break;
					}
					// Case 4: Toggle quit game
					case 4:
					{
						MessageBGM_Play button{ "Click_Btn" };
						CORE->BroadcastMessage(&button);
						CORE->GetSystem<PauseSystem>()->SetActiveLayer(3);

						return;
						break;
					}
					// Case 5 = Quit Game
					case 5:
					{
						Message mesg{ MessageIDTypes::EXIT };
						CORE->BroadcastMessage(&mesg);
						break;
					}
					// Case 6 = Return to previous layer (Return to pause menu layer)
					case 6:
					{
						MessageBGM_Play button{ "Click_Btn" };
						CORE->BroadcastMessage(&button);
						CORE->GetSystem<PauseSystem>()->SetActiveLayer(1);
						break;
					}
					case 10:
					{
						if (help_)
							break;

						CORE->GetSystem<Game>()->ChangeState(&m_CreditsState);
					}
				}
				break;
			}
			case MessageIDTypes::M_BUTTON_PRESS: {
				//for menu navigation in menu
				if (CORE->GetSystem<PauseSystem>()->PrevLayer() == 2)
					CORE->GetSystem<PauseSystem>()->RevertPreviousLayer();
				break;
			}
		}
	}
}