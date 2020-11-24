#include "ImguiWindows/ArchetypeWindow.h"
#include "Entity/Entity.h"
#include "Engine/Core.h"

void ArchetypeWindow::Init() {

	entities_ = &*CORE->GetManager<EntityManager>();
	imgui_ = &*CORE->GetSystem<ImguiSystem>();
	comp_mgr_ = &*CORE->GetManager<ComponentManager>();
	factory_ = &*CORE->GetSystem <EntityFactory>();

	b_nocam = false;
}

void ArchetypeWindow::Update() {
	
	if (imgui_->b_archetypewin) {

		ImGui::Begin("Archetypes", &imgui_->b_archetypewin);

		if (ImGui::CollapsingHeader("Available Archetypes")) {

			AvaliableArchetypes();

			ImGui::Separator();
			ImGui::PushFont(imgui_->img_font_);
			if (ImGui::Button(ICON_FA_SAVE " Save List"))
				imgui_->SaveArchetype();

			ImGui::SameLine();

			if (ImGui::Button(ICON_FA_FILE_IMPORT " Load List"))
				imgui_->LoadArchetype();
			ImGui::PopFont();

			ImGui::SameLine(0, 4);
			imgui_->ImguiHelp("Close Archetype Node before \nmaking other selections");
		}

		if (ImGui::CollapsingHeader("Create New Archetypes")) {

			if (entities_) {
				std::string entityName;

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), entityName.c_str());

				ImGui::Text(ICON_FA_EDIT " New Archetype Name:");
				ImGui::PushItemWidth(250.0f);

				if (ImGui::InputTextWithHint("##name", "Enter name & press Enter", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
					AddArchetype(std::string(buffer));

				ImGui::PopItemWidth();
			}
		}

		ImGui::Separator();

		if (imgui_->b_editcomp) {

			if (imgui_->GetEntity() && !imgui_->GetEntity()->GetID()) {

				ImGui::PushItemWidth(250.0f);

				AddComponent();

				ImGui::PopItemWidth();
			}
		}

		ImGui::End();
	}

	if (b_nocam)
		ImGui::OpenPopup("No Camera");

	ImVec2 centre = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(centre, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	NoCameraPopUp();
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

				if (ImGui::Button(ICON_FA_PLUS_SQUARE " Spawn Entity")) {
					if (imgui_->GetExistingSceneCamera()) {
						entities_->CloneArchetype(entityIT->first);
						imgui_->SetEntity(nullptr);
					}
					else
					{
						if (entityIT->first == "Camera") {

							entities_->CloneArchetype(entityIT->first);
							imgui_->SetEntity(nullptr);
						}
						else if (entityIT->first != "Camera")
							b_nocam = true;

					}
				}

				if (ImGui::Button("Delete Archetype")) {

					imgui_->SetEntity(entityIT->second);

					ImGui::OpenPopup(ICON_FA_TRASH " Delete Confirmation");
				}

				imgui_->DeletePopUp(ICON_FA_TRASH " Delete Confirmation", entityIT->first);

				ImGui::Checkbox("Add/Edit Components", &imgui_->b_editcomp); ImGui::SameLine(0, 3);
				imgui_->ImguiHelp("Untick this whenever you are done");

				imgui_->SetEntity(entityIT->second);

				ImGui::TreePop();
			}


		}
	}
}

void ArchetypeWindow::NoCameraPopUp() {

	if (ImGui::BeginPopup("No Camera")) {

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

		ImGui::SameLine(0, 4);

		if (ImGui::Button("No")) {
			b_nocam = false;
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
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

			ComponentTypes componenttype = StringToComponentType(it->first.c_str());

			if (!imgui_->GetEntity()->HasComponent(componenttype)) {

				if (ImGui::Selectable(it->first.c_str())) {

					std::shared_ptr<Component> comp;
					IComponentCreator* creator = comp_mgr_->GetComponentCreator(it->first.c_str());
					comp = creator->Create();
					imgui_->GetEntity()->AddComponent(componenttype, comp);
					imgui_->GetEntity()->InitArchetype();
				}
			}
		}

		ImGui::EndCombo();
	}
}
