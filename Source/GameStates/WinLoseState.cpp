#include "GameStates/WinLoseState.h"
#include "GameStates/MenuState.h"
#include "Systems/Factory.h"
#include "Engine/Core.h"

WinLoseState m_WinLoseState;

void WinLoseState::Init(std::string level_name) {

	FACTORY->DeSerializeLevelEntities(level_name);
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
		default:
			break;
	}
}

std::string WinLoseState::GetStateName() {
	
	return "WinLose";
}