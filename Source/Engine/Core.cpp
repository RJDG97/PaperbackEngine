/**********************************************************************************
*\file         Core.cpp
*\brief        Contains definition of functions and variables used for
*			   the Core Engine System
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


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
	god_mode_{ false },
	game_pause_{ false },
	movement_lock_{ false },
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

	while (b_game_active_ && !glfwWindowShouldClose(win->ptr_window)) {
		
		for (int steps = 0; steps < PE_FrameRate.GetSteps(); ++steps)
		{
			if (debug_)
				M_DEBUG->WriteDebugMessage("Core Engine System Update:\n");

			PE_FrameRate.FrameRateLoop();

			glfwSetWindowTitle(win->ptr_window, win->GetWindowName().c_str());

			for (SystemIt system = systems_.begin(); system != systems_.end(); ++system) {
				
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

bool CoreEngine::GetGodMode() {
	return god_mode_;
}

void CoreEngine::SetGodMode(bool status) {

	god_mode_ = status;
}

void CoreEngine::ResetGodMode() {
	god_mode_ = false;
}

void CoreEngine::ToggleGodMode() {
	god_mode_ = !god_mode_;
}

bool CoreEngine::GetMovementLock() {
	return movement_lock_;
}

void CoreEngine::SetMovementLock(bool status) {

	movement_lock_ = status;
}