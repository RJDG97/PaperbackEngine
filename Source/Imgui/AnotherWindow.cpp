#include "ImguiWindows/AnotherWindow.h"
#include "Systems/Debug.h"
#include "Entity/ComponentTypes.h"
#include "MathLib/Vector2D.h"
#include <tuple>


void AnotherWindow::Init(){

	imgui_system_ = &*CORE->GetSystem<ImguiSystem>();
}

void AnotherWindow::Update(){

	ImGui::ShowDemoWindow();
	ImGui::Begin("Entity Inspector");
	ImGui::Text("Select something");

	if (imgui_system_->GetDebugBool() && imgui_system_->GetLockBool())
	{
		bool lock = imgui_system_->GetLockBool();
		ImGui::Checkbox("Lock Entity", &lock);
		ImGui::SameLine(); 
		imgui_system_->ImguiHelp("To select other entities,\nuncheck this box.");
		imgui_system_->SetLockBool(lock);

		std::pair<Entity*, std::vector<ComponentTypes>> entitycomp = imgui_system_->GetSelectedEntity();

		ComponentType(entitycomp);

		//Imgui_Demo.cpp -> reference: Line 1991 on ImGui::Combo();
	}

	ImGui::End();
}

void AnotherWindow::ComponentType(std::pair<Entity*, std::vector<ComponentTypes>> entitycomponent)
{
	if (entitycomponent.first){

		for (auto componenttype : entitycomponent.second){

			switch (componenttype)
			{
			case ComponentTypes::NAME:
			{
				std::shared_ptr<Name> entityname = std::dynamic_pointer_cast<Name>(entitycomponent.first->GetComponent(ComponentTypes::NAME));

				ImGui::Text("Entity:"); ImGui::SameLine(0, 2);
				ImGui::TextColored(ImVec4{ 0.498f, 1.0f, 0.831f, 1.0f }, entityname->GetName().c_str());
				break;
			}
			case ComponentTypes::TRANSFORM:
			{
				if (!ImGui::CollapsingHeader("Components"))
					return;

				std::shared_ptr<Transform> entitytransform = std::dynamic_pointer_cast<Transform>(entitycomponent.first->GetComponent(ComponentTypes::TRANSFORM));

				float inputRot = entitytransform->GetRotation();
				Vector2D inputPos = {entitytransform->GetPosition()};

				if (ImGui::TreeNode("Rotation")){

					ComponentInput("X", "##rot", inputRot, 1.0f, 10.0f);

					entitytransform->SetRotation(inputRot);

					ImGui::Text("Angle: ");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4{ 0.678f, 1.0f, 0.184f, 1.0f }, "%.2f", entitytransform->GetRotation());
					ImGui::NewLine();

					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Position")){
					
					ComponentInput("X", "##posX", inputPos.x);
					ImGui::SameLine();
					ComponentInput("Y", "##posY", inputPos.y);

					entitytransform->SetPosition(inputPos);

					ImGui::Text("Entity Position:");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4{ 1.0f, 0.843f, 0.0f, 1.0f}, "%.2f, %.2f", entitytransform->GetPosition().x, entitytransform->GetPosition().y);
					ImGui::NewLine();

					ImGui::TreePop();
				}
				break;
			}
			case ComponentTypes::AABB:
			{
				if (ImGui::TreeNode("Bounding Box Scale")) {

					std::shared_ptr<AABB> entityAABB = std::dynamic_pointer_cast<AABB>(entitycomponent.first->GetComponent(ComponentTypes::AABB));

					Vector2D inputAABB{ entityAABB->GetAABBScale()};

					ComponentInput("X", "##aabbX", inputAABB.x);
					ImGui::SameLine();
					ComponentInput("Y", "##aabbY", inputAABB.y);

					entityAABB->SetAABBScale(inputAABB);

					ImGui::Text("Bounding Box Scale: ");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f, 1.0f}, "%.2f, %.2f", entityAABB->GetAABBScale().x, entityAABB->GetAABBScale().y);
					ImGui::NewLine();

					ImGui::TreePop();
				}
				break;
			}
			case ComponentTypes::SCALE:
			{
				if (ImGui::TreeNode("Texture Scale")) {

					std::shared_ptr<Scale> entityscale = std::dynamic_pointer_cast<Scale>(entitycomponent.first->GetComponent(ComponentTypes::SCALE));
					Vector2D inputScale = {entityscale->GetScale()};

					ComponentInput("X", "##scaleX", inputScale.x);
					ImGui::SameLine();
					ComponentInput("Y", "##scaleY", inputScale.y);

					entityscale->SetScale(inputScale);

					ImGui::Text("Texture Scale: ");
					ImGui::SameLine();
					ImGui::TextColored(ImVec4{ 0.0f, 0.749f, 1.0f, 1.0f}, "%.2f, %.2f", entityscale->GetScale().x, entityscale->GetScale().y);

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

void AnotherWindow::ComponentInput(const char* componentLabel, const char* inputLabel, float& componentVar, float startVal, float endVal, float inputWidth)
{
	ImGui::PushItemWidth(inputWidth);

	ImGui::Text(componentLabel);
	ImGui::SameLine();
	ImGui::InputFloat(inputLabel, &componentVar, startVal, endVal, "%.2f");


}


