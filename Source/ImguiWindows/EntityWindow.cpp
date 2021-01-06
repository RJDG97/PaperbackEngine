/**********************************************************************************
*\file         EntityWindow.cpp
*\brief        Contains definition of functions and variables used for
*			   the EntityWindow to display/manage entities as well as their components

*\author	   Ee Ling Adele, Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "ImguiWindows/EntityWindow.h"
#include "Components/SoundEmitter.h"
#include "Entity/ComponentTypes.h"
#include "MathLib/Vector2D.h"
#include "Entity/Entity.h"

void EntityWindow::Init(){

	imgui_ = &*CORE->GetSystem<ImguiSystem>();
	entities_ = &*CORE->GetManager<EntityManager>();
	texture_ = &*CORE->GetManager<TextureManager>();
	animation_ = &*CORE->GetManager<AnimationManager>();
	graphics_ = &*CORE->GetSystem<GraphicsSystem>();
	component_ = &*CORE->GetManager<ComponentManager>();
	win_ = &*CORE->GetSystem<WindowsSystem>();
	input_ = &*CORE->GetSystem<InputSystem>();
	collision_ = &*CORE->GetSystem<Collision>();
	camera_ = &*CORE->GetSystem<CameraSystem>();
	sound_ = &*CORE->GetSystem<SoundSystem>();

	originalVec_ = mousePos_ = { 0,0 };	
	b_draw = false;
	b_grid = false;
	b_light = false;
}

void EntityWindow::Update() {
	//ImGui::ShowDemoWindow();
	//ImGui::ShowStyleEditor();
	if (imgui_->b_entity_win) {

		ImGui::Begin("Entity Inspector", &imgui_->b_entity_win);

		ImGui::Separator();

		ShowEntityList();

		ImGui::End();
	}

	if (imgui_->b_component) {

		ImGui::Begin("Component Inspector", &imgui_->b_component);

		SelectEntityComponent();// to see the components
		ImGui::End();
	}

	if (imgui_->b_settings) {

		ImGui::Begin("Editor Settings", &imgui_->b_settings);

		DragEntityCheckBox();

		ImGui::SameLine(0, 3);

		if (ImGui::Checkbox("Bounding Boxes", &b_draw)) {

			Message msg(MessageIDTypes::DEBUG_ALL);
			CORE->BroadcastMessage(&msg);
		}

		ImGui::Checkbox("Enable Lighting", &b_light);

		graphics_->EnableLighting(b_light);

		ImGui::SameLine(0, 3);

		if (imgui_->GetCamera()) {

			ImGui::Checkbox("Draw Grid", &b_grid);

			if (!imgui_->EditorMode()) {

				if (input_->IsMousePressed(0))
					mousePos_ = input_->GetUpdatedCoords();

				ImGui::Text("Current Cursor Position: %.2f, %.2f", mousePos_.x, mousePos_.y);
			}
		}

		if (b_grid)
			imgui_->DrawGrid();

		ImGui::End();
	}

	DragEntity();
}

void EntityWindow::CheckComponentType(std::pair<Entity*, std::vector<ComponentTypes>> entitycomponent) {

	if (entitycomponent.first) {

		imgui_->ImguiHelp("1.  For all components with sliders Ctrl + Click to type manually.\n2. Click on the side buttons to reset the value to default");
		ImGui::SameLine(0, 3); ImGui::Text("-> Hover here for help :)");
		!entitycomponent.first->GetID() ? ImGui::TextColored(GOLDENORANGE, "Type: Archetype/Prefab") : ImGui::TextColored(SKYBLUE, "Type: Entity");
		for (auto componenttype : entitycomponent.second) {

			switch (componenttype)
			{
				case ComponentTypes::NAME:
					NameComponent(entitycomponent.first);
					break;

				case ComponentTypes::MOTION:
					MotionComponent(entitycomponent.first);
					break;

				case ComponentTypes::TRANSFORM:
					TransformComponent(entitycomponent.first);
					break;

				case ComponentTypes::HEALTH:
					HealthComponent(entitycomponent.first);
					break;

				case ComponentTypes::CAMERA:
					CameraComponent(entitycomponent.first);
					break;

				case ComponentTypes::CONTROLLER:
					break;

				case ComponentTypes::TEXTURERENDERER:
					TextureRendererComponent(entitycomponent.first);
					break;

				case ComponentTypes::ANIMATIONRENDERER:
					AnimationRendererComponent(entitycomponent.first);
					break;

				case ComponentTypes::TEXTRENDERER:
					break;

				case ComponentTypes::AABB:
					AABBComponent(entitycomponent.first);
					break;

				case ComponentTypes::AI:
					AIComponent(entitycomponent.first);
					break;

				case ComponentTypes::SCALE:
					ScaleComponent(entitycomponent.first);
					break;

				case ComponentTypes::STATUS:
					StatusComponent(entitycomponent.first);
					break;

				case ComponentTypes::POINTLIGHT:
					PointLightComponent(entitycomponent.first);
					break;

				case ComponentTypes::CONELIGHT:
					ConeLightComponent(entitycomponent.first);
					break;

				case ComponentTypes::CLICKABLE:
					break;

				case ComponentTypes::INPUTCONTROLLER:
					break;

				case ComponentTypes::PARENTCHILD: 
					ParentChildComponent(entitycomponent.first);
					break;

				case ComponentTypes::LOGICCOMPONENT:
					break;

				case ComponentTypes::INVENTORY:
					break;

				case ComponentTypes::PARTICLE:
					ParticleComponent(entitycomponent.first);
					break;

				case ComponentTypes::EMITTER:
					EmitterComponent(entitycomponent.first);
					break;

				case ComponentTypes::SOUNDEMITTER:
					SoundEmitterComponent(entitycomponent.first);
					break;
			}
		}
	}
}

void EntityWindow::ShowEntityList() {

	bool opened = false;

	if (entities_) {

		ImGui::Text("Number of Entities: %d", entities_->GetEntities().size());
		ImGui::Separator();

		static ImGuiTextFilter filter;
		filter.Draw(ICON_FA_FILTER, 150.0f);

		for (entityIT = entities_->GetEntities().begin(); entityIT != entities_->GetEntities().end(); entityIT++) {

			std::shared_ptr<Name> entityname = std::dynamic_pointer_cast<Name>(entityIT->second->GetComponent(ComponentTypes::NAME));

			if (filter.PassFilter(entityname->GetName().c_str())) {

				std::string label = entityname->GetName() + " (" + std::to_string(entityIT->second->GetID()) + ")";

				ImGuiTreeNodeFlags flags = ((imgui_->GetEntity() == entityIT->second) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

				opened = (ImGui::TreeNodeEx((void*)(size_t)entityIT->second, flags, label.c_str()));

				if (ImGui::IsItemClicked())
					imgui_->SetEntity(entityIT->second); // store the selected Entity to find its components

				if (opened) {

					if (ImGui::Button("Delete Entity...")) {
						imgui_->SetEntity(entityIT->second);
						ImGui::OpenPopup(ICON_FA_TRASH " Delete Entity");
					}
					imgui_->DeletePopUp(ICON_FA_TRASH " Delete Entity", entityname->GetName());

					ImGui::TreePop();
				}
			}
		}
	}
}

std::pair<Entity*, std::vector<ComponentTypes>> EntityWindow::GetEntityComponents(Entity* entity) {

	std::vector<ComponentTypes> comp_arr;

	if (entity) {
		ComponentArr arr = entity->GetComponentArr();
		comp_arr.reserve(arr.size());

		for (ComponentArrIt it = arr.begin(); it != arr.end(); ++it)
			comp_arr.push_back((*it)->GetComponentTypeID());
	}

	return std::make_pair(entity, comp_arr);
}

void EntityWindow::SelectEntityComponent() {

	if ((imgui_->GetEntity() && imgui_->GetEntity()->GetID() || (imgui_->GetEntity() && !imgui_->GetEntity()->GetID() && imgui_->b_edit_comp))) {

		std::pair<Entity*, std::vector<ComponentTypes>> entity = GetEntityComponents(imgui_->GetEntity());

		CheckComponentType(entity);
	}
}

void EntityWindow::DragEntityCheckBox() {

	bool lock = imgui_->GetLockBool();
	ImGui::Checkbox("Drag Entity", &lock);
	ImGui::SameLine(); imgui_->ImguiHelp("Check this box\nto Drag Entities around");
	imgui_->SetLockBool(lock);

}

void EntityWindow::DragEntity() {

	if (imgui_->GetEntity() && imgui_->GetEntity()->GetID() && input_->IsMousePressed(0) && !imgui_->EditorMode()) {

		Vector2D mousepos = input_->GetUpdatedCoords();
		EntityID checkselect = collision_->SelectEntity(mousepos);

		imgui_->SetEntity(entities_->GetEntity(checkselect));

		if (imgui_->GetEntity() && imgui_->GetLockBool()) {

			Transform* entitytrans = component_->GetComponent<Transform>(imgui_->GetEntity()->GetID());

			Vector2D TopR = component_->GetComponent<AABB>(imgui_->GetEntity()->GetID())->GetTopRight();
			Vector2D BottomL = component_->GetComponent<AABB>(imgui_->GetEntity()->GetID())->GetBottomLeft();

			Vector2D halfpt = (TopR - BottomL) / 2;
			Vector2D centre = { BottomL + halfpt };

			if (input_->IsMouseTriggered(0))
				originalVec_ = (centre - mousepos);

			if (input_->IsMousePressed(0)) {

				Vector2D entpos = mousepos + (originalVec_ -entitytrans->GetAABBOffset());
				entitytrans->SetPosition(entpos);
			}

			if (!input_->IsMousePressed(0) && !input_->IsMouseTriggered(0))
				originalVec_ = {};
		}
	}
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
	return { lineHeight + 3.0f, lineHeight };
}

void EntityWindow::EmitterInput(float& emitter_var, float default_val, std::string input_label, float start_val, float end_val, std::string button_label) {

	ImGui::PushFont(imgui_->img_font_);
	ImGui::PushFont(imgui_->bold_font_);

	if (ImGui::Button(button_label.c_str()))
		emitter_var = default_val;

	ImGui::PopFont();
	ImGui::PopFont();

	ImGui::SameLine(0, 3);

	ImGui::InputFloat(("##" + input_label).c_str(), &emitter_var, start_val, end_val, "%.2f");
}

void EntityWindow::Vec2Input(Vector2D& componentVar, float defaultVal, const char* Xlabel, const char* Ylabel) {

	imgui_->CustomImGuiButton(REDDEFAULT, REDHOVERED, REDACTIVE);

	ImGui::PushFont(imgui_->bold_font_);

	if (ImGui::Button("X", SetButtonSize()))
		componentVar.x = defaultVal;

	ImGui::PopFont();

	ImGui::PopStyleColor(3);

	ImGui::SameLine(0, 2);

	ComponentInputFloat("", Xlabel, componentVar.x, 95.0f);

	ImGui::SameLine();

	imgui_->CustomImGuiButton(GREENDEFAULT, GREENHOVERED, GREENACTIVE);

	ImGui::PushFont(imgui_->bold_font_);

	if (ImGui::Button("Y", SetButtonSize()))
		componentVar.y = defaultVal;

	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine(0, 2);

	ComponentInputFloat("", Ylabel, componentVar.y, 95.0f);
}

void EntityWindow::FloatInput(float& componentVar, const char* label, float defaultVal) {

	imgui_->CustomImGuiButton(REDDEFAULT, REDHOVERED, REDACTIVE);

	ImGui::PushFont(imgui_->bold_font_);
	if (ImGui::Button(label, SetButtonSize()))
		componentVar = defaultVal;
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine(0, 2);

	ComponentInputFloat("", "##rot", componentVar, 95.0f);
}

void EntityWindow::ComponentInputFloat(const char* componentLabel, const char* inputLabel, float& componentVar, float inputWidth, float startVal, float endVal) {
	ImGui::PushItemWidth(inputWidth);

	ImGui::Text(componentLabel);
	ImGui::SameLine();
	ImGui::DragFloat(inputLabel, &componentVar, 0.1f, startVal, endVal, "%.2f");
	ImGui::PopItemWidth();
}

void EntityWindow::ComponentInputInt(const char* componentLabel, const char* inputLabel, int& componentVar, float inputWidth, int startVal, int endVal) {
	ImGui::PushItemWidth(inputWidth);

	ImGui::Text(componentLabel);
	ImGui::SameLine();
	ImGui::InputInt(inputLabel, &componentVar, startVal, endVal);
	ImGui::PopItemWidth();
}

void EntityWindow::RemoveComponent(const char* windowName, std::string objName, Entity* entity, std::shared_ptr<Component> component) {

	if (!entity->GetID()) {
		if (ImGui::Button(ICON_FA_MINUS_SQUARE " Delete"))
			ImGui::OpenPopup(windowName);
	}
	imgui_->DeletePopUp(windowName, objName, entity, component);
}

const char* EntityWindow::GetAIState(int aiState)
{
	for (int i = 0; i < 5; ++i) {
		if (i == aiState)
			return AIstates_[i];
	}
	return nullptr;
}

const char* EntityWindow::GetAIType(int aiType)
{
	for (int i = 0; i < 3; ++i) {
		if (i == aiType)
			return AItype_[i];
	}
	return nullptr;
}

const char* EntityWindow::GetPlayerStatus(int playerState) {
	
	for (int i = 0; i < 4; ++i) {
		if (i == playerState)
			return Playerstatus_[i];
	}
	return nullptr;
}

void EntityWindow::AABBComponent(Entity* entity) {

	std::shared_ptr<AABB> entity_AABB = std::dynamic_pointer_cast<AABB>(entity->GetComponent(ComponentTypes::AABB));

	Vector2D input_AABB{ entity_AABB->GetAABBScale() };

	if (ImGui::CollapsingHeader("Collider")) {

		ImGui::Text("Collision Layer: %d", entity_AABB->GetLayer());

		ImGui::Text("Collider Scale");
		Vec2Input(input_AABB, 1.0f, "##AABBX", "##AABBY");
		entity_AABB->SetAABBScale(input_AABB);

	}
}

void EntityWindow::AIComponent(Entity* entity) {

	std::shared_ptr<AI> entity_AI = std::dynamic_pointer_cast<AI>(entity->GetComponent(ComponentTypes::AI));

	const char* entity_State = GetAIState(entity_AI->GetState());
	const char* entity_Type = GetAIType(entity_AI->GetType());

	float input_range = entity_AI->GetRange();
	int input_Atk = entity_AI->GetAtk();
	float input_speed = entity_AI->GetSpeed();

	size_t input_num_des = entity_AI->GetNumDes();
	std::vector<Vector2D> inputDes = entity_AI->GetDestinations();
	Vector2D new_Node = { entity_AI->GetNewDestination().x, entity_AI->GetNewDestination().y };
	size_t counter = 1; // to act as an id for the TreeNode

	if (ImGui::CollapsingHeader("AI Components")) {

		ImGui::Text("AI Type: ");
		ImGui::SameLine(0, 2);
		ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f, 1.0f }, entity_Type);

		if (ImGui::TreeNode("AI States")) {
			ImGui::Text("Current AI State: ");
			ImGui::SameLine(0, 2);
			ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f, 1.0f }, entity_State);

			ImGui::PushItemWidth(130.0f);
			if (ImGui::BeginCombo("##abc", "AI States")) {
				for (int i = 0; i < IM_ARRAYSIZE(AIstates_); ++i)
					if (ImGui::Selectable(AIstates_[i]))
						entity_AI->SetState(static_cast<AI::AIState>(i));

				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("AI Range")) {

			ComponentInputFloat("Range", "##AIrange", input_range, 175.0f);
			entity_AI->SetRange(input_range);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("AI Attack Power")) {

			ComponentInputInt("Power", "##AIpower", input_Atk, 175.0f, 1, 2);
			entity_AI->SetAtk(input_Atk);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("AI Speed")) {

			ComponentInputFloat("Speed", "##AIspeed", input_speed, 175.0f, 0.1f, 1.0f);
			entity_AI->SetSpeed(input_speed);

			ImGui::TreePop();
		}

		if (ImGui::TreeNode("AI Destinations")) {

			ImGui::Text("Number of Destinations: %d", input_num_des);
			ImGui::Text("Current Destinations: ");

			for (std::vector<Vector2D>::iterator it = inputDes.begin(); it != inputDes.end(); ++it) {
				++counter;
				if (ImGui::TreeNodeEx((void*)(size_t)counter, 0, "%.2f, %.2f", (*it).x, (*it).y))
				{
					if (inputDes.size() > 1) {
						if (ImGui::Button("Delete Node")) {
							if (it == inputDes.begin())
								it = inputDes.erase(it);
							else {
								it = inputDes.erase(it);
								--it;
							}
							entity_AI->GetDestinations().clear();
							entity_AI->SetDestinations(inputDes);
							entity_AI->SetNumDes(inputDes.size());
							//counter = 1;
						}
					}
					ImGui::TreePop();
				}
			}

			if (ImGui::TreeNode("Add Destination")) {

				ImGui::Text("New Node Position:");

				Vec2Input(new_Node, 0.0f, "##nodeX", "##nodeY");

				entity_AI->SetNewDestination(new_Node);

				ImGui::Text("%.2f, %.2f", new_Node.x, new_Node.y);

				if (ImGui::Button("Add")) {
					inputDes.push_back(new_Node);

					entity_AI->GetDestinations().clear();
					entity_AI->SetDestinations(inputDes);
					entity_AI->SetNumDes(inputDes.size());
					entity_AI->SetNewDestination(Vector2D{ 0.0f, 0.0f });
				}
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
		RemoveComponent("Delete AI Component", std::string("AI Component"), entity, entity_AI);
	}
}

void EntityWindow::AnimationRendererComponent(Entity* entity) {

	std::shared_ptr<AnimationRenderer> entity_animation = std::dynamic_pointer_cast<AnimationRenderer>(entity->GetComponent(ComponentTypes::ANIMATIONRENDERER));
	int input_layer = graphics_->GetLayer(&*entity_animation);

	if (ImGui::CollapsingHeader("Animation Component")) {

		ComponentInputInt("Animation Renderer Layer: ", "##animlayer", input_layer, 95.0f, 1, 1);

		graphics_->ChangeLayer(&*entity_animation, input_layer);

		if (ImGui::BeginCombo("##Animation", entity_animation->GetCurrentAnimation().c_str())) {
			for (auto it = animation_->GetAnimationMap().begin(); it != animation_->GetAnimationMap().end(); ++it)
				if (ImGui::Selectable(it->first.c_str()))
					graphics_->ChangeAnimation(&(*entity_animation), it->first.c_str());
			ImGui::EndCombo();
		}

		RemoveComponent("Delete Animation Renderer Component", std::string("Animation Renderer"), entity, entity_animation);
	}
}

void EntityWindow::CameraComponent(Entity* entity) {

	std::shared_ptr<Camera> entity_camera = std::dynamic_pointer_cast<Camera>(entity->GetComponent(ComponentTypes::CAMERA));

	Vector2D input_cam_pos = {};

	if (ImGui::CollapsingHeader("Camera Component")) {

		if (imgui_->GetExistingSceneCamera()) {
			ImGui::Text("Camera Zoom: %.2f", *entity_camera->GetCameraZoom());

			if (ImGui::Button("+", SetButtonSize()))
				camera_->CameraZoom(&*entity_camera, 0.9f);
			
			ImGui::SameLine(0, 3);

			if (ImGui::Button("-", SetButtonSize()))
				entity_camera->SetCameraZoom(&*entity_camera, 1.1f);

			ImGui::Text("Camera Position: %.2f, %.2f", entity_camera->GetVector2DCameraPosition().x, entity_camera->GetVector2DCameraPosition().y);

			ImGui::Text("Camera Offset");
			ImGui::PushItemWidth(90.0f);
			ImGui::DragFloat("##cameraposX", &input_cam_pos.x, 0.5f, 0.0f, 0.0f, "%.2f");
			ImGui::SameLine(0, 7);
			ImGui::DragFloat("##cameraposY", &input_cam_pos.y, 0.5f, 0.0f, 0.0f, "%.2f");

			ImGui::PopItemWidth();

			camera_->CameraMove(&*entity_camera, input_cam_pos);

			RemoveComponent("Delete Camera Component", std::string("Camera Component"), entity, entity_camera);
		}
	}
}

void EntityWindow::ConeLightComponent(Entity* entity) {

	std::shared_ptr<ConeLight> entity_cone_light = std::dynamic_pointer_cast<ConeLight>(entity->GetComponent(ComponentTypes::CONELIGHT));
	float input_radius = entity_cone_light->GetRadius();
	float input_intensity = entity_cone_light->GetIntensity();
	float input_angle = entity_cone_light->GetAngle();
	ImVec4 input_color = { entity_cone_light->GetColor().x, entity_cone_light->GetColor().y, entity_cone_light->GetColor().z, 1.0f };

	if (ImGui::CollapsingHeader("ConeLight")) {

		ComponentInputFloat("Light Radius", "##lightradcone", input_radius, 102.0f);
		entity_cone_light->SetRadius(input_radius);

		ComponentInputFloat("Light Intensity", "##lightintencone", input_intensity, 102.0f);
		entity_cone_light->SetIntensity(input_intensity);
		ImGui::Text("Light Color ");
		ImGui::ColorEdit3("##color", (float*)&input_color);

		glm::vec3 newColor{ input_color.x, input_color.y, input_color.z };

		entity_cone_light->SetColor(newColor);

		ComponentInputFloat("Light Angle", "##lightanglecone", input_angle, 102.0f);
		entity_cone_light->SetAngle(input_angle);

		RemoveComponent("Delete ConeLight Component", std::string("ConeLight Component"), entity, entity_cone_light);
	}
}

void EntityWindow::EmitterComponent(Entity* entity) {

	std::shared_ptr<Emitter> entity_emitter = std::dynamic_pointer_cast<Emitter>(entity->GetComponent(ComponentTypes::EMITTER));

	float input_emitterlife = entity_emitter->GetLifeTime();
	float input_interval = entity_emitter->GetInterval();
	float input_spawn_interval = entity_emitter->GetSpawnInterval();

	size_t input_request = entity_emitter->GetRequest();
	size_t input_max = entity_emitter->GetMaxNumberParticles();

	Vector2D input_lifetime = entity_emitter->GetLifeTimeStruct().lifetime_range_;

	Vector2D input_spawn_min = entity_emitter->GetPositionStruct().min_pos_;
	Vector2D input_spawn_max = entity_emitter->GetPositionStruct().max_pos_;

	Vector2D input_force = entity_emitter->GetForceStruct().force_range_;
	Vector2D input_direction = entity_emitter->GetForceStruct().direction_range_;

	Vector2D input_rotation_speed = entity_emitter->GetRotationStruct().rotation_speed_;
	Vector2D input_rotation_min = entity_emitter->GetRotationStruct().min_rotation_range_;
	Vector2D input_rotation_max = entity_emitter->GetRotationStruct().max_rotation_range_;

	size_t input_num_tex = entity_emitter->GetTextureStruct().number_of_textures_;
	std::vector<std::string> input_tex = entity_emitter->GetTextureStruct().texture_names_;

	std::string path = {};
	if (ImGui::CollapsingHeader("Emitter")) {

		ImGui::Text("Emitter Basic Infomation:");

		ImGui::Text("Emitter LifeTime:");
		ImGui::PushFont(imgui_->img_font_);
		EmitterInput(input_emitterlife, 1000.0f, "lifetime", 10.0f, 50.0f);

		entity_emitter->SetLifeTime(input_emitterlife);

		ImGui::Text("Emitter Interval:");
		EmitterInput(input_interval, 0.2f, "interval", 0.01f, 0.1f);

		ImGui::Text("Emitter Spawn Interval:");
		EmitterInput(input_spawn_interval, 0.2f, "spawninterval", 0.01f, 0.1f);

		entity_emitter->SetSpawnInterval(input_spawn_interval);
		ImGui::PopFont();

		ImGui::Separator();
		std::string status = {};
		ImGui::Text("Emitter Status: "); ImGui::SameLine(0, 4);
		if (entity_emitter->IsAlive())
			status = "Alive";
		else
			status = "Dead";

		if (ImGui::BeginCombo("Emitter Status", status.c_str())) {

			for (int i = 0; i < 2; ++i) {

				if (ImGui::Selectable(emiiterstatus_[i])) {

					entity_emitter->SetAlive(i);
				}
			}

			ImGui::EndCombo();
		}

		ComponentInputInt("Number of Request", "##req", (int&)input_request, 90.0f, 1, 2);
		entity_emitter->SetRequest(input_request);

		ImGui::Separator();

		ComponentInputInt("Max Particle Count", "##particlemax", (int&)input_max, 90.0f, 1, 5);
		entity_emitter->SetMaxNumberParticles(input_max);

		if (ImGui::CollapsingHeader("Emitter Particles Properties")) {

			if (ImGui::TreeNode("Particle LifeTime ")) {
				EmitterInput(input_lifetime.x, 2.0f, "lifetimerangeX", 0.01f, 0.1f, "Min");
				EmitterInput(input_lifetime.y, 5.0f, "lifetimerangeX", 0.01f, 0.1f, "Max");

				entity_emitter->SetLifeTimeStruct(input_lifetime);

				ImGui::Text("Particle Spawn Range:");
				EmitterInput(input_spawn_min.x, -0.25f, "spawnminX", 0.01f, 0.1f, "Min X");
				EmitterInput(input_spawn_min.y, -0.25f, "spawnminY", 0.01f, 0.1f, "Mix Y");

				ImGui::Separator();

				EmitterInput(input_spawn_max.x, 0.25f, "spawnmaxX", 0.01f, 0.1f, "Max X");
				EmitterInput(input_spawn_max.y, 0.25f, "spawnmaxY", 0.01f, 0.1f, "Max Y");

				entity_emitter->SetPositionStruct(input_spawn_min, input_spawn_max);
				ImGui::TreePop();

			}

			ImGui::Separator();

			if (ImGui::TreeNode("Particle Forces")) {

				ImGui::Text("Particle Force Range:");
				EmitterInput(input_force.x, 25.0f, "forceX", 0.1f, 1.0f, "Min");
				EmitterInput(input_force.y, 0.0f, "forceY", 0.1f, 1.0f, "Max");

				ImGui::Text("Particle Direction Range:");
				EmitterInput(input_direction.x, 0.0f, "directionX", 0.1f, 1.0f, "Min");
				EmitterInput(input_direction.y, 360.0f, "directionY", 0.1f, 1.0f, "Max");

				entity_emitter->SetForceStruct(input_force, input_direction);
				ImGui::TreePop();

			}

			ImGui::Separator();

			if (ImGui::TreeNode("Particle Rotation")) {

				ImGui::Text("Particle Rotation Speed:");

				EmitterInput(input_rotation_speed.x, 0.0f, "rotspeedX", 0.1f, 1.0f, "Min");
				EmitterInput(input_rotation_speed.y, 360.0f, "rotspeedY", 0.1f, 1.0f, "Max");

				ImGui::Text("Particle Rotation Range:");
				EmitterInput(input_rotation_min.x, 45.0f, "rotminX", 1.0f, 10.0f, "Min X");
				EmitterInput(input_rotation_min.y, 90.0f, "rotminY", 1.0f, 10.0f, "Mix Y");

				ImGui::Separator();

				EmitterInput(input_rotation_max.x, 180.0f, "rotmaxX", 1.0f, 10.0f, "Min X");
				EmitterInput(input_rotation_max.y, 225.0f, "rotmaxY", 1.0f, 10.0f, "Mix Y");

				entity_emitter->SetRotationStruct(input_rotation_speed, input_rotation_min, input_rotation_max);

				ImGui::TreePop();
			}

			ImGui::Separator();

			if (ImGui::TreeNode("Particle Texture")) {

				ImGui::Text("Number of Texture(s): %d", input_num_tex);

				ImGui::Text("Texture(s):"); ImGui::SameLine(0, 3);
				imgui_->ImguiHelp("If there are no texture set,\nthe emitter would use the \nold texture(s) before it was 'Dead'");

				if (!input_tex.empty()) {

					for (std::vector<std::string>::iterator it = input_tex.begin(); it != input_tex.end(); ++it) {

						if (ImGui::TreeNodeEx(it->c_str())) {

							ImGui::Button("Update Texture");

							if (ImGui::BeginDragDropTarget()) {

								if (const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("TEXTURE")) {

									if (payLoad->DataSize == sizeof(std::string)) {

										path = *((std::string*)payLoad->Data);

										*it = path;

										entity_emitter->SetTextureStruct(input_tex.size(), input_tex);
									}
								}
								ImGui::EndDragDropTarget();
							}

							if (ImGui::Button("Remove Texture")) {
								if (input_tex.size() > 1) {

									if (it == input_tex.begin())
										it = input_tex.erase(it);
									else {
										it = input_tex.erase(it);
										--it;
									}

									entity_emitter->SetTextureStruct(input_tex.size(), input_tex);
								}
								else if (input_tex.size() == 1) {

									input_tex.clear();
									entity_emitter->SetTextureStruct(input_tex.size(), input_tex);
								}
							}
							ImGui::TreePop();
							break;

						}
					}
				}

				ImGui::Button("Add Texture");

				if (ImGui::BeginDragDropTarget()) {

					if (const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("TEXTURE")) {

						if (payLoad->DataSize == sizeof(std::string)) {

							path = *((std::string*)payLoad->Data);

							input_tex.push_back(path);

							entity_emitter->SetTextureStruct(input_tex.size(), input_tex);
						}
					}

					ImGui::EndDragDropTarget();
				}

				ImGui::TreePop();
			}
		}
		RemoveComponent("Delete Emitter Component", std::string("Emitter Component"), entity, entity_emitter);
	}
}

void EntityWindow::HealthComponent(Entity* entity) {

	std::shared_ptr<Health> entity_health = std::dynamic_pointer_cast<Health>(entity->GetComponent(ComponentTypes::HEALTH));
	int entity_current_health = entity_health->GetCurrentHealth();
	int entity_max_health = entity_health->GetMaxHealth();

	if (ImGui::CollapsingHeader("Health")) {

		SetArrowButtons(entity_current_health);
		entity_health->SetMaxHealth(entity_current_health);

		SetArrowButtons(entity_max_health);
		entity_health->SetMaxHealth(entity_max_health);
	}
}

void EntityWindow::MotionComponent(Entity* entity) {

	std::shared_ptr<Motion> entity_motion = std::dynamic_pointer_cast<Motion>(entity->GetComponent(ComponentTypes::MOTION));

	float input_mass = entity_motion->GetMass();
	if (ImGui::CollapsingHeader("Motion")) {

		ComponentInputFloat("Mass", "##mass", input_mass);
		entity_motion->SetMass(input_mass);

		ImGui::Text("Velocity: X %.2f Y %.2f", entity_motion->GetVelocity().x, entity_motion->GetVelocity().y);

		RemoveComponent("Delete Motion Component", std::string("Motion Component"), entity, entity_motion);
	}
}

void EntityWindow::NameComponent(Entity* entity) {

	std::shared_ptr<Name> entity_name = std::dynamic_pointer_cast<Name>(entity->GetComponent(ComponentTypes::NAME));

	ImGui::Text("Name:"); ImGui::SameLine(0, 4);
	ImGui::TextColored(AQUAMARINE, entity_name->GetName().c_str());

	if (entity->GetID()) {
		ImGui::SameLine(0, 2);
		ImGui::Text(" (%d)", entity->GetID());
	}
}

void EntityWindow::ParentChildComponent(Entity* entity) {

	std::shared_ptr<ParentChild> entity_parent_child = std::dynamic_pointer_cast<ParentChild>(entity->GetComponent(ComponentTypes::PARENTCHILD));
	std::vector<Entity*> input_child = entity_parent_child->GetChildren();
	if (ImGui::CollapsingHeader("Parent Child")) {

		ImGui::Text("Number of Children: %d", input_child.size());
		ImGui::Text("Child(ren): ");
		for (size_t i = 0; i < input_child.size(); ++i) {

			if (ImGui::TreeNodeEx(CORE->GetManager<ComponentManager>()->GetComponent<Name>(input_child[i]->GetID())->GetName().c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen)) {

				ImGui::TreePop();
			}
		}

		RemoveComponent("Delete Parent Child Component", std::string("Parent Child Component"), entity, entity_parent_child);

	}
}

void EntityWindow::ParticleComponent(Entity* entity) {

	std::shared_ptr<Particle> entity_particle = std::dynamic_pointer_cast<Particle>(entity->GetComponent(ComponentTypes::PARTICLE));
	if (ImGui::CollapsingHeader("Particle Component")) {

		ImGui::Text("Particle Status: "); ImGui::SameLine(0, 4);
		if (entity_particle->IsAlive())
			ImGui::TextColored(GOLDENORANGE, "Alive");
		else
			ImGui::TextColored(REDHOVERED, "Dead");

		ImGui::Text("Particle Lifespan: %.2f", entity_particle->GetLifetime());

		RemoveComponent("Delete Particle Component", std::string("Particle Component"), entity, entity_particle);
	}
}

void EntityWindow::PointLightComponent(Entity* entity) {

	std::shared_ptr<PointLight> entity_point_light = std::dynamic_pointer_cast<PointLight>(entity->GetComponent(ComponentTypes::POINTLIGHT));

	float input_radius = entity_point_light->GetRadius();
	float input_intensity = entity_point_light->GetIntensity();
	ImVec4 input_color{ entity_point_light->GetColor().x, entity_point_light->GetColor().y, entity_point_light->GetColor().z, 1.0f };

	if (ImGui::CollapsingHeader("PointLight")) {

		ComponentInputFloat("Light Radius", "##lightRad", input_radius, 102.0f);
		entity_point_light->SetRadius(input_radius);

		ComponentInputFloat("Light Intensity", "##lightinten", input_intensity, 102.0f);
		entity_point_light->SetIntensity(input_intensity);
		ImGui::Text("Light Color ");
		ImGui::ColorEdit3("##color", (float*)&input_color);

		entity_point_light->SetColor(glm::vec3{ input_color.x, input_color.y, input_color.z });

		RemoveComponent("Delete PointLight Component", std::string("PointLight Component"), entity, entity_point_light);
	}
}

void EntityWindow::ScaleComponent(Entity* entity) {

	std::shared_ptr<Scale> entity_scale = std::dynamic_pointer_cast<Scale>(entity->GetComponent(ComponentTypes::SCALE));

	Vector2D input_obj_scale = { entity_scale->GetScale() };

	if (ImGui::CollapsingHeader("Scale")) {

		Vec2Input(input_obj_scale, 5.0f, "##scaleX", "scaleY");
		entity_scale->SetScale(input_obj_scale);
	}
}

void EntityWindow::SoundEmitterComponent(Entity* entity) {

	std::shared_ptr<SoundEmitter> entity_sound = std::dynamic_pointer_cast<SoundEmitter>(entity->GetComponent(ComponentTypes::SOUNDEMITTER));

	std::string input_sound_name = entity_sound->GetSoundName();
	size_t input_num_sound = entity_sound->GetSoundLines().size();
	std::vector<SoundLine> input_sound_lines = entity_sound->GetSoundLines();
	SoundLine new_line = entity_sound->GetNewSoundLine();

	std::string path = {};
	size_t counter = 0;

	if (ImGui::CollapsingHeader("Sound Emitter Component")) {
		ImGui::Text("Current Audio:");
		if (ImGui::BeginCombo("##sounds", input_sound_name.c_str())) {

			for (auto it = sound_->GetSoundLibrary().begin(); it != sound_->GetSoundLibrary().end(); ++it) {

				if (ImGui::Selectable(it->first.c_str()))
					entity_sound->SetSoundName(it->first.c_str());
			}

			ImGui::EndCombo();
		}

		ImGui::Text("Number of Sound Lines: %d", input_num_sound);

		for (std::vector<SoundLine>::iterator begin = input_sound_lines.begin(); begin != input_sound_lines.end(); ++begin) {
			++counter;
			std::string label = "Line " + std::to_string(counter);
			if (ImGui::CollapsingHeader(label.c_str())) {

				if (ImGui::TreeNodeEx((void*)(size_t)counter, 0, "Start Point X: %.2f Y: %.2f\nEnd Point: X: %.2f Y: %.2f", begin->start_.x, begin->start_.y, begin->end_.x, begin->end_.y)) {

					if (ImGui::TreeNode("Update Line Segment")) {

						ImGui::Text("Start Point:");
						Vec2Input(begin->start_, 1.0f, "##startx", "##starty");
						ImGui::Text("End Point:");
						Vec2Input(begin->end_, 1.0f);

						entity_sound->SetSoundLine(input_sound_lines);
						entity_sound->SetNumberLines(input_sound_lines.size());
						ImGui::TreePop();
					}

					if (ImGui::Button("Delete")) {

						if (begin == input_sound_lines.begin())
							begin = input_sound_lines.erase(begin);
						else {
							begin = input_sound_lines.erase(begin);
							--begin;
						}

						entity_sound->SetSoundLine(input_sound_lines);
						entity_sound->SetNumberLines(input_sound_lines.size());
					}
					ImGui::TreePop();
				}
			}
		}

		if (ImGui::TreeNode("Add Line Segment")) {

			ImGui::Text("Start Point:");
			Vec2Input(new_line.start_, 1.0f, "##newlineX", "##newlineY");
			ImGui::Text("End Point");
			Vec2Input(new_line.end_);

			entity_sound->SetNewSoundLine(new_line);

			if (ImGui::Button("Add")) {

				input_sound_lines.push_back(new_line);
				entity_sound->SetSoundLine(input_sound_lines);
				entity_sound->SetNumberLines(input_sound_lines.size());
				entity_sound->SetNewSoundLine({});
			}

			ImGui::TreePop();
		}

		RemoveComponent("Delete Sound Emitter Component", std::string("Sound Emitter Component"), entity, entity_sound);
	}
}

void EntityWindow::StatusComponent(Entity* entity) {

	std::shared_ptr<Status> entity_status = std::dynamic_pointer_cast<Status>(entity->GetComponent(ComponentTypes::STATUS));

	const char* input_state = GetPlayerStatus(static_cast<int>(entity_status->GetStatus()));

	float input_status_timer = entity_status->GetStatusTimer();
	float input_cool_down = entity_status->GetCooldownTimer();

	if (ImGui::CollapsingHeader("Status")) {

		ImGui::Text("Current Status: "); ImGui::SameLine(0, 4); ImGui::Text(input_state);
		ImGui::PushItemWidth(170.0f);

		if (ImGui::BeginCombo("##combo", "Player Status")) {

			for (int i = 0; i < IM_ARRAYSIZE(Playerstatus_); ++i) {

				if (ImGui::Selectable(Playerstatus_[i]))
					entity_status->SetStatus(static_cast<StatusType>(i));
			}
			ImGui::EndCombo();
		}

		ImGui::PopItemWidth();

		if (ImGui::TreeNode("Timers")) {

			ComponentInputFloat("Status Timer", "##playerstatus", input_status_timer);
			entity_status->SetStatusTimer(input_status_timer);

			ComponentInputFloat("CoolDown Timer", "##cooldowntimer", input_cool_down);
			entity_status->SetCoolDownTimer(input_cool_down);

			ImGui::TreePop();
		}
		RemoveComponent("Delete Status Component", std::string("Status Component"), entity, entity_status);
	}
}

void EntityWindow::TextureRendererComponent(Entity* entity) {

	std::shared_ptr<TextureRenderer> entity_texture = std::dynamic_pointer_cast<TextureRenderer>(entity->GetComponent(ComponentTypes::TEXTURERENDERER));

	if (ImGui::CollapsingHeader("Texture Component")) {

		int input_layer = graphics_->GetLayer(&*entity_texture);

		ComponentInputInt("Texture Renderer Layer: ", "##texlayer", input_layer, 95.0f, 1, 1);

		graphics_->ChangeLayer(&*entity_texture, input_layer);

		int input_ui = entity_texture->GetUI();

		ComponentInputInt("UI: ", "##uilayer", input_ui, 95.0f, 0, 1);

		entity_texture->SetUI(input_ui);

		std::string path = {};

		entity_texture->GetCurrentTextureName().empty() ? ImGui::Text("No Texture has been set") : ImGui::Text(entity_texture->GetCurrentTextureName().c_str());

		if (ImGui::BeginDragDropTarget()) {

			if (const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("TEXTURE")) {

				if (payLoad->DataSize == sizeof(std::string)) {

					path = *((std::string*)payLoad->Data);

					graphics_->ChangeTexture(&(*entity_texture), path);
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (!entity_texture->GetCurrentTextureName().empty()) {

			Texture* texture = texture_->GetTexture(entity_texture->GetCurrentTextureName());
			std::vector<glm::vec2>* tex_vtx = texture->GetTexVtx();
			ImTextureID texID = (void*)(intptr_t)texture->GetTilesetHandle();

			if (tex_vtx->size())
				ImGui::Image(texID, ImVec2{ 64, 64 }, ImVec2{ (*tex_vtx)[2].x, (*tex_vtx)[2].y }, ImVec2{ (*tex_vtx)[1].x, (*tex_vtx)[1].y });
			else 
				ImGui::Text("MISSING TEXTURE!!!!!");

		}

		RemoveComponent("Delete Texture Renderer Component", std::string("Texture Renderer"), entity, entity_texture);
	}
}

void EntityWindow::TransformComponent(Entity* entity) {

	std::shared_ptr<Transform> entity_transform = std::dynamic_pointer_cast<Transform>(entity->GetComponent(ComponentTypes::TRANSFORM));

	float input_rot = entity_transform->GetRotation();
	Vector2D input_pos = { entity_transform->GetPosition() };
	Vector2D input_offset{ entity_transform->GetOffset() };
	Vector2D input_aabb_offset{ entity_transform->GetAABBOffset() };

	if (ImGui::CollapsingHeader("Transform Component")) {

		ImGui::Text("Rotation");
		FloatInput(input_rot, "X##transrot");
		entity_transform->SetRotation(input_rot);
		ImGui::Separator();

		ImGui::Text("Position");
		Vec2Input(input_pos, 0.0f, "##Xpos", "##Ypos");
		entity_transform->SetPosition(input_pos);

		ImGui::Text("Child Offset");
		Vec2Input(input_offset, 0.0f, "##Xoffslv", "##Yoffslv");
		entity_transform->SetOffset(input_offset);

		ImGui::Text("Collider Offset");
		Vec2Input(input_aabb_offset, 0.0f, "##X off", "##Y off");
		entity_transform->SetAABBOffset(input_aabb_offset);
	}
}
