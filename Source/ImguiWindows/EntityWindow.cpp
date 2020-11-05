#include "ImguiWindows/EntityWindow.h"
#include "Systems/Debug.h"
#include "Entity/ComponentTypes.h"
#include "MathLib/Vector2D.h"
#include <tuple>


void EntityWindow::Init(){

	imgui_system_ = &*CORE->GetSystem<ImguiSystem>();
	entities_ = &*CORE->GetManager<EntityManager>();
}

void EntityWindow::Update() {

	//ImGui::ShowDemoWindow();

	ImGui::Begin("Entity Inspector");

	ImGui::Text("Current Scene: "); ImGui::SameLine(0, 2);

	ImGui::TextColored(ImVec4{ 1.0f, 0.549f, 0.0f, 1.0f}, CORE->GetSystem<Game>()->GetStateName().c_str());

	ShowEntityList();

	ImGui::End();

	ImGui::Begin("Component Inspector");

	bool lock = imgui_system_->GetLockBool();
	ImGui::Checkbox("Select Entity", &lock);
	ImGui::SameLine(); imgui_system_->ImguiHelp("Uncheck this box,\nif want to selected Entities directly");
	imgui_system_->SetLockBool(lock);

	if (imgui_system_->GetEntity()) {

		std::pair<Entity*, std::vector<ComponentTypes>> entity = GetEntityComponents(imgui_system_->GetEntity());

		CheckComponentType(entity);
	}



	ImGui::End();

}

std::pair<Entity*, std::vector<ComponentTypes>> EntityWindow::GetEntityComponents(Entity* entity) {

	std::vector<ComponentTypes> comp_arr;

	ComponentArr arr = entity->GetComponentArr();
	comp_arr.reserve(arr.size());

	for (ComponentArrIt it = arr.begin(); it != arr.end(); ++it)
		comp_arr.push_back((*it)->GetComponentTypeID());

	return std::make_pair(entity, comp_arr);
}

