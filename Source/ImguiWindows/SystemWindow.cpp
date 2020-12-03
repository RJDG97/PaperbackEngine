/**********************************************************************************
*\file         SystemWindow.cpp
*\brief        Contains definition of functions and variables used for
*			   the SystemWindow class to display percentages used by each system

*\author	   Ee Ling Adele, Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "ImguiWindows/SystemWindow.h"
#include "Engine/Core.h"
#include "Systems/FrameRateController.h"

void SystemWindow::Init(){

	imgui_ = &*CORE->GetSystem<ImguiSystem>();
}

void SystemWindow::Update(){

	if (imgui_->b_display) {

		ImGui::Begin("System Performance Data", &imgui_->b_display);
		float total_time_ = 0.0f;
		for (std::map<std::string, float>::iterator it = PE_FrameRate.GetSystemPerformance().begin(); it != PE_FrameRate.GetSystemPerformance().end(); ++it) {
			total_time_ += it->second;
		}

		for (std::map<std::string, float>::iterator it = PE_FrameRate.GetSystemPerformance().begin(); it != PE_FrameRate.GetSystemPerformance().end(); ++it) {
			ImGui::Text(it->first.c_str()); ImGui::SameLine(0, 2); ImGui::Text(": %.4f", (it->second / total_time_ * 100.0f));
		}
		ImGui::End();
	}
}
