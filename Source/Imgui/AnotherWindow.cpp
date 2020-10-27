#include "ImguiWindows/AnotherWindow.h"
#include "Systems/Debug.h"
#include "Systems/Factory.h"
//#include "Manager/ForcesManager.h"

Entity* playertest;

void AnotherWindow::Init(){

	scale_ = { 1.0f, 1.0f };
}

void AnotherWindow::Update(){

	ImGui::Begin("Debug Window");
	
	if (ImGui::Button("Debug")){ // debug bomb (show collision boxes)

		Message msg(MessageIDTypes::DEBUG_ALL);
		CORE->BroadcastMessage(&msg);
	}
	
	if (playertest == nullptr) {

		playertest = CORE->GetSystem<EntityFactory>()->GetObjectWithID(0);
	}

	//std::shared_ptr<Component> tf_comp = playertest->GetComponent(ComponentTypes::TRANSFORM);
	//if (tf_comp)
	//	std::shared_ptr<Transform> player_xform = std::dynamic_pointer_cast<Transform>(tf_comp);
	//ImGui::SliderFloat2("player scale", &scale_.x, -1.0f, 1.0f);
	//ImGui::SliderFloat2("player position", &scale_.y, -1.0f, 1.0f);

	//player_scale->SetScale((scale_ += player_scale->GetScale()));
	//ImGui::Text("scale X = %f", scale_.x);
	//ImGui::Text("scale Y = %f", scale_.y);

	ImGui::End();
}


