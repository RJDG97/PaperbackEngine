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

	originalVec_ = { 0,0 };	
	b_draw = false;
	b_grid = false;
}

void EntityWindow::Update() {
	//ImGui::ShowDemoWindow();
	//ImGui::ShowStyleEditor();
	if (imgui_->b_entitywin) {

		ImGui::Begin("Entity Inspector", &imgui_->b_entitywin);

		ImGui::Separator();

		ShowEntityList();

		ImGui::End();
	}

	if (imgui_->b_component) {

		ImGui::Begin("Component Inspector", &imgui_->b_component);

		DragEntityCheckBox();

		ImGui::SameLine(0, 3);

		if (imgui_->GetCamera())
			ImGui::Checkbox("Draw Grid", &b_grid);

		if (b_grid)
			imgui_->DrawGrid();

		SelectEntityComponent();// to see the components
		ImGui::End();
	}

	DragEntity();
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

void EntityWindow::CheckComponentType(std::pair<Entity*, std::vector<ComponentTypes>> entitycomponent) {

	if (entitycomponent.first) {

		imgui_->ImguiHelp("1.  For all components with sliders Ctrl + Click to type manually.\n2. Click on the side buttons to reset the value to default");
		ImGui::SameLine(0, 3); ImGui::Text("-> Hover here for help :)");
		!entitycomponent.first->GetID() ? ImGui::TextColored(GOLDENORANGE, "Type: Archetype/Prefab") : ImGui::TextColored(SKYBLUE, "Type: Entity");
		for (auto componenttype : entitycomponent.second) {

			switch (componenttype)
			{
				case ComponentTypes::NAME:
				{
					std::shared_ptr<Name> entityname = std::dynamic_pointer_cast<Name>(entitycomponent.first->GetComponent(ComponentTypes::NAME));

					ImGui::Text("Name:"); ImGui::SameLine(0, 4);
					ImGui::TextColored(AQUAMARINE, entityname->GetName().c_str());
				
					if (entitycomponent.first->GetID()) {
						ImGui::SameLine(0, 2);
						ImGui::Text(" (%d)", entitycomponent.first->GetID());
					}

					break;
				}
				case ComponentTypes::MOTION:
				{
					std::shared_ptr<Motion> entitymotion = std::dynamic_pointer_cast<Motion>(entitycomponent.first->GetComponent(ComponentTypes::MOTION));

					float inputMass = entitymotion->GetMass();
					if (ImGui::CollapsingHeader("Motion")) {

						ComponentInputFloat("Mass", "##mass", inputMass);
						entitymotion->SetMass(inputMass);

						ImGui::Text("Velocity: X %.2f Y %.2f", entitymotion->GetVelocity().x, entitymotion->GetVelocity().y);

						RemoveComponent("Delete Motion Component", std::string("Motion Component"), entitycomponent.first, entitymotion);
					}
					break;
				}
				case ComponentTypes::TRANSFORM:
				{
					std::shared_ptr<Transform> entitytransform = std::dynamic_pointer_cast<Transform>(entitycomponent.first->GetComponent(ComponentTypes::TRANSFORM));

					float inputRot = entitytransform->GetRotation();
					Vector2D inputPos = { entitytransform->GetPosition() };
					Vector2D inputOffset{ entitytransform->GetOffset() };
					Vector2D inputAABBOffset{ entitytransform->GetAABBOffset() };

					if (ImGui::CollapsingHeader("Transform Component")) {

						ImGui::Text("Rotation");
						FloatInput(inputRot);
						entitytransform->SetRotation(inputRot);
						ImGui::Separator();

						ImGui::Text("Position");
						Vec2Input(inputPos);
						entitytransform->SetPosition(inputPos);

						ImGui::Text("Child Offset");
						Vec2Input(inputOffset, 0.0f, "##Xoffslv", "##Yoffslv");
						entitytransform->SetOffset(inputOffset);

						ImGui::Text("Collider Offset");
						Vec2Input(inputAABBOffset, 0.0f, "##X off", "##Y off");
						entitytransform->SetAABBOffset(inputAABBOffset);

					}
					break;
				}
				case ComponentTypes::HEALTH:
				{
					std::shared_ptr<Health> entityHealth = std::dynamic_pointer_cast<Health>(entitycomponent.first->GetComponent(ComponentTypes::HEALTH));
					int entityCurrenthealth = entityHealth->GetCurrentHealth();
					int entityMaxhealth = entityHealth->GetMaxHealth();

					if (ImGui::CollapsingHeader("Health")) {

						SetArrowButtons(entityCurrenthealth);
						entityHealth->SetMaxHealth(entityCurrenthealth);

						SetArrowButtons(entityMaxhealth);
						entityHealth->SetMaxHealth(entityMaxhealth);

					}
					break;
				}
				case ComponentTypes::CAMERA:
				{
					std::shared_ptr<Camera> entityCamera = std::dynamic_pointer_cast<Camera>(entitycomponent.first->GetComponent(ComponentTypes::CAMERA));

					if (ImGui::CollapsingHeader("Camera Component")) {
						ImGui::Text("Camera Zoom: %.2f", *entityCamera->GetCameraZoom());

						if (ImGui::Button("+", SetButtonSize()))
							entityCamera->SetCameraZoom(&*entityCamera, 0.9f);

						ImGui::SameLine(0, 3);

						if (ImGui::Button("-", SetButtonSize()))
							entityCamera->SetCameraZoom(&*entityCamera, 1.1f);

						ImGui::Text("Camera Position: %.2f, %.2f", entityCamera->GetVector2DCameraPosition().x, entityCamera->GetVector2DCameraPosition().y);
						// ask for help with regards to moving camera around

						RemoveComponent("Delete Camera Component", std::string("Camera Component"), entitycomponent.first, entityCamera);

					}
				}
					break;
				case ComponentTypes::CONTROLLER:
					break;
				case ComponentTypes::TEXTURERENDERER:
				{
					std::shared_ptr<TextureRenderer> entitytexture = std::dynamic_pointer_cast<TextureRenderer>(entitycomponent.first->GetComponent(ComponentTypes::TEXTURERENDERER));
					if (ImGui::CollapsingHeader("Texture Component")) {
						std::string path = {};

						entitytexture->GetCurrentTextureName().empty() ? ImGui::Text("No Texture has been set") : ImGui::Text(entitytexture->GetCurrentTextureName().c_str());

						if (ImGui::BeginDragDropTarget()) {

							if (const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("TEXTURE")) {

								if (payLoad->DataSize == sizeof(std::string)) {

									path = *((std::string*)payLoad->Data);

									graphics_->ChangeTexture(&(*entitytexture), path);
								}
							}

							ImGui::EndDragDropTarget();
						}

						if (!entitytexture->GetCurrentTextureName().empty()) {

							Texture* texture = texture_->GetTexture(entitytexture->GetCurrentTextureName());
							std::vector<glm::vec2>* tex_vtx = texture->GetTexVtx();
							ImTextureID texID = (void*)(intptr_t)texture->GetTilesetHandle();

							ImGui::Image(texID, ImVec2{ 64, 64 }, ImVec2{ (*tex_vtx)[2].x, (*tex_vtx)[2].y }, ImVec2{ (*tex_vtx)[1].x, (*tex_vtx)[1].y });
						}

						RemoveComponent("Delete Texture Renderer Component", std::string("Texture Renderer"), entitycomponent.first, entitytexture);
					}
				}
					break;
				case ComponentTypes::ANIMATIONRENDERER:
				{
					std::shared_ptr<AnimationRenderer> entityanim = std::dynamic_pointer_cast<AnimationRenderer>(entitycomponent.first->GetComponent(ComponentTypes::ANIMATIONRENDERER));
					if (ImGui::CollapsingHeader("Animation Component")) {
						if (ImGui::BeginCombo("##Animation", entityanim->GetCurrentAnimation().c_str())) {
							for (auto it = animation_->GetAnimationMap().begin(); it != animation_->GetAnimationMap().end(); ++it)
								if (ImGui::Selectable(it->first.c_str()))
									graphics_->ChangeAnimation(&(*entityanim), it->first.c_str());
							ImGui::EndCombo();
						}

						RemoveComponent("Delete Animation Renderer Component", std::string("Animation Renderer"), entitycomponent.first, entityanim);
					}
				}
					break;
				case ComponentTypes::TEXTRENDERER:
					break;
				case ComponentTypes::AABB:
				{
					std::shared_ptr<AABB> entityAABB = std::dynamic_pointer_cast<AABB>(entitycomponent.first->GetComponent(ComponentTypes::AABB));

					Vector2D inputAABB{ entityAABB->GetAABBScale() };

					if (ImGui::CollapsingHeader("Collider")) {

						ImGui::Text("Collision Layer: %d", entityAABB->GetLayer());

						Vec2Input(inputAABB);
						entityAABB->SetAABBScale(inputAABB);

						if (ImGui::Checkbox("Draw Bounding Box", &b_draw)) {

							Message msg(MessageIDTypes::DEBUG_ALL);
							CORE->BroadcastMessage(&msg);
						}

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
					size_t counter = 1; // to act as an id for the TreeNode

					if (ImGui::CollapsingHeader("AI Components")) {

						ImGui::Text("AI Type: ");
						ImGui::SameLine(0, 2);
						ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f, 1.0f }, entityType);

						if (ImGui::TreeNode("AI States")) {
							ImGui::Text("Current AI State: ");
							ImGui::SameLine(0, 2);
							ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f, 1.0f }, entityState);

							ImGui::PushItemWidth(130.0f);
							if (ImGui::BeginCombo("##abc", "AI States")) {
								for (int i = 0; i < IM_ARRAYSIZE(AIstates_); ++i)
									if (ImGui::Selectable(AIstates_[i]))
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
											entityAI->GetDestinations().clear();
											entityAI->SetDestinations(inputDes);
											//counter = 1;
										}
									}
									ImGui::TreePop();
								}
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
						RemoveComponent("Delete AI Component", std::string("AI Component"), entitycomponent.first, entityAI);
					}
					break;
				}
				case ComponentTypes::SCALE:
				{
					std::shared_ptr<Scale> entityscale = std::dynamic_pointer_cast<Scale>(entitycomponent.first->GetComponent(ComponentTypes::SCALE));

					Vector2D inputScale = { entityscale->GetScale() };

					if (ImGui::CollapsingHeader("Scale")) {
					
						Vec2Input(inputScale, 5.0f);
						entityscale->SetScale(inputScale);
					}
					break;
				}
				case ComponentTypes::STATUS:
				{
					std::shared_ptr<Status> entitystatus = std::dynamic_pointer_cast<Status>(entitycomponent.first->GetComponent(ComponentTypes::STATUS));
			
					const char* inputState = GetPlayerStatus(static_cast<int>(entitystatus->GetStatus()));

					float inputStatustimer = entitystatus->GetStatusTimer();
					float inputcooldown = entitystatus->GetCooldownTimer();

					if (ImGui::CollapsingHeader("Status")) {

						ImGui::Text("Current Status: "); ImGui::SameLine(0, 4); ImGui::Text(inputState);
						ImGui::PushItemWidth(170.0f);

						if (ImGui::BeginCombo("##combo", "Player Status")) {

							for (int i = 0; i < IM_ARRAYSIZE(Playerstatus_); ++i) {

								if (ImGui::Selectable(Playerstatus_[i]))
									entitystatus->SetStatus(static_cast<StatusType>(i));
							}
							ImGui::EndCombo();
						}

						ImGui::PopItemWidth();

						if (ImGui::TreeNode("Timers")) {

							ComponentInputFloat("Status Timer", "##status", inputStatustimer);
							entitystatus->SetStatusTimer(inputStatustimer);

							ComponentInputFloat("CoolDown Timer", "##timer", inputcooldown);
							entitystatus->SetCoolDownTimer(inputcooldown);

							ImGui::TreePop();
						}

						RemoveComponent("Delete Status Component", std::string("Status Component"), entitycomponent.first, entitystatus);
					
					}
					break;
				}
				case ComponentTypes::POINTLIGHT:
				{
					std::shared_ptr<PointLight> entitypointlight = std::dynamic_pointer_cast<PointLight>(entitycomponent.first->GetComponent(ComponentTypes::POINTLIGHT));

					float inputRadius = entitypointlight->GetRadius();
					float inputIntensity = entitypointlight->GetIntensity();
					ImVec4 inputColor{ entitypointlight->GetColor().x, entitypointlight->GetColor().y, entitypointlight->GetColor().z, 1.0f };

					if (ImGui::CollapsingHeader("PointLight")) {

						ComponentInputFloat("Light Radius", "##lightRad", inputRadius, 102.0f);
						entitypointlight->SetRadius(inputRadius);
					
						ComponentInputFloat("Light Intensity", "##lightinten", inputIntensity, 102.0f);
						entitypointlight->SetIntensity(inputIntensity);
						ImGui::Text("Light Color ");
						ImGui::ColorEdit3("##color", (float*)&inputColor);

						entitypointlight->SetColor(glm::vec3{ inputColor.x, inputColor.y, inputColor.z });

						RemoveComponent("Delete PointLight Component", std::string("PointLight Component"), entitycomponent.first, entitypointlight);
					}
					break;
				}
				case ComponentTypes::CONELIGHT:
				{
					std::shared_ptr<ConeLight> entityconelight = std::dynamic_pointer_cast<ConeLight>(entitycomponent.first->GetComponent(ComponentTypes::CONELIGHT));
					float inputRadius = entityconelight->GetRadius();
					float inputIntensity = entityconelight->GetIntensity();
					float inputAngle = entityconelight->GetAngle();
					ImVec4 inputColor = { entityconelight->GetColor().x, entityconelight->GetColor().y, entityconelight->GetColor().z, 1.0f };

					if (ImGui::CollapsingHeader("ConeLight")) {

						ComponentInputFloat("Light Radius", "##lightradcone", inputRadius, 102.0f);
						entityconelight->SetRadius(inputRadius);

						ComponentInputFloat("Light Intensity", "##lightintencone", inputIntensity, 102.0f);
						entityconelight->SetIntensity(inputIntensity);
						ImGui::Text("Light Color ");
						ImGui::ColorEdit3("##color", (float*)&inputColor);

						glm::vec3 newColor{ inputColor.x, inputColor.y, inputColor.z };

						entityconelight->SetColor(newColor);

						ComponentInputFloat("Light Angle", "##lightanglecone", inputAngle, 102.0f);
						entityconelight->SetAngle(inputAngle);

						RemoveComponent("Delete ConeLight Component", std::string("ConeLight Component"), entitycomponent.first, entityconelight);
					}
				}
					break;
				case ComponentTypes::CLICKABLE:
					break;
				case ComponentTypes::INPUTCONTROLLER:
					break;
				case ComponentTypes::PARENTCHILD: 
				{
					std::shared_ptr<ParentChild> entityparentchild = std::dynamic_pointer_cast<ParentChild>(entitycomponent.first->GetComponent(ComponentTypes::PARENTCHILD));
					std::vector<Entity*> inputChild = entityparentchild->GetChildren();
					if (ImGui::CollapsingHeader("Parent Child")) {

						ImGui::Text("Number of Children: %d", inputChild.size());
						ImGui::Text("Child(ren): ");
						for (size_t i = 0; i < inputChild.size(); ++i) {

							if (ImGui::TreeNodeEx(CORE->GetManager<ComponentManager>()->GetComponent<Name>(inputChild[i]->GetID())->GetName().c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen)) {

								ImGui::TreePop();
							}
						}

						RemoveComponent("Delete Parent Child Component", std::string("Parent Child Component"), entitycomponent.first, entityparentchild);

					}
				}
					break;
				case ComponentTypes::LOGICCOMPONENT:
					break;
				case ComponentTypes::INVENTORY:
					break;
				case ComponentTypes::PARTICLE:
				{
					std::shared_ptr<Particle> entityparticle = std::dynamic_pointer_cast<Particle>(entitycomponent.first->GetComponent(ComponentTypes::PARTICLE));
					if (ImGui::CollapsingHeader("Particle Component")) {

						ImGui::Text("Particle Status: "); ImGui::SameLine(0, 4);
						if (entityparticle->IsAlive())
							ImGui::TextColored(GOLDENORANGE, "Alive");
						else
							ImGui::TextColored(REDHOVERED, "Dead");

						ImGui::Text("Particle Lifespan: %.2f", entityparticle->GetLifetime());

						RemoveComponent("Delete Particle Component", std::string("Particle Component"), entitycomponent.first, entityparticle);
					}
				}
					break;
				case ComponentTypes::EMITTER:
				{
					std::shared_ptr<Emitter> entityemitter = std::dynamic_pointer_cast<Emitter>(entitycomponent.first->GetComponent(ComponentTypes::EMITTER));
				
					float input_emitterlife = entityemitter->GetLifeTime();
					float inputinterval = entityemitter->GetInterval();
					float inputspawninterval = entityemitter->GetSpawnInterval();
				
					size_t inputrequest = entityemitter->GetRequest();
					size_t inputmax = entityemitter->GetMaxNumberParticles();

					Vector2D inputlifetime = entityemitter->GetLifeTimeStruct().lifetime_range_;

					Vector2D inputspawnMin = entityemitter->GetPositionStruct().min_pos_;
					Vector2D inputspawnMax = entityemitter->GetPositionStruct().max_pos_;

					Vector2D inputforce = entityemitter->GetForceStruct().force_range_;
					Vector2D inputdirection = entityemitter->GetForceStruct().direction_range_;

					Vector2D inputRotationspeed = entityemitter->GetRotationStruct().rotation_speed_;
					Vector2D inputRotationMin = entityemitter->GetRotationStruct().min_rotation_range_;
					Vector2D inputRotationMax = entityemitter->GetRotationStruct().max_rotation_range_;

					size_t inputnumtex = entityemitter->GetTextureStruct().number_of_textures_;
					std::vector<std::string> inputtex = entityemitter->GetTextureStruct().texture_names_;

					std::string path = {};
					if (ImGui::CollapsingHeader("Emitter")) {

						ImGui::Text("Emitter Basic Infomation:");

						ImGui::Text("Emitter LifeTime:");
						ImGui::PushFont(imgui_->img_font_);
						EmitterInput(input_emitterlife, 1000.0f, "lifetime", 10.0f, 50.0f);

						entityemitter->SetLifeTime(input_emitterlife);

						ImGui::Text("Emitter Interval:");
						EmitterInput(inputinterval, 0.2f, "interval", 0.01f, 0.1f);

						ImGui::Text("Emitter Spawn Interval:");
						EmitterInput(inputspawninterval, 0.2f, "spawninterval", 0.01f, 0.1f);

						entityemitter->SetSpawnInterval(inputspawninterval);
						ImGui::PopFont();

						ImGui::Separator();
						std::string status = {};
						ImGui::Text("Emitter Status: "); ImGui::SameLine(0, 4);
						if (entityemitter->IsAlive())
							status = "Alive";
						else
							status = "Dead";

						if (ImGui::BeginCombo("Emitter Status", status.c_str())) {

							for (int i = 0; i < 2; ++i) {

								if (ImGui::Selectable(emiiterstatus_[i])) {

									entityemitter->SetAlive(i);
								}
							}

							ImGui::EndCombo();
						}

						ComponentInputInt("Number of Request", "##req", (int&)inputrequest, 90.0f, 1, 2);
						entityemitter->SetRequest(inputrequest);

						ImGui::Separator();

						ComponentInputInt("Max Particle Count", "##max", (int&)inputmax, 90.0f, 1, 5);
						entityemitter->SetMaxNumberParticles(inputmax);

						if (ImGui::CollapsingHeader("Emitter Particles Properties")) {

							if (ImGui::TreeNode("Particle LifeTime ")) {
								EmitterInput(inputlifetime.x, 2.0f, "lifetimerangeX", 0.01f, 0.1f, "Min");
								EmitterInput(inputlifetime.y, 5.0f, "lifetimerangeX", 0.01f, 0.1f, "Max");

								entityemitter->SetLifeTimeStruct(inputlifetime);

								ImGui::Text("Particle Spawn Range:");
								EmitterInput(inputspawnMin.x, -0.25f, "spawnminX", 0.01f, 0.1f, "Min X");
								EmitterInput(inputspawnMin.y, -0.25f, "spawnminY", 0.01f, 0.1f, "Mix Y");

								ImGui::Separator();

								EmitterInput(inputspawnMax.x, 0.25f, "spawnmaxX", 0.01f, 0.1f, "Max X");
								EmitterInput(inputspawnMax.y, 0.25f, "spawnmaxY", 0.01f, 0.1f, "Max Y");

								entityemitter->SetPositionStruct(inputspawnMin, inputspawnMax);
								ImGui::TreePop();

							}

							ImGui::Separator();

							if (ImGui::TreeNode("Particle Forces")) {

								ImGui::Text("Particle Force Range:");
								EmitterInput(inputforce.x, 25.0f, "forceX", 0.1f, 1.0f, "Min");
								EmitterInput(inputforce.y, 0.0f, "forceY", 0.1f, 1.0f, "Max");

								ImGui::Text("Particle Direction Range:");
								EmitterInput(inputdirection.x, 0.0f, "directionX", 0.1f, 1.0f, "Min");
								EmitterInput(inputdirection.y, 360.0f, "directionY", 0.1f, 1.0f, "Max");

								entityemitter->SetForceStruct(inputforce, inputdirection);
								ImGui::TreePop();

							}

							ImGui::Separator();

							if (ImGui::TreeNode("Particle Rotation")) {

								ImGui::Text("Particle Rotation Speed:");

								EmitterInput(inputRotationspeed.x, 0.0f, "rotspeedX", 0.1f, 1.0f, "Min");
								EmitterInput(inputRotationspeed.y, 360.0f, "rotspeedY", 0.1f, 1.0f, "Max");

								ImGui::Text("Particle Rotation Range:");
								EmitterInput(inputRotationMin.x, 45.0f, "rotminX", 1.0f, 10.0f, "Min X");
								EmitterInput(inputRotationMin.y, 90.0f, "rotminY", 1.0f, 10.0f, "Mix Y");

								ImGui::Separator();

								EmitterInput(inputRotationMax.x, 180.0f, "rotmaxX", 1.0f, 10.0f, "Min X");
								EmitterInput(inputRotationMax.y, 225.0f, "rotmaxY", 1.0f, 10.0f, "Mix Y");

								entityemitter->SetRotationStruct(inputRotationspeed, inputRotationMin, inputRotationMax);

								ImGui::TreePop();
							}

							ImGui::Separator();

							if (ImGui::TreeNode("Particle Texture")) {

								ImGui::Text("Number of Texture(s): %d", inputnumtex);

								ImGui::Text("Texture(s):"); ImGui::SameLine(0, 3);
								imgui_->ImguiHelp("If there are no texture set,\nthe emitter would use the \nold texture(s) before it was 'Dead'");

								if (!inputtex.empty()) {

									for (std::vector<std::string>::iterator it = inputtex.begin(); it != inputtex.end(); ++it) {

										if (ImGui::TreeNodeEx(it->c_str())) {

											ImGui::Button("Update Texture");

											if (ImGui::BeginDragDropTarget()) {

												if (const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("TEXTURE")) {

													if (payLoad->DataSize == sizeof(std::string)) {

														path = *((std::string*)payLoad->Data);

														*it = path;

														entityemitter->SetTextureStruct(inputtex.size(), inputtex);
													}
												}
												ImGui::EndDragDropTarget();
											}

											if (ImGui::Button("Remove Texture")) {
												if (inputtex.size() > 1) {

													if (it == inputtex.begin())
														it = inputtex.erase(it);
													else {
														it = inputtex.erase(it);
														--it;
													}

													entityemitter->SetTextureStruct(inputtex.size(), inputtex);
												}
												else if (inputtex.size() == 1) {
													inputtex.clear();
													entityemitter->SetTextureStruct(inputtex.size(), inputtex);
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

											inputtex.push_back(path);

											entityemitter->SetTextureStruct(inputtex.size(), inputtex);
										}
									}

									ImGui::EndDragDropTarget();
								}

								ImGui::TreePop();
							}
						}

						RemoveComponent("Delete Emitter Component", std::string("Emitter Component"), entitycomponent.first, entityemitter);
					}
				}
					break;
				case ComponentTypes::SOUNDEMITTER:
				{
					std::shared_ptr<SoundEmitter> entitySound = std::dynamic_pointer_cast<SoundEmitter>(entitycomponent.first->GetComponent(ComponentTypes::SOUNDEMITTER));

					std::string input_sound_name = entitySound->GetSoundName();
					size_t input_num_sound = entitySound->GetSoundLines().size();
					std::vector<SoundLine> input_sound_lines = entitySound->GetSoundLines();
					SoundLine newline = entitySound->GetNewSoundLine();

					std::string path = {};
					size_t counter = 0;

					if (ImGui::CollapsingHeader("Sound Emitter Component")) {
						ImGui::Text("Current Audio:");
						if (ImGui::BeginCombo("##sounds", input_sound_name.c_str())) {

							for (auto it = sound_->GetSoundLibrary().begin(); it != sound_->GetSoundLibrary().end(); ++it) {

								if (ImGui::Selectable(it->first.c_str()))
									entitySound->SetSoundName(it->first.c_str());
							}

							ImGui::EndCombo();
						}

						ImGui::Text("Number of Sound Lines: %d", input_num_sound);

						for (std::vector<SoundLine>::iterator begin = input_sound_lines.begin(); begin != input_sound_lines.end(); ++begin) {
							++counter;

							if (ImGui::TreeNodeEx((void*)(size_t)counter, 0, "Start Point X: %.2f Y: %.2f\nEnd Point: X: %.2f Y: %.2f", begin->start_.x, begin->start_.y, begin->end_.x, begin->end_.y)) {

								if (ImGui::TreeNode("Update Line Segment")) {

									ImGui::Text("Start Point:");
									Vec2Input(begin->start_, 1.0f, "##startx", "##starty");
									ImGui::Text("End Point:");
									Vec2Input(begin->end_, 1.0f);

									entitySound->SetSoundLine(input_sound_lines);

									ImGui::TreePop();
								}

								if (ImGui::Button("Delete")) {

									if (begin == input_sound_lines.begin())
										begin = input_sound_lines.erase(begin);
									else {
										begin = input_sound_lines.erase(begin);
										--begin;
									}

									entitySound->SetSoundLine(input_sound_lines);
								}

								ImGui::TreePop();
							}
						}

						if (ImGui::TreeNode("Add Line Segment")) {

							ImGui::Text("Start Point:");
							Vec2Input(newline.start_, 1.0f, "##newlineX", "##newlineY");
							ImGui::Text("End Point");
							Vec2Input(newline.end_);

							entitySound->SetNewSoundLine(newline);

							if (ImGui::Button("Add")) {

								input_sound_lines.push_back(newline);
								entitySound->SetSoundLine(input_sound_lines);
								entitySound->SetNewSoundLine({});
							}

							ImGui::TreePop();
						}

						RemoveComponent("Delete Sound Emitter Component", std::string("Sound Emitter Component"), entitycomponent.first, entitySound);
					}
				}
					break;
			}
		}
	}
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

void EntityWindow::DragEntityCheckBox() {

	bool lock = imgui_->GetLockBool();
	ImGui::Checkbox("Drag Entity", &lock);
	ImGui::SameLine(); imgui_->ImguiHelp("Check this box\nto Drag Entities around");
	imgui_->SetLockBool(lock);

}

void EntityWindow::SelectEntityComponent() {

	if ((imgui_->GetEntity() && imgui_->GetEntity()->GetID() || (imgui_->GetEntity() && !imgui_->GetEntity()->GetID() && imgui_->b_editcomp))) {

		std::pair<Entity*, std::vector<ComponentTypes>> entity = GetEntityComponents(imgui_->GetEntity());

		CheckComponentType(entity);
	}
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