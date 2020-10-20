#include "Imgui/AnotherWindow.h"


void AnotherWindow::Init(){

}

void AnotherWindow::Update(){

	ImGui::Begin("Another Test Window");
	ImGui::Text("Hello from another window!");
	ImGui::Button("Help Me");
	ImGui::End();
}