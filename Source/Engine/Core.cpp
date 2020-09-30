#include "Engine/Core.h"
#include <iostream>
#include "Systems/Physics.h"
#include "Systems/InputSystem.h"
//#include "../WinKeyCodes.h"

// Global pointer to core engine
CoreEngine* CORE;

CoreEngine::CoreEngine() {
	b_game_active_ = true;
	debug_ = false;
	CORE = this;
}

///Initializes all Systems & Managers in the game.
void CoreEngine::Initialize() {

	for (SystemIt system = systems_.begin(); system != systems_.end(); ++system) {
		system->second->Init();
	}

	for (ManagerIt manager = managers_.begin(); manager != managers_.end(); ++manager) {
		manager->second->Init();
	}
}

///Update all the systems until the game is no longer active.
void CoreEngine::GameLoop() {
	while (b_game_active_) {
		if (debug_)
			M_DEBUG->WriteDebugMessage("Core Engine Update:\n");
		
		PE_FrameRate.FrameRateLoop();

		if (sys_input_.CheckTriggeredInput(0x51)) { // Q key
			M_DEBUG->WriteDebugMessage("TERMINATE GAME LOOP\n");
			b_game_active_ = false;
		}

		for (SystemIt system = systems_.begin(); system != systems_.end(); ++system) {
			if (debug_)
				// Log system message to "Source/Debug.txt"
				M_DEBUG->WriteDebugMessage("Begining update for: " + system->second->GetName() + "\n");
			system->second->Update(PE_FrameRate.Dt);
			system->second->Draw();
		}
	}

		//PE_FrameRate.SetFPS(30);
	
}

///Destroy all systems in reverse order that they were added.
void CoreEngine::DestroySystems() {
	
	for (SystemIt system = systems_.begin(); system != systems_.end(); ++system) {
		M_DEBUG->WriteDebugMessage("Destroying System: " + system->second->GetName() + "\n");
		delete system->second;
	}
}

///Destroy all systems in reverse order that they were added.
void CoreEngine::DestroyManagers() {
	
	for (ManagerIt manager = managers_.begin(); manager != managers_.end(); ++manager) {
		// Log system message to "Source/Debug.txt"
		M_DEBUG->WriteDebugMessage("Destroying Manager: " + manager->first + "\n");
		delete manager->second;
	}
}

///Broadcasts a message to all systems_.
void CoreEngine::BroadcastMessage(Message* m) {
	
	if (m->message_id_ == MessageIDTypes::Exit) {

		//set game bool to false
		b_game_active_ = false;
	}

	for (SystemIt system = systems_.begin(); system != systems_.end(); ++ system) {

		system->second->SendMessageD(m);
	}
}