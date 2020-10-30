#include <iostream>

#include "Systems/Game.h"
#include "GameStates/MenuState.h"
#include "GameStates/PlayState.h"

#include "Systems/InputSystem.h"
#include "Systems/WindowsSystem.h"
#include "Manager/TextureManager.h"
#include "Manager/AnimationManager.h"

#include "Engine/Core.h" //FOR TESTING

#include "Systems/GraphicsSystem.h"
#include "Systems/Factory.h"
#include "Systems/Collision.h"
#include "Systems/ImguiSystem.h"

#include "Components/Status.h"

#include "Components/Transform.h"

//SAMPLE MENU STATE

MenuState m_MenuState;

void MenuState::Init()
{
	std::cout << "-----------------------------" << std::endl << std::endl;
	std::cout << "MenuState init Successful" << std::endl;
	std::cout << "Press SPACE to START" << std::endl;
	std::cout << "Press ESC to QUIT" << std::endl << std::endl;
	std::cout << "-----------------------------" << std::endl << std::endl;

	// Entities created within cannot be checked against directly (No * to entity)
	FACTORY->DeSerializeLevelEntities("Menu");
}

void MenuState::Free()
{
	std::cout << "MenuState clean Successful" << std::endl;

	FACTORY->DestroyAllEntities();

	CORE->GetSystem<ImguiSystem>()->ResetSelectedEntity();
}

void MenuState::Update(Game* game, float frametime)
{
	UNREFERENCED_PARAMETER(game);
	UNREFERENCED_PARAMETER(frametime);
}

void MenuState::Draw(Game* game)
{
	//GraphicsSystem::Instance().Draw();
	UNREFERENCED_PARAMETER(game);
}

void MenuState::StateInputHandler(Message* msg, Game* game) {

	if (game && !game->debug_) {

		switch (msg->message_id_) {
			//check for collision between button & mouse
		case MessageIDTypes::BUTTON: {

			Message_Button* m = dynamic_cast<Message_Button*>(msg);

			if (m->button_index_ == 3) {

				//true returned, trigger scene change
				game->ChangeState(&m_PlayState);
				return;
			}

			if (m->button_index_ == 5) {

				DEBUG_ASSERT(false, "Forced Crash Triggered");
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