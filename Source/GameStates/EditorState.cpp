
#include <iostream>

#include "Systems/Game.h"
#include "GameStates/EditorState.h"
#include "GameStates/PlayState.h"

#include "Systems/InputSystem.h"

#include "Engine/Core.h"
#include "Systems/Factory.h"

#include "Components/Transform.h"
#include "Components/Motion.h"
#include "Components/Name.h"

#include "Entity/ComponentTypes.h"

#include "Manager/ForcesManager.h"

#include <memory>

EditorState m_EditorState;

//demo pointer to player
//Entity* player;

void EditorState::Init(std::string)
{
	std::cout << "-----------------------------" << std::endl << std::endl;
	std::cout << "EditorState init Successful" << std::endl;
	std::cout << "press SPACE to PAUSE" << std::endl;
	std::cout << "press ESCAPE to return to MAIN MENU" << std::endl << std::endl;
	std::cout << "-----------------------------" << std::endl << std::endl;
	
	//used to freeze logic update and keep entities stationary except for actions tied to input
	pause_ = true;

	//TEMPORARY
	//CORE->GetSystem<CameraSystem>()->SetTarget(player);
	
	CORE->ResetCorePauseStatus();
	FACTORY->LoadLevel("Editor");
}

void EditorState::Free()
{
	std::cout << "EditorState clean Successful" << std::endl;

	FACTORY->DestroyAllEntities();
}

void EditorState::Update(Game* game, float frametime)
{
	if (!pause_) 
		m_PlayState.Update(game, frametime);
}

void EditorState::Draw(Game* game)
{
	m_PlayState.Draw(game);
}

std::string EditorState::GetStateName() {

	return "Editor";
}

void EditorState::StateInputHandler(Message* msg, Game* game) {

	if (msg->message_id_ == MessageIDTypes::M_BUTTON_PRESS) {

		Message_Input* m = dynamic_cast<Message_Input*>(msg);
		switch (m->message_id_) {

			case MessageIDTypes::M_BUTTON_PRESS:
			{

				if (m->input_ == GLFW_KEY_P) {

					pause_ = !pause_;
				}
			}
		}
	}

	m_PlayState.StateInputHandler(msg, game);
}