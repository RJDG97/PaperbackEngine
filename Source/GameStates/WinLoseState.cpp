/**********************************************************************************
*\file         WinLoseState.cpp
*\brief        Contains definition of WinLose State
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "GameStates/WinLoseState.h"
#include "GameStates/MenuState.h"
#include "GameStates/PlayState.h"
#include "Systems/Factory.h"
#include "Engine/Core.h"
#include "Manager/AMap.h"
#include "Systems/Partitioning.h"
#include "Systems/ImguiSystem.h"

WinLoseState m_WinLoseState;

void WinLoseState::Init(std::string level_name) {

	CORE->ResetGodMode();
	CORE->ResetCorePauseStatus();
	CORE->ResetGamePauseStatus();
	
	CORE->GetManager<LayerManager>()->LoadLevelLayers(level_name);

	is_win_ = (level_name == "Win") ? true : false;

	FACTORY->LoadLevel(level_name);

	CORE->GetManager<AMap>()->InitAMap(CORE->GetManager<EntityManager>()->GetEntities());
	CORE->GetSystem<PartitioningSystem>()->InitPartition();

	CORE->GetSystem<GraphicsSystem>()->EnableLighting(false);

	MessageBGM_Play msg{ level_name };
	CORE->BroadcastMessage(&msg);
}


void WinLoseState::Update(Game* game, float frametime) {

	(void)game;
	(void)frametime;
}


void WinLoseState::Draw(Game* game) {

	(void)game;
}


void WinLoseState::Free() {

	CORE->ResetGodMode();
	CORE->ResetCorePauseStatus();
	CORE->ResetGamePauseStatus();

	CORE->GetSystem<ImguiSystem>()->ResetSelectedEntity();
	FACTORY->DestroyAllEntities();
}


void WinLoseState::StateInputHandler(Message* msg, Game* game) {

	switch(msg->message_id_)
	{
		case MessageIDTypes::BUTTON:
		{
			Message_Button* m = dynamic_cast<Message_Button*>(msg);

			if (m->button_index_ == 1) {

				//if win then either next stage or back to title
				if (is_win_) {
					
					Levels* levels = CORE->GetSystem<EntityFactory>()->GetLevelsFile();
					Level* curr_play = levels->GetLastPlayLevel();

					if (curr_play->optional_next_ != "") {
					
						//there is successive level
						levels->GetPlayLevel(curr_play->optional_next_);
					}
					else {

						//last level
						levels->ResetPlayLevels();
						game->ChangeState(&m_MenuState);
						return;
					}
				}

				//else case of lose state, replay stage
				game->ChangeState(&m_PlayState);
				return;
			}
			if (m->button_index_ == 2) {

				//quitting, temporarily reset the play counter
				Levels* levels = CORE->GetSystem<EntityFactory>()->GetLevelsFile();
				//levels->ResetPlayLevels();

				game->ChangeState(&m_MenuState);
				return;
			}
		}
		default:
			break;
	}
}

std::string WinLoseState::GetStateName() {
	
	return "WinLose";
}