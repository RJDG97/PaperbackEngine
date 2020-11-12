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

#include "Engine/Core.h" //FOR TESTING

#include "Systems/GraphicsSystem.h"
#include "Systems/Factory.h"
#include "Systems/Collision.h"
#include "Systems/ImguiSystem.h"

#include "Components/Status.h"

#include "Components/Transform.h"

//SAMPLE MENU STATE

MenuState m_MenuState;

void MenuState::Init(std::string)
{
	std::cout << "-----------------------------" << std::endl << std::endl;
	std::cout << "MenuState init Successful" << std::endl;
	std::cout << "Press SPACE to START" << std::endl;
	std::cout << "Press ESC to QUIT" << std::endl << std::endl;
	std::cout << "-----------------------------" << std::endl << std::endl;

	CORE->ResetCorePauseStatus();
	// Entities created within cannot be checked against directly (No * to entity)

	FACTORY->LoadLevel("Menu");

	MessageBGM_Play msg{ "MenuDefault" };
	CORE->BroadcastMessage(&msg);
}

void MenuState::Free()
{
	std::cout << "MenuState clean Successful" << std::endl;

	MessageBGM_Stop msg{ "MenuDefault" };
	CORE->BroadcastMessage(&msg);

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

std::string MenuState::GetStateName() {

	return "Menu";
}

void MenuState::StateInputHandler(Message* msg, Game* game) {

	if (game && !game->debug_) {

		switch (msg->message_id_) {
			//check for collision between button & mouse
		case MessageIDTypes::BUTTON: {

			Message_Button* m = dynamic_cast<Message_Button*>(msg);

			MessageBGM_Play button{ "ButtonPress" };
			CORE->BroadcastMessage(&button);

			if (m->button_index_ == 1) {

				// Enter play state
				game->ChangeState(&m_PlayState);
				return;
			}

			if (m->button_index_ == 2) {

				// "How to play"
				return;
			}

			if (m->button_index_ == 3) {

				// Editor mode
				CORE->GetSystem<ImguiSystem>()->SetImguiBool(true);
				game->ChangeState(&m_EditorState);
				return;
			}

			if (m->button_index_ == 4) {

				// Toggle off game
				CORE->SetGameActiveStatus(false);
				return;
			}

			if (m->button_index_ == 5) {

				// Enter "Win" state
				game->ChangeState(&m_WinLoseState, "Win"); // convert to quit game
				return;
			}

			if (m->button_index_ == 6) {

				// Enter "Lose" state
				game->ChangeState(&m_WinLoseState, "Lose"); // convert to quit game
				return;
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