#include "Core.h"
#include <iostream>
#include "Physics.h"
#include "InputSystem.h"
//#include "../WinKeyCodes.h"

// Global pointer to core engine
CoreEngine* CORE;

CoreEngine::CoreEngine() {
	GameActive = true;
	CORE = this;
}

///Initializes all systems in the game.
void CoreEngine::Initialize() {

	for (size_t i = 0; i < Systems.size(); ++i) {
		Systems[i]->Init();
	}
}

///Update all the systems until the game is no longer active.
void CoreEngine::GameLoop() {
	while (GameActive) {
		
		PE_FrameRate.FrameRateLoop();

		if (sys_input_.CheckTriggeredInput(0x51)) { // Q key

			GameActive = false;
		}

		for (size_t i = 0; i < Systems.size(); ++i) {
			Systems[i]->Update(PE_FrameRate.Dt);
		}

		//PE_FrameRate.SetFPS(30);
	}
}

///Destroy all systems in reverse order that they were added.
void CoreEngine::DestroySystems() {
	
	for (unsigned int i = 0; i < Systems.size(); ++i) {

		delete Systems[Systems.size() - i - 1];
	}
}

///Broadcasts a message to all systems.
void CoreEngine::BroadcastMessage(Message* m) {
	
	if (m->message_id_ == MessageIDTypes::Exit) {

		//set game bool to false
		GameActive = false;
	}

	for (unsigned int i = 0; i < Systems.size(); ++i) {

		Systems[i]->SendMessageD(m);
	}
}

///Adds a new system to the game.
void CoreEngine::AddSystem(ISystem* system) {

	Systems.push_back(system);
}