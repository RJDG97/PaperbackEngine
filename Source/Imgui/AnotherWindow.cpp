#include "ImguiWindows/AnotherWindow.h"
#include "Systems/Debug.h"
#include <tuple>


void AnotherWindow::Init(){

	imgui_system_ = &*CORE->GetSystem<ImguiSystem>();
}

void AnotherWindow::Update(){

	ImGui::Begin("Debug Window");
	
	if (ImGui::Button("Debug")){ // debug bomb (show collision boxes)

		Message msg(MessageIDTypes::DEBUG_ALL);
		CORE->BroadcastMessage(&msg);
	}

	std::pair<Entity*, std::vector<ComponentTypes>> entitycomp = imgui_system_->GetSelectedEntity();
	if (entitycomp.first){

		ImGui::Text("Entity ID: %d, has the following components: ", entitycomp.first->GetID());

		for (auto i : entitycomp.second){

			ImGui::Text("[%d]", i);
		}
	}
	ImGui::End();
}


