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
	FACTORY->DeSerializeLevelEntities("Resources/EntityConfig/menu.json");
}

void MenuState::Free()
{
	std::cout << "MenuState clean Successful" << std::endl;

	FACTORY->DestroyAllEntities();
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

	if (!game && msg->message_id_ == MessageIDTypes::M_MOVEMENT) {

		Message_PlayerInput* m = dynamic_cast<Message_PlayerInput*>(msg);
		assert(m != nullptr && "Message is not a player input message");
		unsigned char key_val = m->input_flag_;

		// set up velocity based input flag value
		Vec2 new_vel{};

		if (key_val & UP_FLAG) {

			new_vel.y += 100.0f;
		}

		if (key_val & DOWN_FLAG) {

			new_vel.y -= 100.0f;
		}

		if (key_val & LEFT_FLAG) {

			new_vel.x -= 100.0f;
		}

		if (key_val & RIGHT_FLAG) {

			new_vel.x += 100.0f;
		}

		MessagePhysics_Motion m2{ MessageIDTypes::PHY_UPDATE_VEL, new_vel };
		CORE->BroadcastMessage(&m2);
	}

	if (game && msg->message_id_ == MessageIDTypes::BUTTON) {

		//check for collision between button & mouse
		Message_Button* m = dynamic_cast<Message_Button*>(msg);

		if (m->button_index_ == 3) {
			
			//true returned, trigger scene change
			game->ChangeState(&m_PlayState);
			return;
		}

		if (m->button_index_ == 5) {

			DEBUG_ASSERT(false, "Forced Crash Triggered");
		}
	}
}