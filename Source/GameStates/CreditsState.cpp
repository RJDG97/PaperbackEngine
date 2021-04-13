/**********************************************************************************
*\file         SplashState.cpp
*\brief        Contains definition of Splash State
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "GameStates/CreditsState.h"
#include "GameStates/MenuState.h"
#include "Manager/TransitionManager.h"
#include "Engine/Core.h"
#include "Systems/Factory.h"
#include "Systems/SoundSystem.h"

CreditsState m_CreditsState;

CreditsState::CreditsState()
{ }


void CreditsState::Init(std::string) {

	component_mgr_ = &*CORE->GetManager<ComponentManager>();
	entity_mgr_ = &*CORE->GetManager<EntityManager>();

	CORE->ResetCorePauseStatus();
	CORE->ResetGamePauseStatus();

	CORE->GetManager<LayerManager>()->LoadLevelLayers("Cutscene");
	FACTORY->LoadLevel("Cutscene");
	MessageBGM_Play msg{ "Tutorial_BGM" };
	CORE->BroadcastMessage(&msg);

	exiting_ = CORE->GetManager<TransitionManager>()->ResetTransition("Credits", &m_MenuState);

	timer_ = set_time_;

	exiting_ = true;
}


void CreditsState::Free() {

	CORE->SetMovementLock(false);

	//CORE->GetSystem<SoundSystem>()->StopSound("All", true);
	CORE->ResetGodMode();
	CORE->ResetCorePauseStatus();
	CORE->ResetGamePauseStatus();
	CORE->GetSystem<SoundSystem>()->StopSound("All", true);
	FACTORY->DestroyAllEntities();
}


void CreditsState::Update(Game* game, float frametime) {

	(void)game;
	if (exiting_) {

		timer_ -= frametime;

		if (timer_ < 0.0f) {

			exiting_ = false;
			timer_ = set_time_;
		}
	}
}


void CreditsState::Draw(Game* game) {

	(void)game;
}


void CreditsState::StateInputHandler(Message* msg, Game* game) {

	if (game && !exiting_) {

		switch (msg->message_id_) {
			//check for collision between button & mouse
		case MessageIDTypes::M_BUTTON_TRIGGERED: {

			Message_Input* m = dynamic_cast<Message_Input*>(msg);

			switch (m->input_)
			{
			case GLFW_KEY_ENTER:
			{

				CORE->GetManager<TransitionManager>()->ResetCurrentTransitionTimer();
				exiting_ = true;
				break;
			}
			}
			break;
		}
		}
	}
}