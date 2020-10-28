#include "ImguiWindows/AnotherWindow.h"
#include "Systems/Debug.h"
#include "Entity/ComponentTypes.h"
#include "MathLib/Vector2D.h"
#include <tuple>


void AnotherWindow::Init(){

	imgui_system_ = &*CORE->GetSystem<ImguiSystem>();
	collision_ = &*CORE->GetSystem<Collision>();
}

void AnotherWindow::Update(){

	ImGui::ShowDemoWindow();
	ImGui::Begin("Debug Window");
	if (ImGui::Button("Debug")){ // debug bomb (show collision boxes)

		Message msg(MessageIDTypes::DEBUG_ALL);
		CORE->BroadcastMessage(&msg);
	}
		
	if (imgui_system_->GetDebugBool() && imgui_system_->GetLockEntity())
	{
		bool lock = imgui_system_->GetLockEntity();
		ImGui::Checkbox("Select Entity", &lock);
		imgui_system_->SetLockEntity(lock);

		std::pair<Entity*, std::vector<ComponentTypes>> entitycomp = imgui_system_->GetSelectedEntity();

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
			case ComponentTypes::TRANSFORM:
			{
				if (!ImGui::CollapsingHeader("Transform"))
					return;

				std::shared_ptr<Transform> entitytransform = std::dynamic_pointer_cast<Transform>(entitycomponent.first->GetComponent(ComponentTypes::TRANSFORM));
				float rot = 0.0f;
				float posX = 0.0f, posY = 0.0f;
				float inputPos[2] = { entitytransform->GetPosition().x, entitytransform->GetPosition().y };

				if (ImGui::TreeNode("Rotation")){

					ImGui::Text("Rotation: ");
					ImGui::SameLine();
					ImGui::SliderAngle("", &rot);

					rot += entitytransform->GetRotation();
					entitytransform->SetRotation(rot);
					ImGui::Text("Angle: %.2f", entitytransform->GetRotation());

					ImGui::TreePop();

				}

				if (ImGui::TreeNode("Postion")){

					ImGui::Text("Pos Input: ");
					ImGui::SameLine();
					ImGui::PushItemWidth(100.0f);
					ImGui::InputFloat2("##inputpos", inputPos);

					Vector2D inputpos = { inputPos[0], inputPos[1] };
					entitytransform->SetPosition(inputpos);

					ImGui::Text(" X Position: ");
					ImGui::SameLine();
					ImGui::PushItemWidth(0.01f);
					ImGui::InputFloat("##X", &posX, 1.0f, 5.0f, "%.2f");

					ImGui::Text(" Y Position: ");
					ImGui::SameLine();
					ImGui::PushItemWidth(0.01f);
					ImGui::InputFloat("##Y", &posY, 1.0f, 5.0f, "%.2f");

					posX += entitytransform->GetPosition().x;
					posY += entitytransform->GetPosition().y;

					Vector2D adjustpos = { posX, posY };
					entitytransform->SetPosition(adjustpos);
					
					ImGui::Text("Entity Position: %.2f, %.2f", entitytransform->GetPosition().x, entitytransform->GetPosition().y);


					ImGui::TreePop();
				}
				break;
			}
			case ComponentTypes::SCALE:
			{
				if (ImGui::TreeNode("Entity Scale")) {

					std::shared_ptr<Scale> entityscale = std::dynamic_pointer_cast<Scale>(entitycomponent.first->GetComponent(ComponentTypes::SCALE));
					float scaleX = 0.0f, scaleY = 0.0f;
					float inputScale[2] = { entityscale->GetScale().x, entityscale->GetScale().y };

					ImGui::Text("Scale Input: ");
					ImGui::SameLine();
					ImGui::PushItemWidth(100.0f);
					ImGui::InputFloat2("##inputscale", inputScale);

					Vector2D newinputScale = { inputScale[0], inputScale[1] };
					entityscale->SetScale(newinputScale);


					ImGui::Text(" X Scale: ");
					ImGui::SameLine();
					ImGui::PushItemWidth(0.01f);
					ImGui::InputFloat("##scaleX", &scaleX, 0.1f, 1.0f, "%.2f");

					ImGui::Text("Y Scale: ");
					ImGui::SameLine();
					ImGui::InputFloat("##scaleY", &scaleY, 0.1f, 1.0f, "%.2f");

					scaleX += entityscale->GetScale().x;
					scaleY += entityscale->GetScale().y;

					Vector2D newScale = { scaleX, scaleY };
					entityscale->SetScale(newScale);

					ImGui::Text("Scale: %.2f, %.2f", entityscale->GetScale().x, entityscale->GetScale().y);

					ImGui::TreePop();
				}
				break;
			}
			}

		}
	}
	else
		ImGui::Text("There is no Entity(s) selected");
}


