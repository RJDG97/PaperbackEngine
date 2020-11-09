#include "ImguiWindows/ArchetypeWindow.h"
#include "Systems/FrameRateController.h"
#include "Entity/Entity.h"
#include "Engine/Core.h"

void ArchetypeWindow::Init() {

	entities_ = &*CORE->GetManager<EntityManager>();
	imgui_ = &*CORE->GetSystem<ImguiSystem>();
	comp_mgr_ = &*CORE->GetManager<ComponentManager>();
	b_editcomp = false;
}

void ArchetypeWindow::Update() {
	
	if (imgui_->b_archetypewin) {

		ImGui::Begin("Archetypes", &imgui_->b_archetypewin);

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
				ImGui::PushItemWidth(250.0f);

				if (ImGui::InputTextWithHint("##name", "Enter name & press Enter", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {

					entityName = buffer;
					AddArchetype(entityName);
				}
				ImGui::PopItemWidth();
			}
		}

		ImGui::Separator();

		if (b_editcomp) {

			ImGui::Text("Available Components:");

			if (imgui_->GetEntity()) {

				ImGui::PushItemWidth(250.0f);

				AddComponent();

				ImGui::PopItemWidth();
			}
		}
		ImGui::End();
	}

	if (imgui_->b_display) {

		
		ImGui::Begin("System Performance", &imgui_->b_display, ImGuiDockNodeFlags_AutoHideTabBar);
		float total_time_ = 0.0f;
		for (std::map<std::string, float>::iterator it = PE_FrameRate.GetSystemPerformance().begin(); it != PE_FrameRate.GetSystemPerformance().end(); ++it) {
			total_time_ += it->second;
		}

		for (std::map<std::string, float>::iterator it = PE_FrameRate.GetSystemPerformance().begin(); it != PE_FrameRate.GetSystemPerformance().end(); ++it) {
			std::string output = it->first + ": " + std::to_string(it->second / total_time_ * 100) + "%";
			ImGui::Text(output.c_str());
		}
		ImGui::End();
	}
}

void ArchetypeWindow::AvaliableArchetypes() {
	bool opened = false;

	if (entities_) {

		for (EntityManager::EntityArchetypeMapTypeIt entityIT = entities_->GetArchetypes().begin(); entityIT != entities_->GetArchetypes().end(); ++entityIT) {

			ImGuiTreeNodeFlags flags = ((imgui_->GetEntity() == entityIT->second) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

			opened = (ImGui::TreeNodeEx((void*)(size_t)entityIT->second, flags, entityIT->first.c_str()));

			if (ImGui::IsItemClicked())
				imgui_->SetEntity(entityIT->second); // store the selected Entity to find its components

			if (opened) {
				if (ImGui::Button("Add Entity"))
					entities_->CloneArchetype(entityIT->first);

				if (ImGui::Button("Delete Archetype")) {

					imgui_->SetEntity(entityIT->second);

					ImGui::OpenPopup("Delete Confirmation");
				}

				imgui_->DeletePopUp("Delete Confirmation", entityIT->first);

				ImGui::Checkbox("Add / Edit Components", &b_editcomp);

				imgui_->SetEntity(entityIT->second);

				ImGui::TreePop();
			}
		}
	}
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

	if (ImGui::BeginCombo("##components", "Available Components")) {

		for (ComponentManager::ComponentMapTypeIt it = comp_mgr_->GetComponentList().begin(); it != comp_mgr_->GetComponentList().end(); ++it) {

			ComponentTypes component = StringToComponentType(it->first.c_str());

			if (!imgui_->GetEntity()->HasComponent(component)) {

				if (ImGui::Selectable(it->first.c_str())) {

					std::shared_ptr<Component> comp;
					IComponentCreator* creator = comp_mgr_->GetComponentCreator(it->first.c_str());
					comp = creator->Create();
					imgui_->GetEntity()->AddComponent(component, comp);
					imgui_->GetEntity()->InitArchetype();
				}
			}
		}

		ImGui::EndCombo();
	}


}
