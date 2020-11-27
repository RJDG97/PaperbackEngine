#include "Engine/Core.h"
#include <iostream>
#include "Systems/Physics.h"
#include "Systems/InputSystem.h"
#include "Systems/WindowsSystem.h"

// Global pointer to core engine
std::unique_ptr<CoreEngine> CORE;

CoreEngine::CoreEngine() :
	debug_{ false },
	pause_{ false },
	game_pause_{ false },
	b_game_active_{ true },
	global_scale_{ 64.0f }
{
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
		
		for (int steps = 0; steps < PE_FrameRate.GetSteps(); ++steps)
		{
			if (input->IsKeyTriggered(GLFW_KEY_B)) {
				debug_ = !debug_;
			}

			if (debug_)
				M_DEBUG->WriteDebugMessage("Core Engine System Update:\n");

			PE_FrameRate.FrameRateLoop();

			glfwSetWindowTitle(win->ptr_window, (win->GetWindowName() + " | " + std::to_string(PE_FrameRate.GetFPS()) + " FPS").c_str());

			//if (CORE->GetSystem<InputSystem>()->IsKeyTriggered(GLFW_KEY_ESCAPE)) { // Q key
			//	M_DEBUG->WriteDebugMessage("TERMINATE GAME LOOP\n");
			//	b_game_active_ = false;
			//}

			for (SystemIt system = systems_.begin(); system != systems_.end(); ++system) {
				// Placeholder
				PE_FrameRate.StartSystemTimer();

				system->second->Update(PE_FrameRate.GetFixedDelta());
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
	}
	glfwTerminate();

	//PE_FrameRate.SetFPS(30);	
}

float CoreEngine::GetGlobalScale() const {

	return global_scale_;
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

bool CoreEngine::GetCorePauseStatus() {
	return pause_;
}

void CoreEngine::ResetCorePauseStatus() {
	pause_ = false;
}

void CoreEngine::ToggleCorePauseStatus() {
	pause_ = !pause_;
}

bool CoreEngine::GetGamePauseStatus() {
	return game_pause_;
}

void CoreEngine::ResetGamePauseStatus() {
	game_pause_ = false;
}

void CoreEngine::ToggleGamePauseStatus() {
	game_pause_ = !game_pause_;
}

void CoreEngine::SetGameActiveStatus(bool status) {
	b_game_active_ = status;
}