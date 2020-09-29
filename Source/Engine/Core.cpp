#include "Engine/Core.h"
#include <iostream>
#include "Systems/Physics.h"
#include "Systems/InputSystem.h"
//#include "../WinKeyCodes.h"

// Global pointer to core engine
CoreEngine* CORE;

CoreEngine::CoreEngine() {
	b_game_active_ = true;
	CORE = this;
}

///Initializes all Systems & Managers in the game.
void CoreEngine::Initialize() {

	for (size_t i = 0; i < systems_.size(); ++i) {
		systems_[i]->Init();
	}

	for (std::pair<std::string, IManager*> manager : managers_) {
		manager.second->Init();
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
	}

		//PE_FrameRate.SetFPS(30);
	
}

///Destroy all systems in reverse order that they were added.
void CoreEngine::DestroySystems() {
	
	for (size_t i = 0; i < systems_.size(); ++i) {

		delete systems_[systems_.size() - 1 - i];
	}
}

///Destroy all systems in reverse order that they were added.
void CoreEngine::DestroyManagers() {
	
	for (std::pair<std::string, IManager*> manager : managers_) {
		delete manager.second;
	}
}

///Broadcasts a message to all systems_.
void CoreEngine::BroadcastMessage(Message* m) {
	
	if (m->message_id_ == MessageIDTypes::Exit) {

		//set game bool to false
		b_game_active_ = false;
	}

	for (size_t i = 0; i < systems_.size(); ++i) {

		systems_[i]->SendMessageD(m);
	}
}