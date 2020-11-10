#include "ImguiWindows/EntityWindow.h"
#include "Systems/Debug.h"
#include "Entity/ComponentTypes.h"
#include "MathLib/Vector2D.h"
#include <tuple>


void EntityWindow::Init(){

	imgui_ = &*CORE->GetSystem<ImguiSystem>();
	entities_ = &*CORE->GetManager<EntityManager>();
}

void EntityWindow::Update() {

	//ImGui::ShowDemoWindow();
	if (imgui_->b_entitywin) {

		ImGui::Begin("Entity Inspector", &imgui_->b_entitywin);

		//maybe move this somewhere else
		if (ImGui::Button("Clear Selection"))
			imgui_->SetEntity({});
		ImGui::Separator();

		ShowEntityList();

		ImGui::End();
	}

	if (imgui_->b_component) {

		ImGui::Begin("Component Inspector", &imgui_->b_component);

		bool lock = imgui_->GetLockBool();
		ImGui::Checkbox("Select Entity", &lock);
		ImGui::SameLine(); imgui_->ImguiHelp("Uncheck this box,\n to select Entities directly");
		imgui_->SetLockBool(lock);

		if (imgui_->GetEntity()) {

			std::pair<Entity*, std::vector<ComponentTypes>> entity = GetEntityComponents(imgui_->GetEntity());

			CheckComponentType(entity);
		}

		ImGui::End();
	}
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

	bool opened = false;

	if (entities_) {

		ImGui::Text("Entity Count: %d", entities_->GetEntities().size());
		for (entityIT = entities_->GetEntities().begin(); entityIT != entities_->GetEntities().end(); entityIT++) {

			std::shared_ptr<Name> entityname = std::dynamic_pointer_cast<Name>(entityIT->second->GetComponent(ComponentTypes::NAME));

			std::string label = entityname->GetName() + " (" + std::to_string(entityIT->second->GetID()) + ")";

			ImGuiTreeNodeFlags flags = ((imgui_->GetEntity() == entityIT->second) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			
			opened = (ImGui::TreeNodeEx((void*)(size_t)entityIT->second, flags, label.c_str()));

			if (ImGui::IsItemClicked())
				imgui_->SetEntity(entityIT->second); // store the selected Entity to find its components

			if (opened) {

				if (ImGui::Button("Delete Entity...")) {
					imgui_->SetEntity(entityIT->second);
					ImGui::OpenPopup("Delete Entity");
				}
				imgui_->DeletePopUp("Delete Entity", entityname->GetName());

				ImGui::TreePop();
			}
		}
	}
}

void EntityWindow::CheckComponentType(std::pair<Entity*, std::vector<ComponentTypes>> entitycomponent) {

	ImGuiIO& io = ImGui::GetIO();
	if (entitycomponent.first) {

		for (auto componenttype : entitycomponent.second) {

			switch (componenttype)
			{
			case ComponentTypes::NAME:
			{
				std::shared_ptr<Name> entityname = std::dynamic_pointer_cast<Name>(entitycomponent.first->GetComponent(ComponentTypes::NAME));

				ImGui::Text("Entity:"); ImGui::SameLine(0, 2);
				ImGui::TextColored(AQUAMARINE, entityname->GetName().c_str());

				break;
			}
			case ComponentTypes::MOTION:
			{
				std::shared_ptr<Motion> entitymotion = std::dynamic_pointer_cast<Motion>(entitycomponent.first->GetComponent(ComponentTypes::MOTION));

				float inputMass = entitymotion->GetMass();
				if (ImGui::TreeNode("Motion")) {

					ComponentInputFloat("Mass", "##mass", inputMass);
					entitymotion->SetMass(inputMass);

					if (ImGui::Button("Delete"))
						ImGui::OpenPopup("Delete Motion Component");

					imgui_->DeletePopUp("Delete Motion Component", std::string("Motion Component"), entitycomponent.first, entitymotion);
					ImGui::TreePop();
				}
				break;
			}
			case ComponentTypes::TRANSFORM:
			{
				std::shared_ptr<Transform> entitytransform = std::dynamic_pointer_cast<Transform>(entitycomponent.first->GetComponent(ComponentTypes::TRANSFORM));

				float inputRot = entitytransform->GetRotation();
				Vector2D inputPos = { entitytransform->GetPosition() };

				if (ImGui::TreeNode("Transform Component")) {

					ImGui::Text("Rotation");
					FloatInput(inputRot);

					entitytransform->SetRotation(inputRot);
					ImGui::Separator();

					ImGui::Text("Position");

					Vec2Input(inputPos);
					entitytransform->SetPosition(inputPos);

					ImGui::TreePop();
				}
				break;
			}
			case ComponentTypes::HEALTH:
			{
				std::shared_ptr<Health> entityHealth = std::dynamic_pointer_cast<Health>(entitycomponent.first->GetComponent(ComponentTypes::HEALTH));
				int entityCurrenthealth = entityHealth->GetCurrentHealth();
				int entityMaxhealth = entityHealth->GetMaxHealth();

				if (ImGui::TreeNode("Health")) {

					SetArrowButtons(entityCurrenthealth);
					entityHealth->SetMaxHealth(entityCurrenthealth);

					SetArrowButtons(entityMaxhealth);
					entityHealth->SetMaxHealth(entityMaxhealth);

					ImGui::TreePop();
				}
				break;
			}
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
				std::shared_ptr<AABB> entityAABB = std::dynamic_pointer_cast<AABB>(entitycomponent.first->GetComponent(ComponentTypes::AABB));

				Vector2D inputAABB{ entityAABB->GetAABBScale() };

				if (ImGui::TreeNode("Collider")) {

					Vec2Input(inputAABB);

					entityAABB->SetAABBScale(inputAABB);
					
					if (ImGui::Checkbox("Draw Bounding Box", &b_draw)) {

						Message msg(MessageIDTypes::DEBUG_ALL);
						CORE->BroadcastMessage(&msg);
					}

					ImGui::TreePop();
				}
				break;
			}
			case ComponentTypes::AI:
			{
				std::shared_ptr<AI>entityAI = std::dynamic_pointer_cast<AI>(entitycomponent.first->GetComponent(ComponentTypes::AI));
				
				const char* entityState = GetAIState(entityAI->GetState());
				const char* entityType = GetAIType(entityAI->GetType());

				float inputRange = entityAI->GetRange();
				int inputAtk = entityAI->GetAtk();
				float inputSpeed = entityAI->GetSpeed();

				size_t inputNumDes = entityAI->GetNumDes();
				std::vector<Vector2D> inputDes = entityAI->GetDestinations();
				Vector2D newNode = { entityAI->GetNewDestination().x, entityAI->GetNewDestination().y };
				size_t counter = 0; // to act as an id for the TreeNode

				if (ImGui::TreeNode("AI Components")) {

					if (ImGui::TreeNode("AI Type")) {

						ImGui::Text("Current AI Type: ");
						ImGui::SameLine(0, 2);
						ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f, 1.0f }, entityType);
						ImGui::TreePop();
					}

					if (ImGui::TreeNode("AI States")) {
						ImGui::Text("Current AI State: ");
						ImGui::SameLine(0, 2);
						ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f, 1.0f }, entityState);


						ImGui::PushItemWidth(130.0f);
						if (ImGui::BeginCombo("##abc", "AI States")) {
							for (int i = 0; i < IM_ARRAYSIZE(AIstates); ++i)
								if (ImGui::Selectable(AIstates[i]))
									entityAI->SetState(static_cast<AI::AIState>(i));

							ImGui::EndCombo();
						}
						ImGui::PopItemWidth();

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("AI Range")) {

						ComponentInputFloat("Range", "##range", inputRange, 175.0f);
						entityAI->SetRange(inputRange);
						ImGui::TreePop();
					}

					if (ImGui::TreeNode("AI Attack Power")) {

						ComponentInputInt("Power", "##power", inputAtk, 175.0f, 1, 2);
						entityAI->SetAtk(inputAtk);

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("AI Speed")) {

						ComponentInputFloat("Speed", "##speed", inputSpeed, 175.0f, 0.1f, 1.0f);
						entityAI->SetSpeed(inputSpeed);

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("Ai Destinations")) {

						ImGui::Text("Number of Destinations: %d", inputNumDes);
						ImGui::Text("Current Destinations: ");

						for (std::vector<Vector2D>::iterator it = inputDes.begin(); it != inputDes.end(); it++) {
							counter++; 
							if (ImGui::TreeNodeEx((void*)(size_t)counter, 0, "%.2f, %.2f", (*it).x, (*it).y))
							{
								if (ImGui::Button("Delete Node")) {
									if (it == inputDes.begin()) 
										it = inputDes.erase(it);
									else {
										it = inputDes.erase(it);
										it--;
									}
									entityAI->GetDestinations().clear();
									entityAI->SetDestinations(inputDes);
								}

								ImGui::TreePop();
							}
						}

						if (ImGui::Button("Clear All Nodes")) {
							entityAI->GetDestinations().clear();
							entityAI->SetDestinations({});
						}
					
						if (ImGui::TreeNode("Add Destination")) {

							ImGui::Text("New Node Position:");

							Vec2Input(newNode);

							entityAI->SetNewDestination(newNode);

							ImGui::Text("%.2f, %.2f", newNode.x, newNode.y);

							if (ImGui::Button("Add")) {
								inputDes.push_back(newNode);
								entityAI->GetDestinations().clear();
								entityAI->SetDestinations(inputDes);
								entityAI->SetNewDestination(Vector2D{ 0.0f, 0.0f });
							}
							ImGui::TreePop();
						}

						ImGui::TreePop();
					}

					ImGui::TreePop();

					if (ImGui::Button("Delete"))
						ImGui::OpenPopup("Delete AI Component");

					imgui_->DeletePopUp("Delete AI Component", std::string("AI Component"), entitycomponent.first, entityAI);

				}
				break;
			}
			case ComponentTypes::SCALE:
			{
				std::shared_ptr<Scale> entityscale = std::dynamic_pointer_cast<Scale>(entitycomponent.first->GetComponent(ComponentTypes::SCALE));

				Vector2D inputScale = { entityscale->GetScale() };

				if (ImGui::TreeNode("Scale")) {
					
					Vec2Input(inputScale);
					entityscale->SetScale(inputScale);
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
				if (ImGui::TreeNode("PointLight")) {

					ComponentInputFloat("Light Radius", "##lightRad", inputRadius, 102.0f);
					entitypointlight->SetRadius(inputRadius);
					
					ComponentInputFloat("Light Intensity", "##lightinten", inputIntensity, 102.0f);
					entitypointlight->SetIntensity(inputIntensity);
					
					if (ImGui::Button("Delete"))
						ImGui::OpenPopup("Delete Point Light Component");

					imgui_->DeletePopUp("Delete Point Light Component", std::string("Point Light Component"), entitycomponent.first, entitypointlight);

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

const char* EntityWindow::GetAIState(int aiState)
{
	for (int i = 0; i < 5; ++i) {
		if (i == aiState)
			return AIstates[i];
	}
	return nullptr;
}

const char* EntityWindow::GetAIType(int aiType)
{
	for (int i = 0; i < 3; ++i) {
		if (i == aiType)
			return AItype[i];
	}
	return nullptr;
}

void EntityWindow::SetArrowButtons(int& componentVar) {

	ImGui::Text("Current Health: %d", componentVar);
	ImGui::SameLine(0, 3);
	ImGui::PushButtonRepeat(true);
	if (ImGui::ArrowButton("##left", ImGuiDir_Left)) { componentVar--; }
	ImGui::SameLine(0, 5);
	if (ImGui::ArrowButton("##right", ImGuiDir_Right)) { componentVar++; }
	ImGui::PopButtonRepeat();
}

ImVec2 EntityWindow::SetButtonSize() {

	float lineHeight = ImGui::GetIO().FontDefault->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
	return buttonSize;
}

void EntityWindow::Vec2Input(Vector2D& componentVar, float defaultVal) {

	SetButtonSize();
	imgui_->CustomImGuiButton(REDDEFAULT, REDHOVERED, REDACTIVE);

	ImGui::PushFont(imgui_->bold_font_);
	if (ImGui::Button("X", SetButtonSize()))
		componentVar.x = defaultVal;
	ImGui::PopFont();

	ImGui::PopStyleColor(3);

	ImGui::SameLine(0, 2);

	ComponentInputFloat("", "##X", componentVar.x, 102.0f);

	ImGui::SameLine();

	imgui_->CustomImGuiButton(GREENDEFAULT, GREENHOVERED, GREENACTIVE);

	ImGui::PushFont(imgui_->bold_font_);
	if (ImGui::Button("Y", SetButtonSize()))
		componentVar.y = defaultVal;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine(0, 2);

	ComponentInputFloat("", "##Y", componentVar.y, 100.0f);
}

void EntityWindow::FloatInput(float& componentVar, const char* label, float defaultVal) {

	imgui_->CustomImGuiButton(REDDEFAULT, REDHOVERED, REDACTIVE);

	ImGui::PushFont(imgui_->bold_font_);
	if (ImGui::Button(label, SetButtonSize()))
		componentVar = 0.0f;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine(0, 2);

	ComponentInputFloat("", "##rot", componentVar, 95.0f, 1.0f, 10.0f);
}

void EntityWindow::ComponentInputFloat(const char* componentLabel, const char* inputLabel, float& componentVar, float inputWidth, float startVal, float endVal) {
	ImGui::PushItemWidth(inputWidth);

	ImGui::Text(componentLabel);
	ImGui::SameLine();
	ImGui::InputFloat(inputLabel, &componentVar, startVal, endVal, "%.2f");
	ImGui::PopItemWidth();
}

void EntityWindow::ComponentInputInt(const char* componentLabel, const char* inputLabel, int& componentVar, float inputWidth, int startVal, int endVal) {
	ImGui::PushItemWidth(inputWidth);

	ImGui::Text(componentLabel);
	ImGui::SameLine();
	ImGui::InputInt(inputLabel, &componentVar, startVal, endVal);
	ImGui::PopItemWidth();

}

void EntityWindow::ComponentDisplayFloat(ImVec4 color, const char* label, float componentVal, const char* format) {
	ImGui::Text(label);
	ImGui::SameLine();
	ImGui::TextColored(color, format, componentVal);
	ImGui::Separator();
}

void EntityWindow::ComponentDisplayInt(ImVec4 color, const char* label, int componentVal, const char* format) {
	ImGui::Text(label);
	ImGui::SameLine();
	ImGui::TextColored(color, format, componentVal);
	ImGui::Separator();
}

void EntityWindow::ComponentDisplayVec(ImVec4 color, const char* label, Vector2D componentVec) {
	ImGui::Text(label);
	ImGui::SameLine();
	ImGui::TextColored(color, "X: %.2f Y: %.2f", componentVec.x, componentVec.y);
	ImGui::Separator();
}
