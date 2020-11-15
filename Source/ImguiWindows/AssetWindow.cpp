#include "ImguiWindows\AssetWindow.h"


void AssetWindow::Init() {
	imgui_ = &*CORE->GetSystem<ImguiSystem>();
}

void AssetWindow::Update() {
	ImGui::Begin("Asset Browser");
	for (ImguiSystem::directoryfileit it = imgui_->directory_map_.begin(); it != imgui_->directory_map_.end(); ++it)
		ImGui::Text(it->first.c_str());
	ImGui::End();
}





