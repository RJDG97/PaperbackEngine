/**********************************************************************************
*\file         SplashState.cpp
*\brief        Contains definition of Splash State
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "GameStates/CutSceneState.h"
#include "GameStates/PlayState.h"
#include "Manager/TransitionManager.h"
#include "Manager/DialogueManager.h"
#include "Engine/Core.h"
#include "Systems/Factory.h"
#include "Systems/DialogueSystem.h"
#include "Systems/Parenting.h"

CutSceneState m_CutSceneState;

CutSceneState::CutSceneState()
{ }


void CutSceneState::Init(std::string) {
	
	component_mgr_ = &*CORE->GetManager<ComponentManager>();
	entity_mgr_ = &*CORE->GetManager<EntityManager>();

	CORE->ResetGodMode();
	CORE->ResetCorePauseStatus();
	CORE->ResetGamePauseStatus();

	CORE->GetManager<LayerManager>()->LoadLevelLayers("Cutscene");
	FACTORY->LoadLevel("Cutscene");

	//used to skip transitions if they dont exist
	std::string name = CORE->GetSystem<EntityFactory>()->GetLevelsFile()->GetLastPlayLevel()->name_;
	exiting_ = !CORE->GetManager<TransitionManager>()->ResetTransition(name, &m_PlayState);

	if (exiting_) {

		CORE->GetManager<TransitionManager>()->ResetTransition("Default", &m_PlayState);
	}
	else {

		/*MessageBGM_Play msg{ "MenuDefault" };
		CORE->BroadcastMessage(&msg);*/

		CORE->GetSystem<ParentingSystem>()->LinkParentAndChild();

		CORE->GetManager<DialogueManager>()->LoadDialogueSet("Cutscene_" + name);
		CORE->GetSystem<DialogueSystem>()->SetCurrentDialogue("text");

		timer_ = set_time_ / 2;

		exiting_ = true;
	}
}


void CutSceneState::Free() {

	CORE->GetSystem<DialogueSystem>()->TempCleanup();

	CORE->SetMovementLock(false);

	CORE->GetSystem<SoundSystem>()->StopSound("All", true);
	CORE->ResetGodMode();
	CORE->ResetCorePauseStatus();
	CORE->ResetGamePauseStatus();

	FACTORY->DestroyAllEntities();
}


void CutSceneState::Update(Game* game, float frametime) {
	
	(void)game;
	if (exiting_) {

		timer_ -= frametime;

		if (timer_ < 0.0f) {

			exiting_ = false;
		}
	}
}


void CutSceneState::Draw(Game* game) {
	
	(void)game;
}


void CutSceneState::StateInputHandler(Message* msg, Game* game) {
	
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
				CORE->GetSystem<DialogueSystem>()->AdvanceText();
				exiting_ = true;
				break;
			}
			}
			break;
		}
		}
	}
}