#include <iostream>
#include "Systems/Game.h"
#include "GameStates/GameState.h"
#include "Systems/InputSystem.h"
#include "Engine/Core.h"
#include "GameStates/MenuState.h"
#include "GameStates/PlayState.h"
#include "Systems/Debug.h"
#include <algorithm>

bool Game::CheckExist(GameState* compare) {
	if (!states_.empty() && states_.back() == compare) {
		std::cout << "Error: State already exists on stack" << std::endl;
		return true;
	}
	return false;
}

Game::Game() :
	debug_{ false }
{}

void Game::Init()
{
	b_running_ = true;

	//CORE->GetManager<TextureManager>()->TempTextureBatchLoad();
	//CORE->GetManager<AnimationManager>()->TempAnimationBatchLoad();
	FACTORY->CreateAllArchetypes("Resources/EntityConfig/archetypes.json");

	ComponentManager* comp_mgr = &*CORE->GetManager<ComponentManager>();

	input_controller_arr_ = comp_mgr->GetComponentArray<InputController>();
	status_arr_ = comp_mgr->GetComponentArray<Status>();
	basicai_arr_ = comp_mgr->GetComponentArray<BasicAI>();

	CORE->GetManager<TextureManager>()->TextureBatchLoad("Menu");
	CORE->GetManager<AnimationManager>()->AnimationBatchLoad("Menu");
	CORE->GetManager<FontManager>()->FontBatchLoad("Menu");

	CORE->GetManager<TextureManager>()->TextureBatchLoad("Play");
	CORE->GetManager<AnimationManager>()->AnimationBatchLoad("Play");
	CORE->GetManager<FontManager>()->FontBatchLoad("Play");

	ChangeState(&m_MenuState);

	M_DEBUG->WriteDebugMessage("Game System Init\n");
}

// takes a pointer to a gamestate
void Game::ChangeState(GameState* state, std::string level_name)
{

	// Remove current state
	if (!states_.empty()) {
	
		states_.back()->Free();
		states_.pop_back();
	}

	states_.push_back(state);
	states_.back()->Init(level_name);
	
	// Backup copy
	/*
	if (!CheckExist(state)) {
		// Remove current state
		if (!states_.empty()) {
		
			states_.back()->Free();
			states_.pop_back();
		}

		states_.push_back(state);
		states_.back()->Init(level_name);
	}
	*/
}

// temporarily pushes a new state onto the stack
void Game::PushState(GameState* state) // need to check if current state already exists in stack
{
	(void)state;
}

void Game::PopState()
{
	// cleanup the current state
	if (!states_.empty())
	{
		states_.back()->Free();
		states_.pop_back();
	}
}

void Game::Update(float frametime)
{

	// let the current state take control
	if (!states_.empty()) {
	
		states_.back()->Update(this, frametime);
	}
}

void Game::Draw()
{
	// let the current state take control
	states_.back()->Draw(this);
}

void Game::Free()
{
	while (!states_.empty())
	{
		states_.back()->Free();
		states_.pop_back();
	}
}

std::string Game::GetStateName() {

	if (!states_.empty())
		return states_.back()->GetStateName();

	// Do check the return string
	return {};
}

void Game::SendMessageD(Message* m) {

	//assume all messages of concern to game are only input message
	switch (m->message_id_) {

	case MessageIDTypes::M_BUTTON_PRESS:
	case MessageIDTypes::M_BUTTON_TRIGGERED:
	{
		Message_Input* msg = dynamic_cast<Message_Input*>(m);

		if (msg && msg->input_ == GLFW_KEY_B) {

			debug_ = !debug_;
			break;
		}

		//forward onto state to handle
		states_.back()->StateInputHandler(m, this);
		break;
	}
	case MessageIDTypes::BUTTON:
	{
		states_.back()->StateInputHandler(m, this);
		break;
	}
	case MessageIDTypes::C_MOVEMENT:
	{
		//value larger than what can be gotten from input flags
		states_.back()->StateInputHandler(m);
		break;
	}
	}
}