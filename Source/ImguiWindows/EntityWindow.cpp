#include "ImguiWindows/EntityWindow.h"
#include "Engine/Core.h"

void EntityWindow::Init() {

	entities_ = &*CORE->GetManager<EntityManager>();
	b_addentity = false;
	imgui_ = &*CORE->GetSystem<ImguiSystem>();
	comp_mgr_ = &*CORE->GetManager<ComponentManager>();

	b_delete = false;
}

void EntityWindow::Update() {

	Entity* entity;
	
	ImGui::Begin("Entity Management");

	if (ImGui::Button("Add Entities")) {

		b_addentity = !b_addentity;
	}

	if (b_addentity) {
		if (ImGui::CollapsingHeader("Existing Archetypes")) {


			if (entities_) {
				
				for (EntityManager::EntityArchetypeMapTypeIt entityIT = entities_->GetArchetypes().begin(); entityIT != entities_->GetArchetypes().end(); ++entityIT) {
					if (ImGui::TreeNode(entityIT->first.c_str())) {

						if (ImGui::Button("Add Entity"))
							entities_->CloneArchetype(entityIT->first);

						if (ImGui::Button("Delete Archetype")){
							
							imgui_->SetEntity(entityIT->second);
							//entity = entityIT->second;
							//selectionName = entityIT->first;

							ImGui::OpenPopup("Delete Confirmation");
						}

						ImVec2 centre = ImGui::GetMainViewport()->GetCenter();

						ImGui::SetNextWindowPos(centre, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

						if (ImGui::BeginPopupModal("Delete Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize))
						{
							ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f , 1.0f }, "Delete Entity?\n This operation cannot be undone");
							ImGui::Separator();

							if (ImGui::Button("Ok"))
							{
								entities_->DeleteArchetype(entityIT->second);
								imgui_->SetEntity((nullptr));

								ImGui::CloseCurrentPopup();
								
								
							}
							//ImGui::SameLine();
							if (ImGui::Button("Cancel"))
							{
								ImGui::CloseCurrentPopup();
								
							}

							ImGui::EndPopup();

						}

						


						
						ImGui::TreePop();
					}
				}
			}
		}
		
		if (ImGui::CollapsingHeader("New Archetypes")) {
			if (entities_) {

				std::string entityName;
				std::stringstream name;
				std::shared_ptr<Component> component;
				
				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), entityName.c_str());
				
				ImGui::Text("Archetype Name:");
				
				bool add = (ImGui::InputText("##name", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue));
				//ComponentArr arr = imgui_->GetEntity()->GetComponentArr();

				if (add)
				{
					entityName = buffer;
					name << entityName;

					imgui_->SetEntity(entities_->CreateNewArchetype(entityName));
					
					IComponentCreator* creator = comp_mgr_->GetComponentCreator("Name");
					component = creator->Create();

					component->DeSerialize(name);
					imgui_->GetEntity()->AddComponent(ComponentTypes::NAME, component);
					imgui_->GetEntity()->InitArchetype();
				}


				if (ImGui::Button("add transform"))
				{
					if (imgui_->GetEntity()) {
						IComponentCreator* creator = comp_mgr_->GetComponentCreator("Transform");
						component = creator->Create();

						//if ()
						
						imgui_->GetEntity()->AddComponent(ComponentTypes::TRANSFORM, component);
						imgui_->GetEntity()->InitArchetype();
					}
				}
			}
		}
	}
	ImGui::End();
}

void EntityWindow::AddComponents(std::string component)
{
	
}
