#include "GameStates/WinLoseState.h"
#include "GameStates/MenuState.h"
#include "GameStates/PlayState.h"
#include "Systems/Factory.h"
#include "Engine/Core.h"

WinLoseState m_WinLoseState;

void WinLoseState::Init(std::string level_name) {

	CORE->ResetCorePauseStatus();
	FACTORY->LoadLevel(level_name);
}


void WinLoseState::Update(Game* game, float frametime) {

	(void)game;
	(void)frametime;
}


void WinLoseState::Draw(Game* game) {

	(void)game;
}


void WinLoseState::Free() {

	FACTORY->DestroyAllEntities();
}


void WinLoseState::StateInputHandler(Message* msg, Game* game) {

	switch(msg->message_id_)
	{
		case MessageIDTypes::M_BUTTON_TRIGGERED:
		{
			Message_Input* m = dynamic_cast<Message_Input*>(msg);
			if (m) {
				// yeah change this input to the button checker thingies ma jig when we have buttons
				if (m->input_== GLFW_KEY_ENTER)
					game->ChangeState(&m_MenuState);
			}
			break;
		}
		case MessageIDTypes::BUTTON:
		{
			Message_Button* m = dynamic_cast<Message_Button*>(msg);

			if (m->button_index_ == 1) {

				game->ChangeState(&m_PlayState);
				return;
			}
			if (m->button_index_ == 2) {
				// "How to play"
				return;
			}
			if (m->button_index_ == 3) {
				
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