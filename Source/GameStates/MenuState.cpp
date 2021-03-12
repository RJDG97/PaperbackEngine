/**********************************************************************************
*\file         MenuState.cpp
*\brief        Contains definition of Menu State
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include <iostream>

#include "Systems/Game.h"
#include "GameStates/MenuState.h"
#include "GameStates/PlayState.h"
#include "GameStates/EditorState.h"
#include "GameStates/WinLoseState.h"

#include "Systems/InputSystem.h"
#include "Systems/WindowsSystem.h"
#include "Systems/ImGuiSystem.h"
#include "Manager/TextureManager.h"
#include "Manager/AnimationManager.h"
#include "Manager/AMap.h"
#include "Manager/TransitionManager.h"

#include "Engine/Core.h"

#include "Systems/GraphicsSystem.h"
#include "Systems/Factory.h"
#include "Systems/Collision.h"
#include "Systems/ImguiSystem.h"
#include "Systems/Parenting.h"

#include "Components/Status.h"

#include "Components/Transform.h"

//SAMPLE MENU STATE

MenuState m_MenuState;

void MenuState::Init(std::string)
{
	help_ = false;

	FACTORY->LoadLevel("Menu");
	CORE->GetSystem<GraphicsSystem>()->EnableLighting(false);

	CORE->ResetGodMode();
	CORE->ResetCorePauseStatus();
	CORE->ResetGamePauseStatus();

	MessageBGM_Play msg{ "MenuDefault" };
	CORE->BroadcastMessage(&msg);

	CORE->GetManager<AMap>()->InitAMap(CORE->GetManager<EntityManager>()->GetEntities());
	CORE->GetSystem<PartitioningSystem>()->InitPartition();
	CORE->GetSystem<ParentingSystem>()->LinkParentAndChild();
	CORE->GetSystem<CameraSystem>()->CameraZoom(CORE->GetSystem<CameraSystem>()->GetMainCamera(), 0.8f);

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
	CORE->GetSystem<ImguiSystem>()->ResetSelectedEntity();

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
	//GraphicsSystem::Instance().Draw();
	UNREFERENCED_PARAMETER(game);
}

std::string MenuState::GetStateName() {

	return "Menu";
}

void MenuState::StateInputHandler(Message* msg, Game* game) {

	if (game && !game->debug_) {

		switch (msg->message_id_) {
			//check for collision between button & mouse
		case MessageIDTypes::BUTTON: {

			Message_Button* m = dynamic_cast<Message_Button*>(msg);

			switch (m->button_index_)
			{
			case 1:
			{

				if (help_)
					break;

				MessageBGM_Play button{ "ButtonPress" };
				CORE->BroadcastMessage(&button);

				// Enter play state
				CORE->GetManager<TransitionManager>()->ResetTransition("Default", &m_PlayState);
				return;
				break;
			}
			case 2:
			{

				if (help_)
					break;

				MessageBGM_Play button{ "ButtonPress" };
				CORE->BroadcastMessage(&button);

				// "How to play"
				CORE->GetSystem<Collision>()->ToggleClickables(2);
				CORE->GetSystem<Collision>()->ToggleClickables(0);
				help_ = true;
				return;
				break;
			}
			case 3: 
			{

				if (help_)
					break;

				MessageBGM_Play button{ "ButtonPress" };
				CORE->BroadcastMessage(&button);

				// Editor mode
				CORE->GetSystem<ImguiSystem>()->SetImguiBool(true);
				game->ChangeState(&m_EditorState);
				return;
				break;
			}
			case 4: 
			{

				if (help_)
					break;

				MessageBGM_Play button{ "ButtonPress" };
				CORE->BroadcastMessage(&button);
				// Toggle off game
				CORE->SetGameActiveStatus(false);
				return;
				break;
			}
			case 7: 
			{

				if (!help_)
					break;

				MessageBGM_Play button{ "ButtonPress" };
				CORE->BroadcastMessage(&button);

				CORE->GetSystem<Collision>()->ToggleClickables(2);
				CORE->GetSystem<Collision>()->ToggleClickables(0);
				help_ = false;
				return;
				break;
			}
			}
			break;
		}
		case MessageIDTypes::M_BUTTON_PRESS: {
			//for menu navigation in menu
			break;
		}
		}
	}
}