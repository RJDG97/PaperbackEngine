/**********************************************************************************
*\file         ArchetypeWindow.cpp
*\brief        Contains definition of functions and variables used for
*			   the ArchetypeWindow for the management of archetypes
*			   (Creating, deleting, amending existing archetypes)

*\author	   Ee Ling Adele, Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "ImguiWindows/ArchetypeWindow.h"
#include "Entity/Entity.h"
#include "Engine/Core.h"

void ArchetypeWindow::Init() {

	entities_ = &*CORE->GetManager<EntityManager>();
	imgui_ = &*CORE->GetSystem<ImguiSystem>();
	comp_mgr_ = &*CORE->GetManager<ComponentManager>();
	factory_ = &*CORE->GetSystem <EntityFactory>();

	b_nocam = false;
	b_missingcomp = false;
	b_noscale = b_notrans = false;
	b_new_archetype = false;

	archetype_name = {};
}

void ArchetypeWindow::Update() {
	
	if (imgui_->b_archetypewin) {

		ImGui::Begin("Archetypes", &imgui_->b_archetypewin, ImGuiWindowFlags_MenuBar);

		ArchetypeMenuBar();

		if (ImGui::CollapsingHeader("Archetypes List", ImGuiTreeNodeFlags_DefaultOpen))
			AvaliableArchetypes();

		AddComponent();

		ImGui::End();

		NoCameraPopUp();
		AddNewArchetypePopup();
	}
}

void ArchetypeWindow::AvaliableArchetypes() {
	bool opened = false;

	static ImGuiTextFilter filter;
	filter.Draw(ICON_FA_FILTER, 150.0f);

	if (entities_) {

		for (EntityManager::EntityArchetypeMapTypeIt entityIT = entities_->GetArchetypes().begin(); entityIT != entities_->GetArchetypes().end(); ++entityIT) {

			ImGuiTreeNodeFlags flags = ((imgui_->GetEntity() == entityIT->second) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

			if (filter.PassFilter(entityIT->first.c_str())) {
				opened = (ImGui::TreeNodeEx((void*)(size_t)entityIT->second, flags, entityIT->first.c_str()));

				if (ImGui::IsItemClicked())
					imgui_->SetEntity(entityIT->second); // store the selected Entity to find its components

				if (opened) {

					if (ImGui::Button(ICON_FA_PLUS_SQUARE " Spawn Entity")) {

						if (imgui_->GetExistingSceneCamera()) {

							if (entityIT->second->GetComponent(ComponentTypes::TEXTURERENDERER) || entityIT->second->GetComponent(ComponentTypes::ANIMATIONRENDERER)) {

								if (entityIT->second->GetComponent(ComponentTypes::TRANSFORM) && entityIT->second->GetComponent(ComponentTypes::SCALE)) {

									entities_->CloneArchetype(entityIT->first);
									imgui_->SetEntity(nullptr);

								}
								else
									b_missingcomp = true;
							}
							else {
								entities_->CloneArchetype(entityIT->first);
								imgui_->SetEntity(nullptr);
							}
						}
						else // no camera
						{
							if (entityIT->first == "Camera") {

								entities_->CloneArchetype(entityIT->first);
								imgui_->SetEntity(nullptr);
							}
							else if (entityIT->first != "Camera")
								b_nocam = true;
						}
					}
					if (b_missingcomp) {
						ImGui::OpenPopup("Components are Missing!!!");

						imgui_->SetEntity(entityIT->second);
						archetype_name = entityIT->first;

						if (!imgui_->GetEntity()->GetComponent(ComponentTypes::TRANSFORM))
							b_notrans = true;
						if (!imgui_->GetEntity()->GetComponent(ComponentTypes::SCALE))
							b_noscale = true;
					}

					MissingComponentPopUp();

					if (ImGui::Button("Delete Archetype")) {

						imgui_->SetEntity(entityIT->second);

						ImGui::OpenPopup(ICON_FA_TRASH " Delete Confirmation");
					}

					imgui_->DeletePopUp(ICON_FA_TRASH " Delete Confirmation", entityIT->first);

					ImGui::Checkbox("Add/Edit Components", &imgui_->b_editcomp); ImGui::SameLine(0, 3);
					imgui_->ImguiHelp("Untick this whenever you are done");

					ImGui::TreePop();
				}
			}
		}
	}
}

void ArchetypeWindow::ArchetypeMenuBar() {

	if (ImGui::BeginMenuBar()) {
		ImGui::PushFont(imgui_->img_font_);

		if (ImGui::MenuItem(ICON_FA_PLUS_SQUARE))
			b_new_archetype = true;

		imgui_->ImguiHelp("Add New Archetype", 0);

		if (ImGui::MenuItem(ICON_FA_SAVE))
			imgui_->SaveArchetype(imgui_->GetArchetypePath());

		imgui_->ImguiHelp("Save Current List", 0);

		if (ImGui::MenuItem(ICON_FA_SAVE ICON_FA_ELLIPSIS_H)) {

			std::string path = imgui_->OpenSaveDialog("(*.json) Paperback Engine Scene\0*.json\0", 1);
			imgui_->SaveArchetype(path);
		}

		imgui_->ImguiHelp("Save List As...", 0);

		if (ImGui::MenuItem(ICON_FA_FILE_IMPORT))
			imgui_->LoadArchetype();

		imgui_->ImguiHelp("Load Archetype List", 0);

		imgui_->ImguiHelp("Close Archetype Tab before\nmaking other selections");

		ImGui::PopFont();
		ImGui::EndMenuBar();
	}
}

std::vector<std::string> ArchetypeWindow::CheckArchetypeTexture(std::string path)
{
	std::pair<Entity*, std::vector<ComponentTypes>> entity = {};
	std::vector<std::string> archetypes = {};
	if (entities_) {

		for (EntityManager::EntityArchetypeMapTypeIt entityIT = entities_->GetArchetypes().begin(); entityIT != entities_->GetArchetypes().end(); ++entityIT) {

			if (entityIT->second->GetComponent(ComponentTypes::TEXTURERENDERER)) {
				std::shared_ptr<TextureRenderer> entitytexture = std::dynamic_pointer_cast<TextureRenderer>(entityIT->second->GetComponent(ComponentTypes::TEXTURERENDERER));

				if (entitytexture->GetCurrentTextureName() == path)
					archetypes.push_back(entityIT->first);
			}
		}
	}

	return archetypes;
}

void ArchetypeWindow::AddArchetype(std::string archetypeName)
{
	std::stringstream name;
	std::shared_ptr<Component> component;

	name << archetypeName;

	imgui_->SetEntity(entities_->CreateNewArchetype(archetypeName));

	IComponentCreator* creator = comp_mgr_->GetComponentCreator("Name");
	component = creator->Create();

	component->DeSerialize(name);
	imgui_->GetEntity()->AddComponent(ComponentTypes::NAME, component);
	imgui_->GetEntity()->InitArchetype();

	imgui_->SetEntity(nullptr);
}

void ArchetypeWindow::AddComponent() {

	if (imgui_->b_editcomp) {

		ImGui::Begin("Add Components to Archetype");

		if (imgui_->GetEntity() && !imgui_->GetEntity()->GetID()) {

			ImGui::Text("Amending Archetype: "); ImGui::SameLine(0, 7);
			ImGui::TextColored(GOLDENORANGE, archetype_name.c_str());

			if (ImGui::BeginCombo("##components", "Available Components")) {

				for (ComponentManager::ComponentMapTypeIt it = comp_mgr_->GetComponentList().begin(); it != comp_mgr_->GetComponentList().end(); ++it) {

					ComponentTypes componenttype = StringToComponentType(it->first.c_str());

					if (!imgui_->GetEntity()->HasComponent(componenttype)) {

						if (ImGui::Selectable(it->first.c_str()))
							AddSingleComponent(it->first, componenttype);
					}
				}

				ImGui::EndCombo();
			}
		}

		if (ImGui::Button("Close This Panel")) {

			imgui_->b_editcomp = false;
			imgui_->SetEntity({});
			archetype_name = {};
		}

		ImGui::End();
	}
}

void ArchetypeWindow::AddSingleComponent(std::string archetype, ComponentTypes component) {

	std::shared_ptr<Component> comp;
	IComponentCreator* creator = comp_mgr_->GetComponentCreator(archetype.c_str());
	comp = creator->Create();
	imgui_->GetEntity()->AddComponent(component, comp);
	imgui_->GetEntity()->InitArchetype();
}

void ArchetypeWindow::AddNewArchetypePopup() {

	if (b_new_archetype)
		ImGui::OpenPopup("Add Archetype");

	imgui_->SetPopupPosition();

	if (ImGui::BeginPopup("Add Archetype")) {

		if (entities_) {
			std::string entityName;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), entityName.c_str());

			ImGui::Text(ICON_FA_EDIT " New Archetype Name:");
			ImGui::PushItemWidth(250.0f);

			if (ImGui::InputTextWithHint("##name", "Enter name & press Enter", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {

				AddArchetype(std::string(buffer));
				b_new_archetype = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopItemWidth();
		}

		if (ImGui::Button("Close This Panel")) {
			b_new_archetype = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void ArchetypeWindow::NoCameraPopUp() {

	if (b_nocam)
		ImGui::OpenPopup("No Camera");

	imgui_->SetPopupPosition();

	if (ImGui::BeginPopupModal("No Camera", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

		ImGui::Text("There is no Active Camera in the scene");
		ImGui::Text("Do you want to spawn one in?");

		imgui_->CustomImGuiButton(BLUEDEFAULT, BLUEHOVERED, BLUEACTIVE);

		if (ImGui::Button("Yes")) {

			entities_->CloneArchetype("Camera");
			imgui_->SetEntity(nullptr);

			b_nocam = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::PopStyleColor(3);

		ImGui::SameLine(0, 10);

		if (ImGui::Button("No")) {
			b_nocam = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void ArchetypeWindow::MissingComponentPopUp() {

	imgui_->SetPopupPosition();

	if (ImGui::BeginPopupModal("Components are Missing!!!", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

		ImGui::TextColored(REDHOVERED, "Error!!!!!");
		if (b_notrans && b_noscale)
			ImGui::Text("Trying to add an entity with a Texture without Scale & Transform Components");
		else if (b_notrans)
			ImGui::Text("Trying to add an entity with a Texture with no Transform Component");
		else if (b_noscale)
			ImGui::Text("Trying to add an entity with a Texture with no have Scale Component");

		ImGui::Text("Want to add in the missing component? Or Remove the texture component?");

		if (ImGui::Button("Add")) {

			if (b_notrans && b_noscale) {

				AddSingleComponent("Transform", ComponentTypes::TRANSFORM);
				AddSingleComponent("Scale", ComponentTypes::SCALE);

				entities_->CloneArchetype(archetype_name);
				//imgui_->SetEntity(nullptr);
				archetype_name = {};

				b_noscale = false;
				b_notrans = false;
			}
			else if (b_notrans) {

				AddSingleComponent("Transform", ComponentTypes::TRANSFORM);

				entities_->CloneArchetype(archetype_name);
				//imgui_->SetEntity(nullptr);
				archetype_name = {};

				b_notrans = false;
			}
			else if (b_noscale) {

				AddSingleComponent("Scale", ComponentTypes::SCALE);

				entities_->CloneArchetype(archetype_name);
				//imgui_->SetEntity(nullptr);
				archetype_name = {};

				b_noscale = false;
			}

			ImGui::CloseCurrentPopup();
			b_missingcomp = false;
		}

		ImGui::SameLine(0, 10);

		if (ImGui::Button("Cancel")) {

			ImGui::CloseCurrentPopup();
			b_notrans = false;
			b_noscale = false;
			b_missingcomp = false;
			archetype_name = {};
		}

		ImGui::SameLine(0, 10);

		if (ImGui::Button("Remove Texture")) {

			std::shared_ptr<TextureRenderer> entitytexture = std::dynamic_pointer_cast<TextureRenderer>(imgui_->GetEntity()->GetComponent(ComponentTypes::TEXTURERENDERER));
			imgui_->GetEntity()->RemoveComponent(entitytexture);

			b_notrans = false;
			b_noscale = false;
			b_missingcomp = false;
			archetype_name = {};

			ImGui::CloseCurrentPopup();
		}

		imgui_->SetEntity({});
		ImGui::EndPopup();
	}
}
