#include "ImguiWindows/ImguiViewport.h"
#include "Engine/Core.h"


void ImguiViewport::Init() {
    viewportsize_ = ImVec2{ 0.0f, 0.0f };
	editorviewport = &*CORE->GetSystem<GraphicsSystem>();
}

void ImguiViewport::Update() {

	ImGui::Begin("viewport");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	viewportsize_ = { viewportPanelSize.x, viewportPanelSize.y };
	uint64_t textureID = editorviewport->getFramebuffer();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{viewportsize_.x,viewportsize_.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();
}