#include "Core.h"
#include <iostream>
#include "Physics.h"
#include "../InputSystem.h"
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
		Systems[i]->init();
	}
}

///Update all the systems until the game is no longer active.
void CoreEngine::GameLoop() {
	while (GameActive) {
		
		if (sys_input.CheckTriggeredInput(0x51)) { // Q key

			GameActive = false;
		}

		if (sys_input.CheckTriggeredInput(0x39)) { // 9 key on number row
			std::cout << "triggered general rotation" << std::endl;

			MessageRotation msg{ 1 };
			CORE->BroadcastMessage(&msg);
		}

		if (sys_input.CheckTriggeredInput(0x30)) { // 0 key on number row

			std::cout << "triggered general hp decrement" << std::endl;

			MessageHPDecre msg{ 2 };
			CORE->BroadcastMessage(&msg);
		}

		for (size_t i = 0; i < Systems.size(); ++i) {
			Systems[i]->update(PE_FrameRate.Dt);
		}
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
	
	if (m->MessageID == MessageIDTypes::Exit) {

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