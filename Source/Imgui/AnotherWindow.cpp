#include "ImguiWindows/AnotherWindow.h"
#include "Systems/Debug.h"
#include "Entity/ComponentTypes.h"
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

	if (imgui_system_->b_debug)
	{
		std::pair<Entity*, std::vector<ComponentTypes>> entitycomp = imgui_system_->GetSelectedEntity();
		//if (entitycomp.first) {

		//	ImGui::Text("Entity ID: %d, has the following components: ", entitycomp.first->GetID());

		//	for (auto i : entitycomp.second) {

		//		ImGui::Text("[%d]", i);
		//	}
		//}

		Component(entitycomp);
	}
	ImGui::End();
}

void AnotherWindow::Component(std::pair<Entity*, std::vector<ComponentTypes>> entitycomponent)
{
	if (entitycomponent.first)
	{
		for (auto componenttype : entitycomponent.second)
		{
			switch (componenttype)
			{
				case ComponentTypes::NAME:
				{
					std::shared_ptr<Name> entityname = std::dynamic_pointer_cast<Name>(entitycomponent.first->GetComponent(ComponentTypes::NAME));
					std::string label = { "Entity: " };
					label += entityname->GetName();
					ImGui::Text(label.c_str());
					break;

				}
			}
		}
	}
}


