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
	if (imgui_system_->b_entitywin) {

		ImGui::Begin("Entity Inspector", &imgui_system_->b_entitywin);

		ImGui::Text("Current Scene: "); ImGui::SameLine(0, 2);

		ImGui::TextColored(ImVec4{ 1.0f, 0.549f, 0.0f, 1.0f }, CORE->GetSystem<Game>()->GetStateName().c_str());

		ShowEntityList();

		ImGui::End();
	}

	if (imgui_system_->b_component) {

		ImGui::Begin("Component Inspector", &imgui_system_->b_component);

		bool lock = imgui_system_->GetLockBool();
		ImGui::Checkbox("Select Entity", &lock);
		ImGui::SameLine(); imgui_system_->ImguiHelp("Uncheck this box,\n to select Entities directly");
		imgui_system_->SetLockBool(lock);

		if (imgui_system_->GetEntity()) {

			std::pair<Entity*, std::vector<ComponentTypes>> entity = GetEntityComponents(imgui_system_->GetEntity());

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

			ImGuiTreeNodeFlags flags = ((imgui_system_->GetEntity() == entityIT->second) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			
			opened = (ImGui::TreeNodeEx((void*)(size_t)entityIT->second, flags, label.c_str()));

			if (ImGui::IsItemClicked())
				imgui_system_->SetEntity(entityIT->second); // store the selected Entity to find its components

			if (opened) {

				if (ImGui::Button("Delete Entity...")) {
					imgui_system_->SetEntity(entityIT->second);
					ImGui::OpenPopup("Delete Entity");
				}
				imgui_system_->DeletePopUp("Delete Entity", entityname->GetName());

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
				ImGui::TextColored(ImVec4{ 0.498f, 1.0f, 0.831f, 1.0f }, entityname->GetName().c_str());

				break;
			}
			case ComponentTypes::MOTION:
			{
				std::shared_ptr<Motion> entitymotion = std::dynamic_pointer_cast<Motion>(entitycomponent.first->GetComponent(ComponentTypes::MOTION));

				float inputMass = entitymotion->GetMass();
				if (ImGui::TreeNode("Motion")) {

					ComponentInputFloat("Mass", "##mass", inputMass);
					entitymotion->SetMass(inputMass);

					ComponentDisplayFloat(ImVec4{ 1.0f, 0.855f, 0.725f, 1.0f }, "Mass: ", entitymotion->GetMass());

					if (ImGui::Button("Delete"))
						ImGui::OpenPopup("Delete Motion Component");

					imgui_system_->DeletePopUp("Delete Motion Component", std::string("Motion Component"), entitycomponent.first, entitymotion);
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

					ImGui::Text("Rotation:");

					// move into a function
					float lineHeight = io.FontDefault->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
					ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.973f ,0.227f ,0.267f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.976f, 0.384f, 0.412f, 1.0f });
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.773f, 0.027f, 0.067f, 1.0f });
					ImGui::PushFont(imgui_system_->bold_font_);
					if (ImGui::Button("X", buttonSize))
						entitytransform->SetRotation(0.0f);
					ImGui::PopFont();
					ImGui::PopStyleColor(3);

					ImGui::SameLine(0,2);
					ComponentInputFloat("", "##rot", inputRot, 95.0f, 1.0f, 10.0f);

					entitytransform->SetRotation(inputRot);

					ComponentDisplayFloat(ImVec4{ 0.678f, 1.0f, 0.184f, 1.0f }, "Angle: ", entitytransform->GetRotation());

					ImGui::Text("Position:");
					ComponentInputFloat("X", "##posX", inputPos.x, 102.0f);
					ImGui::SameLine();
					ComponentInputFloat("Y", "##posY", inputPos.y, 100.0f);

					entitytransform->SetPosition(inputPos);

					ComponentDisplayVec(ImVec4{ 1.0f, 0.843f, 0.0f, 1.0f }, "Entity Position: ", entitytransform->GetPosition());

					ImGui::TreePop();
				}
				break;
			}
			case ComponentTypes::HEALTH:
				ImGui::Text("Health Component");
				break;
			case ComponentTypes::CAMERA:
				ImGui::Text("Camera Component");
				break;
			case ComponentTypes::CONTROLLER:
				ImGui::Text("Controller");
				break;
			case ComponentTypes::TEXTURERENDERER:
				ImGui::Text("Texture Renderer");
				break;
			case ComponentTypes::ANIMATIONRENDERER:
				ImGui::Text("Animation Renderer");
				break;
			case ComponentTypes::TEXTRENDERER:
				ImGui::Text("Text Renderer");
				break;
			case ComponentTypes::AABB:
			{
				std::shared_ptr<AABB> entityAABB = std::dynamic_pointer_cast<AABB>(entitycomponent.first->GetComponent(ComponentTypes::AABB));

				Vector2D inputAABB{ entityAABB->GetAABBScale() };

				if (ImGui::TreeNode("Bounding Box")) {

					ComponentInputFloat("X", "##aabbX", inputAABB.x, 100.0f);
					ImGui::SameLine();
					ComponentInputFloat("Y", "##aabbY", inputAABB.y, 100.0f);

					entityAABB->SetAABBScale(inputAABB);

					ComponentDisplayVec(ImVec4{ 0.863f, 0.078f, 0.235f, 1.0f }, "Bounding Box Scale: ", entityAABB->GetAABBScale());
					
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
						ImGui::Text("States: ");
						ImGui::PushItemWidth(100.0f);
						if (ImGui::BeginCombo("##abc", AIstates[0])) {
							for (int i = 0; i < IM_ARRAYSIZE(AIstates); ++i)
								if (ImGui::Selectable(AIstates[i]))
									entityAI->SetState(static_cast<AI::AIState>(i));

							ImGui::EndCombo();
						}
						ImGui::PopItemWidth();

						ImGui::Text("Current AI State: ");
						ImGui::SameLine(0, 2); 
						ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f, 1.0f }, entityState);

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("AI Range")) {
						ComponentInputFloat("Range", "##range", inputRange);
						entityAI->SetRange(inputRange);

						ComponentDisplayFloat(ImVec4{ 0.863f, 0.078f, 0.235f, 1.0f }, "Current Set Range: ", entityAI->GetRange());
						ImGui::TreePop();
					}

					if (ImGui::TreeNode("AI Attack Power")) {

						ComponentInputInt("Power", "##power", inputAtk, 100.0f, 1, 5);
						entityAI->SetAtk(inputAtk);

						ComponentDisplayInt(ImVec4{ 0.863f, 0.078f, 0.235f, 1.0f }, "Current Attack Power: ", inputAtk);
						ImGui::TreePop();
					}

					if (ImGui::TreeNode("AI Speed")) {
						ComponentInputFloat("Speed", "##speed", inputSpeed, 100.0f, 1.0f, 5.0f);

						entityAI->SetSpeed(inputSpeed);

						ComponentDisplayFloat(ImVec4{ 0.863f, 0.078f, 0.235f, 1.0f }, "Current Speed:", inputSpeed);
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

							ComponentInputFloat("X Position", "##nodeXpos", newNode.x, 100.0f, 0.5f, 1.0f);
							ComponentInputFloat("Y Position", "##nodeYpos", newNode.y, 100.0f, 0.5f, 1.0f);

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

					imgui_system_->DeletePopUp("Delete AI Component", std::string("AI Component"), entitycomponent.first, entityAI);

				}
				break;
			}
			case ComponentTypes::SCALE:
			{
				std::shared_ptr<Scale> entityscale = std::dynamic_pointer_cast<Scale>(entitycomponent.first->GetComponent(ComponentTypes::SCALE));

				Vector2D inputScale = { entityscale->GetScale() };

				if (ImGui::TreeNode("Scale")) {
					ImGui::Text("Texture Scale");

					ComponentInputFloat("X", "##scaleX", inputScale.x);
					ImGui::SameLine();
					ComponentInputFloat("Y", "##scaleY", inputScale.y);

					entityscale->SetScale(inputScale);

					ComponentDisplayVec(ImVec4{ 0.0f, 0.749f, 1.0f, 1.0f }, "Texture Scale: ", entityscale->GetScale());
					ImGui::TreePop();
				}
				break;
			}
			case ComponentTypes::STATUS:
				ImGui::Text("Status Component");
				break;
			case ComponentTypes::POINTLIGHT:
			{
				std::shared_ptr<PointLight> entitypointlight = std::dynamic_pointer_cast<PointLight>(entitycomponent.first->GetComponent(ComponentTypes::POINTLIGHT));

				float inputRadius = entitypointlight->GetRadius();
				float inputIntensity = entitypointlight->GetIntensity();
				if (ImGui::TreeNode("PointLight")) {

					ImGui::Text("Radius");
					ComponentInputFloat("Light Radius", "##lightRad", inputRadius, 102.0f);
					entitypointlight->SetRadius(inputRadius);

					ComponentDisplayFloat(ImVec4{ 1.0f, 0.271f, 0.0f, 1.0f }, "Point Light Radius: ", entitypointlight->GetRadius());
					
					ImGui::Text("Intensity");
					ComponentInputFloat("Light Intensity", "##lightinten", inputIntensity, 102.0f);
					entitypointlight->SetIntensity(inputIntensity);

					ComponentDisplayFloat(ImVec4{ 1.0f, 0.271f, 0.0f, 1.0f }, "Point Light Intensity: ", entitypointlight->GetIntensity());
					
					if (ImGui::Button("Delete"))
						ImGui::OpenPopup("Delete Point Light Component");

					imgui_system_->DeletePopUp("Delete Point Light Component", std::string("Point Light Component"), entitycomponent.first, entitypointlight);

					ImGui::TreePop();
				}
				break;
			}
			case ComponentTypes::CONELIGHT:
				ImGui::Text("Cone Light");
				break;
			case ComponentTypes::BASICAI:
				ImGui::Text("Basic AI");
				break;
			case ComponentTypes::CLICKABLE:
				ImGui::Text("Clickable");
				break;
			case ComponentTypes::INPUTCONTROLLER:
				ImGui::Text("Input Controller");
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

void EntityWindow::ComponentInputFloat(const char* componentLabel, const char* inputLabel, float& componentVar, float inputWidth, float startVal, float endVal) {
	ImGui::PushItemWidth(inputWidth);

	ImGui::Text(componentLabel);
	ImGui::SameLine();
	ImGui::InputFloat(inputLabel, &componentVar, startVal, endVal, "%.2f");
}

void EntityWindow::ComponentInputInt(const char* componentLabel, const char* inputLabel, int& componentVar, float inputWidth, int startVal, int endVal) {
	ImGui::PushItemWidth(inputWidth);

	ImGui::Text(componentLabel);
	ImGui::SameLine();
	ImGui::InputInt(inputLabel, &componentVar, startVal, endVal);
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
