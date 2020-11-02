#include "ImguiWindows/EntityWindow.h"
#include "Engine/Core.h"

void EntityWindow::Init() {

	/*viewport_size_ = ImVec2{ 0.0f, 0.0f };
	editor_viewport_ = &*CORE->GetSystem<GraphicsSystem>();*/
	entities_ = &*CORE->GetManager<EntityManager>();
	b_addentity = false;
	imgui_ = &*CORE->GetSystem<ImguiSystem>();
	comp_mgr_ = &*CORE->GetManager<ComponentManager>();
}

void EntityWindow::Update() {
	//char* entityTag = {};
	bool add = false;
	/*ImGui::Begin("viewport");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	viewport_size_ = { viewportPanelSize.x, viewportPanelSize.y };
	uint64_t textureID = editor_viewport_->GetFramebuffer();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{viewport_size_.x,viewport_size_.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();*/

	ImGui::Begin("Entity Management");

	if (ImGui::Button("Add Entities")) {

		b_addentity = !b_addentity;
	}


	if (b_addentity) {
		if (ImGui::CollapsingHeader("Existing Archetypes")) {
			if (entities_) {
				for (EntityManager::EntityArchetypeMapTypeIt entityIT = entities_->GetArchetypes().begin(); entityIT != entities_->GetArchetypes().end(); ++entityIT) {
					if (ImGui::Button(entityIT->first.c_str())) {

						entities_->CloneArchetype(entityIT->first);
					}
				}
			}
		}
		if (ImGui::CollapsingHeader("New Archetypes")) {
			if (entities_) {

				std::string entityName;

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), entityName.c_str());
				ImGui::Text("Archetype Name:");
				if (ImGui::InputText("##name", buffer, sizeof(buffer)))
				{
					entityName = std::string(buffer);
				}


				if (ImGui::Button("Add New Archetype"))
				{
					add = !add;
				}

				imgui_->SetEntity(entities_->CreateNewArchetype(entityName));

				if (add)
				{

					IComponentCreator* creator = comp_mgr_->GetComponentCreator("Transform");
					std::shared_ptr<Component> component = creator->Create();
					imgui_->GetEntity()->AddComponent(ComponentTypes::TRANSFORM, component);

				}
			}
		}
	}
	ImGui::End();
}