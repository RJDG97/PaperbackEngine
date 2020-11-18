#include "ImguiWindows/ImguiViewport.h"
#include "Engine/Core.h"

void ImguiViewport::Init() {

    viewport_size_ = ImVec2{ 0.0f, 0.0f };
	editor_viewport_ = &*CORE->GetSystem<GraphicsSystem>();
	//a = 0;
	//b = 0;
}

void ImguiViewport::Update() {

	/*ImGui::Begin("viewport");*/
	//ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	//viewport_size_ = { viewportPanelSize.x, viewportPanelSize.y };
	//uint64_t textureID = editor_viewport_->GetFramebuffer();
	//ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{viewport_size_.x,viewport_size_.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
//	int a = GetA();
//	int b = GetB();
//	int c = 1;
//	ImGui::SliderInt("a", &a, 1, 100, "%d");
//	ImGui::SliderInt("b", &b, 1, 100, "%d");
//
//	SetA(a);
//	SetB(b);
//
//	for (int i = 0; i < a; ++i) {
//		for (int j = 0; j < b; ++j) {
//			ImGui::InputInt("ab", &c);
//		}
//	}
//		//
//
//	
//	ImGui::End();
//
//}
//
//int ImguiViewport::GetA()
//{
//	return a;
//}
//
//void ImguiViewport::SetA(int A)
//{
//	a = A;
//}
//
//int ImguiViewport::GetB()
//{
//	return b;
//}
//
//void ImguiViewport::SetB(int B)
//{
//	b = B;
}
