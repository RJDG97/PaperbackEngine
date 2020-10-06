#include <iostream>
#include "Systems/Game.h"
#include "GameStates/GameState.h"
#include "Systems/InputSystem.h"
#include "Engine/Core.h"
#include "GameStates/MenuState.h"
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

	ChangeState(&m_MenuState);

	M_DEBUG->WriteDebugMessage("Game System Init\n");
}

// takes a pointer to a gamestate
void Game::ChangeState(GameState* state)
{

	if (!CheckExist(state)) {
		// Remove current state
		if (!states_.empty()) {
		
			states_.back()->Free();
			states_.pop_back();
		}

		states_.push_back(state);
		states_.back()->Init();
	}

	/*
	// cleanup the current state
	if (!states_.empty())
	{
		// free(); removes the old state from the stack
		states_.back()->free();
		states_.pop_back();
	}

	// store and init the new state
	states_.push_back(state);
	states_.back()->init();
	// back(); refers to the last element on the stack
	*/
}

// temporarily pushes a new state onto the stack
void Game::PushState(GameState* state) // need to check if current state already exists in stack
{
	(void)state;
	// pause the current state
	/*if (!states_.empty())
	{
		states_.back()->pause();
	}
	
	//store and init the new state
	if (std::find_if(states_.begin(), states_.end(), CheckExist) == states_.end()) {
		states_.push_back(state);
		states_.back()->init();
		std::cout << "States in stack after push: " << states_.size() << std::endl;
	}*/
}

void Game::PopState()
{
	// cleanup the current state
	if (!states_.empty())
	{
		states_.back()->Free();
		states_.pop_back();
	}

	//resume the previous state
	/*if (!states_.empty())
	{
		states_.back()->resume();
	}*/

	//std::cout << "States in stack after pop: " << states_.size() << std::endl;
}

void Game::Update(float frametime)
{

	// let the current state take control
	if (!states_.empty()) {
	
		states_.back()->Update(this, frametime);
	}
}

void Game::AddStatusComponent(EntityID id, Status* status) {

	M_DEBUG->WriteDebugMessage("Adding Status Component to entity: " + std::to_string(id) + "\n");
	status_arr_[id] = status;
}

void Game::RemoveStatusComponent(EntityID id) {

	StatusIt it = status_arr_.find(id);

	if (it != status_arr_.end()) {

		M_DEBUG->WriteDebugMessage("Removing Status Component from entity: " + std::to_string(id) + "\n");
		status_arr_.erase(it);
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

void Game::SendMessageD(Message* m) {

	//std::cout << "Message received by Game" << std::endl;

	switch (m->message_id_) {

	case MessageIDTypes::GSM_PUSHSTATE:
	{
		Message_CustomState* msg = dynamic_cast<Message_CustomState*>(m);
		PushState(msg->state_);
		break;
	}
	case MessageIDTypes::GSM_CHANGESTATE:
	{	
		Message_CustomState* msg = dynamic_cast<Message_CustomState*>(m);
		ChangeState(msg->state_);
		break;
	}
	case MessageIDTypes::GSM_PAUSESTATE:
	{
		break;
	}
	case MessageIDTypes::GSM_POPSTATE:
	{
		PopState();
		break;
	}
	case MessageIDTypes::M_BUTTON_PRESS: 
	{
		//std::cout << "GSM: Button press event, dispatching to Game State" << std::endl;
		//Message_PlayerInput* msg = dynamic_cast<Message_PlayerInput*>(m);
		
		states_.back()->StateInputHandler(m, this);
		break;
	}
	case MessageIDTypes::M_MOVEMENT:
	{
		//value larger than what can be gotten from input flags
		states_.back()->StateInputHandler(m);
		break;
	}
	case MessageIDTypes::M_MOUSE_PRESS:
	{
		states_.back()->StateInputHandler(m, this);
		break;
	}
	case MessageIDTypes::DEBUG_ALL:
	{

		debug_ = true;
	}
	default:
		break;
	}
}