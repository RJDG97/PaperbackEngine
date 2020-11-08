#include "ImguiWindows/ArchetypeWindow.h"
#include "Engine/Core.h"

void ArchetypeWindow::Init() {

	entities_ = &*CORE->GetManager<EntityManager>();
	imgui_ = &*CORE->GetSystem<ImguiSystem>();
	comp_mgr_ = &*CORE->GetManager<ComponentManager>();

	b_addentity = false;
	b_delete = false;
}

void ArchetypeWindow::Update() {
	
	ImGui::Begin("Archetypes");

	if (ImGui::CollapsingHeader("Existing Archetypes")) {
		AvaliableArchetypes();

		ImGui::Separator();

		if (ImGui::Button("Save List"))
			imgui_->SaveArchetype();

		ImGui::SameLine();

		if (ImGui::Button("Load List"))
			imgui_->LoadArchetype();

	}
	
	if (ImGui::CollapsingHeader("Create New Archetypes")) {
		if (entities_) {
			std::string entityName;
			
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), entityName.c_str());
			
			ImGui::Text("Archetype Name:");
			ImGui::PushItemWidth(200.0f);

			if (ImGui::InputTextWithHint("##name", "Enter name & press Enter", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) { 

				entityName = buffer;
				AddArchetype(entityName);
			}
			ImGui::PopItemWidth();

			// function for all of these as well
			if (imgui_->GetEntity()) {

				//for (ComponentManager::ComponentMapTypeIt it = comp_mgr_->GetComponentList().begin(); it != comp_mgr_->GetComponentList().end(); ++it)
				//{
				//	ImGui::Button(it->first.c_str());
				//}
				//add for loop (put this chunk into a fn taking in ComponentTypes id + char* component Name)
				if (!imgui_->GetEntity()->HasComponent(ComponentTypes::TRANSFORM)) {

					if (ImGui::Button("add transform")) {

						std::shared_ptr<Component> component;
						IComponentCreator* creator = comp_mgr_->GetComponentCreator("Transform");
						component = creator->Create();

						imgui_->GetEntity()->AddComponent(ComponentTypes::TRANSFORM, component);
						imgui_->GetEntity()->InitArchetype();
					}
				}
			}
		}
	}
	
	ImGui::End();
}

void ArchetypeWindow::AvaliableArchetypes() {

	if (entities_) {

		for (EntityManager::EntityArchetypeMapTypeIt entityIT = entities_->GetArchetypes().begin(); entityIT != entities_->GetArchetypes().end(); ++entityIT) {
			if (ImGui::TreeNode(entityIT->first.c_str())) {

				if (ImGui::Button("Add Entity"))
					entities_->CloneArchetype(entityIT->first);

				if (ImGui::Button("Delete Archetype")) {

					imgui_->SetEntity(entityIT->second);

					ImGui::OpenPopup("Delete Confirmation");
				}

				imgui_->DeletePopUp("Delete Confirmation", entityIT->first);

				if (ImGui::Button("Add/Edit Components"))
					imgui_->SetEntity(entityIT->second);

				ImGui::TreePop();
			}
		}
	}
}

void ArchetypeWindow::AddComponents(ComponentTypes type, const char* typeName, const char* buttonLabel){
	std::shared_ptr<Component> component;
	IComponentCreator* creator = comp_mgr_->GetComponentCreator(typeName);

	component = creator->Create();

	imgui_->GetEntity()->AddComponent(type, component);
	imgui_->GetEntity()->InitArchetype();
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
