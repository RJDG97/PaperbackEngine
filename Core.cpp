#include "Core.h"
#include <iostream>
#include "Physics.h"
#include "InputSystem.h"
//#include "../WinKeyCodes.h"

// Global pointer to core engine
CoreEngine* CORE;

CoreEngine::CoreEngine() {
	b_game_active_ = true;
	CORE = this;
}

///Initializes all systems in the game.
void CoreEngine::Initialize() {

	for (size_t i = 0; i < systems_.size(); ++i) {
		systems_[i]->Init();
	}
}

///Update all the systems until the game is no longer active.
void CoreEngine::GameLoop() {
	while (b_game_active_) {
		
		PE_FrameRate.FrameRateLoop();

		if (sys_input_.CheckTriggeredInput(0x51)) { // Q key

			b_game_active_ = false;
		}

		for (size_t i = 0; i < systems_.size(); ++i) {
			systems_[i]->Update(PE_FrameRate.Dt);
			systems_[i]->Draw();
		}

		//PE_FrameRate.SetFPS(30);
	}
}

///Destroy all systems in reverse order that they were added.
void CoreEngine::DestroySystems() {
	
	for (unsigned int i = 0; i < systems_.size(); ++i) {

		delete systems_[systems_.size() - i - 1];
	}
}

///Broadcasts a message to all systems_.
void CoreEngine::BroadcastMessage(Message* m) {
	
	if (m->message_id_ == MessageIDTypes::Exit) {

		//set game bool to false
		b_game_active_ = false;
	}

	for (unsigned int i = 0; i < systems_.size(); ++i) {

		systems_[i]->SendMessageD(m);
	}
}

///Adds a new system to the game.
void CoreEngine::AddSystem(ISystem* system) {

	systems_.push_back(system);
}