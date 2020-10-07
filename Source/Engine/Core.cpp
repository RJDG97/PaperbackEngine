#include "Engine/Core.h"
#include <iostream>
#include "Systems/Physics.h"
#include "Systems/InputSystem.h"
#include "Systems/WindowsSystem.h"
//#include "../WinKeyCodes.h"

// Global pointer to core engine
std::unique_ptr<CoreEngine> CORE;

CoreEngine::CoreEngine() {
	b_game_active_ = true;
	debug_ = false;
}

///Initializes all Systems & Managers in the game.
void CoreEngine::Initialize() {

	M_DEBUG->WriteDebugMessage("Core Engine System Init\n");

	for (ManagerIt manager = managers_.begin(); manager != managers_.end(); ++manager) {
		manager->second->Init();
	}

	for (SystemIt system = systems_.begin(); system != systems_.end(); ++system) {
		system->second->Init();
	}
}

///Update all the systems until the game is no longer active.
void CoreEngine::GameLoop() {
	// Get a pointer to Windows System
	WindowsSystem* win = CORE->GetSystem<WindowsSystem>();

	while (b_game_active_ && !glfwWindowShouldClose(win->ptr_window)) {
		if (debug_)
			M_DEBUG->WriteDebugMessage("Core Engine System Update:\n");
		
		PE_FrameRate.FrameRateLoop();

		//std::cout << PE_FrameRate.GetFPS() << std::endl;
		glfwSetWindowTitle(win->ptr_window, (win->GetWindowName() + " " + std::to_string(PE_FrameRate.GetFPS())).c_str());

		if (CORE->GetSystem<InputSystem>()->IsKeyTriggered(GLFW_KEY_Q)) { // Q key
			M_DEBUG->WriteDebugMessage("TERMINATE GAME LOOP\n");
			b_game_active_ = false;
		}

		//uncomment for testing, other functions using this will not work with this active
		//if (CORE->GetSystem<InputSystem>()->IsMouseTriggered(GLFW_MOUSE_BUTTON_LEFT))
		//{
		//	std::cout << CORE->GetSystem<InputSystem>()->GetCursorPosition().x << ": " << CORE->GetSystem<InputSystem>()->GetCursorPosition().y << std::endl;
		//}

		for (SystemIt system = systems_.begin(); system != systems_.end(); ++system) {
			if (debug_)
				// Log system message to "Source/Debug.txt"
				M_DEBUG->WriteDebugMessage("Begining update for: " + system->second->GetName() + "\n");
			system->second->Update(PE_FrameRate.dt_);
			system->second->Draw();
		}

		glfwSwapBuffers(win->ptr_window);
		glfwPollEvents();
		M_DEBUG->SaveDebug();
	}
	glfwTerminate();

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
	
	if (m->message_id_ == MessageIDTypes::EXIT) {

		//set game bool to false
		b_game_active_ = false;
	}

	for (SystemIt system = systems_.begin(); system != systems_.end(); ++ system) {

		system->second->SendMessageD(m);
	}
}