#include "ImguiWindows/ImguiViewport.h"
#include "Engine/Core.h"

void ImguiViewport::Init() {

    viewport_size_ = ImVec2{ 0.0f, 0.0f };
	editor_viewport_ = &*CORE->GetSystem<GraphicsSystem>();
}

void ImguiViewport::Update() {

	ImGui::Begin("viewport");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	viewport_size_ = { viewportPanelSize.x, viewportPanelSize.y };
	uint64_t textureID = editor_viewport_->GetFramebuffer();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{viewport_size_.x,viewport_size_.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();

}