#include "Engine/Core.h"
#include <iostream>
#include "Systems/Physics.h"
#include "Systems/InputSystem.h"
#include "Systems/WindowsSystem.h"

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
	WindowsSystem* win = &*CORE->GetSystem<WindowsSystem>();
	InputSystem* input = &*CORE->GetSystem<InputSystem>();

	while (b_game_active_ && !glfwWindowShouldClose(win->ptr_window)) {
		// Placeholder (Game's logic component)
		if (input->IsKeyTriggered(GLFW_KEY_B)) {
			debug_ = !debug_;
		}

		if (debug_)
			M_DEBUG->WriteDebugMessage("Core Engine System Update:\n");
		
		PE_FrameRate.FrameRateLoop();

		glfwSetWindowTitle(win->ptr_window, (win->GetWindowName() + " | " + std::to_string(PE_FrameRate.GetFPS()) + " FPS").c_str());

		if (CORE->GetSystem<InputSystem>()->IsKeyTriggered(GLFW_KEY_ESCAPE)) { // Q key
			M_DEBUG->WriteDebugMessage("TERMINATE GAME LOOP\n");
			b_game_active_ = false;
		}

		for (SystemIt system = systems_.begin(); system != systems_.end(); ++system) {
			// Placeholder
			PE_FrameRate.StartSystemTimer();

			system->second->Update(PE_FrameRate.dt_);
			system->second->Draw();

			PE_FrameRate.EndSystemTimer();

			if (debug_) {
				// Log system message to "Source/Debug.txt"
				M_DEBUG->WriteDebugMessage("Begining update for: " + system->second->GetName() + "\n");
				// Placeholder
				PE_FrameRate.SetSystemPerformance(&*(system->second));
			}
		}

		// Placeholder
		if (debug_) {
			PE_FrameRate.PrintSystemPerformance();
			debug_ = !debug_;
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
		//delete &*(system->second);
	}
}

///Destroy all systems in reverse order that they were added.
void CoreEngine::DestroyManagers() {
	
	for (ManagerIt manager = managers_.begin(); manager != managers_.end(); ++manager) {
		// Log system message to "Source/Debug.txt"
		M_DEBUG->WriteDebugMessage("Destroying Manager: " + manager->first + "\n");
		//delete &*(manager->second);
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
	};
}