void EntityWindow::ShowEntityList() {

	bool opened = false, deleteentity = false;

	if (entities_) {

		for (entityIT = entities_->GetEntities().begin(); entityIT != entities_->GetEntities().end(); entityIT++) {

			std::shared_ptr<Name> entityname = std::dynamic_pointer_cast<Name>(entityIT->second->GetComponent(ComponentTypes::NAME));

			ImGuiTreeNodeFlags flags = ((selection == entityIT->second) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

			opened = (ImGui::TreeNodeEx((void*)(size_t)entityIT->second, flags, entityname->GetName().c_str()));

			if (ImGui::IsItemClicked())
				imgui_system_->SetEntity(entityIT->second); // store the selected Entity to find its components

			if (opened) {

				if (ImGui::Button("Delete Entity...")) {
					imgui_system_->SetEntity(entityIT->second);
					ImGui::OpenPopup("Delete Entity");
				}

				ImVec2 centre = ImGui::GetMainViewport()->GetCenter();

				ImGui::SetNextWindowPos(centre, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				if (ImGui::BeginPopup("Delete Entity")) {

					if (ImGui::Button("OK")) {
						entities_->DeleteEntity(imgui_system_->GetEntity());
						imgui_system_->SetEntity(nullptr);
						ImGui::CloseCurrentPopup();
					}
					if (ImGui::Button("Cancel")) {
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}

				ImGui::TreePop();
			}
		}
	}
}

//void EntityWindow::deletepop(const char* windowName)
//{
//}

void EntityWindow::CheckComponentType(std::pair<Entity*, std::vector<ComponentTypes>> entitycomponent) {
	if (entitycomponent.first) {

		for (auto componenttype : entitycomponent.second) {

			switch (componenttype)
			{
			case ComponentTypes::NAME:
			{
				std::shared_ptr<Name> entityname = std::dynamic_pointer_cast<Name>(entitycomponent.first->GetComponent(ComponentTypes::NAME));

				ImGui::Text("Entity:"); ImGui::SameLine(0, 2);
				ImGui::TextColored(ImVec4{ 0.498f, 1.0f, 0.831f, 1.0f }, entityname->GetName().c_str());

				//if (!ImGui::CollapsingHeader("Components"))
				//	return;
				break;
			}
			case ComponentTypes::MOTION:
			{
				std::shared_ptr<Motion> entitymotion = std::dynamic_pointer_cast<Motion>(entitycomponent.first->GetComponent(ComponentTypes::MOTION));

				float inputMass = entitymotion->GetMass();
				if (ImGui::TreeNode("Mass")) {

					ComponentInput("Mass", "##mass", inputMass);
					entitymotion->SetMass(inputMass);

					ComponentDisplayFloat(ImVec4{ 1.0f, 0.855f, 0.725f, 1.0f }, "Mass: ", entitymotion->GetMass());
					ImGui::TreePop();
				}
				break;
			}
			case ComponentTypes::TRANSFORM:
			{
				std::shared_ptr<Transform> entitytransform = std::dynamic_pointer_cast<Transform>(entitycomponent.first->GetComponent(ComponentTypes::TRANSFORM));

				float inputRot = entitytransform->GetRotation();
				Vector2D inputPos = { entitytransform->GetPosition() };

				if (ImGui::TreeNode("Rotation")) {

					ComponentInput("X", "##rot", inputRot, 95.0f, 1.0f, 10.0f);

					entitytransform->SetRotation(inputRot);

					ComponentDisplayFloat(ImVec4{ 0.678f, 1.0f, 0.184f, 1.0f }, "Angle: ", entitytransform->GetRotation());
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Position")) {

					ComponentInput("X", "##posX", inputPos.x, 102.0f);
					ImGui::SameLine();
					ComponentInput("Y", "##posY", inputPos.y, 95.0f);

					entitytransform->SetPosition(inputPos);

					ComponentDisplayVec(ImVec4{ 1.0f, 0.843f, 0.0f, 1.0f }, "Entity Position: ", entitytransform->GetPosition());

					ImGui::TreePop();
				}
				break;
			}
			case ComponentTypes::HEALTH:
				break;
			case ComponentTypes::CAMERA:
				break;
			case ComponentTypes::CONTROLLER:
				break;
			case ComponentTypes::TEXTURERENDERER:
				break;
			case ComponentTypes::ANIMATIONRENDERER:
				break;
			case ComponentTypes::TEXTRENDERER:
				break;
			case ComponentTypes::AABB:
			{
				if (ImGui::TreeNode("Bounding Box Scale")) {

					std::shared_ptr<AABB> entityAABB = std::dynamic_pointer_cast<AABB>(entitycomponent.first->GetComponent(ComponentTypes::AABB));

					Vector2D inputAABB{ entityAABB->GetAABBScale() };

					ComponentInput("X", "##aabbX", inputAABB.x);
					ImGui::SameLine();
					ComponentInput("Y", "##aabbY", inputAABB.y);

					entityAABB->SetAABBScale(inputAABB);

					ComponentDisplayVec(ImVec4{ 0.863f, 0.078f, 0.235f, 1.0f }, "Bounding Box Scale: ", entityAABB->GetAABBScale());

					ImGui::TreePop();
				}
				break;
			}
			case ComponentTypes::AI:
			{
				ImGui::Text("There is an AI Component");
				break;
			}
			case ComponentTypes::SCALE:
			{
				std::shared_ptr<Scale> entityscale = std::dynamic_pointer_cast<Scale>(entitycomponent.first->GetComponent(ComponentTypes::SCALE));

				Vector2D inputScale = { entityscale->GetScale() };

				if (ImGui::TreeNode("Texture Scale")) {

					ComponentInput("X", "##scaleX", inputScale.x);
					ImGui::SameLine();
					ComponentInput("Y", "##scaleY", inputScale.y);

					entityscale->SetScale(inputScale);

					ComponentDisplayVec(ImVec4{ 0.0f, 0.749f, 1.0f, 1.0f }, "Texture Scale: ", entityscale->GetScale());
					ImGui::TreePop();
				}
				break;
			}
			case ComponentTypes::STATUS:
				break;
			case ComponentTypes::POINTLIGHT:
			{
				std::shared_ptr<PointLight> entitypointlight = std::dynamic_pointer_cast<PointLight>(entitycomponent.first->GetComponent(ComponentTypes::POINTLIGHT));

				float inputRadius = entitypointlight->GetRadius();
				float inputIntensity = entitypointlight->GetIntensity();

				if (ImGui::TreeNode("Point Light Radius")) {
					ComponentInput("Light Radius", "##lightRad", inputRadius);
					entitypointlight->SetRadius(inputRadius);

					ComponentDisplayFloat(ImVec4{ 1.0f, 0.271f, 0.0f, 1.0f }, "Point Light Radius: ", entitypointlight->GetRadius());
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Point Light Intensity")) {
					ComponentInput("Light Intensity", "##lightinten", inputIntensity);
					entitypointlight->SetIntensity(inputIntensity);

					ComponentDisplayFloat(ImVec4{ 1.0f, 0.271f, 0.0f, 1.0f }, "Point Light Intensity: ", entitypointlight->GetIntensity());
					ImGui::TreePop();
				}
				break;
			}

			case ComponentTypes::CONELIGHT:
				break;
			case ComponentTypes::BASICAI:
				break;
			case ComponentTypes::CLICKABLE:
				break;
			case ComponentTypes::INPUTCONTROLLER:
				break;
			}
		}
	}
}

void EntityWindow::ComponentInput(const char* componentLabel, const char* inputLabel, float& componentVar, float inputWidth, float startVal, float endVal) {
	ImGui::PushItemWidth(inputWidth);

	ImGui::Text(componentLabel);
	ImGui::SameLine();
	ImGui::InputFloat(inputLabel, &componentVar, startVal, endVal, "%.2f");
}

void EntityWindow::ComponentDisplayFloat(ImVec4 color, const char* label, float componentVal, const char* format) {
	ImGui::Text(label);
	ImGui::SameLine();
	ImGui::TextColored(color, format, componentVal);
	ImGui::NewLine();
}

void EntityWindow::ComponentDisplayVec(ImVec4 color, const char* label, Vector2D componentVec, const char* format) {
	ImGui::Text(label);
	ImGui::SameLine();
	ImGui::TextColored(color, format, format, componentVec.x, componentVec.y);
	ImGui::NewLine();
}
