#include "ImguiWindows/EntityPathWindow.h"
#include "Engine/Core.h"

void EntityPathWindow::Init() {
    imgui_ = &*CORE->GetSystem<ImguiSystem>();
    entities_ = &*CORE->GetManager<EntityManager>();
	factory_ = &*CORE->GetSystem <EntityFactory>();
}

void EntityPathWindow::Update() {

	if (imgui_->b_editpath) {

		ImGui::Begin("Set Entity Save Path", &imgui_->b_editpath);

		Level* editor = factory_->GetLevel("Editor");

		ManagePaths(editor);

		ImGui::Separator();

		AddPaths(editor);

		ImGui::End();
	}

}

void EntityPathWindow::ManagePaths(Level* editor) {
	std::string path, newPath;
	size_t counter = 0;
	if (!editor->entity_paths_.empty()) {

		ImGui::Text("Current Set Path(s): ");
		for (Level::EntityPathsIt it = editor->entity_paths_.begin(); it != editor->entity_paths_.end(); ++it) {
			++counter;

			if (ImGui::TreeNodeEx((void*)(size_t)counter, 0, it->first.c_str())) {

				ImGui::Text(it->second.c_str());

				if (ImGui::BeginDragDropTarget()) {

					if (const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("UPDATED_PATH")) {

						if (payLoad->DataSize == sizeof(std::string)) {

							newPath = *((std::string*)payLoad->Data);

							if (newPath.find(".json") != newPath.npos)

								it->second = newPath;
						}
					}

					ImGui::EndDragDropTarget();
				}

				if (ImGui::Button(ICON_FA_EDIT" Update")) {

					path = imgui_->OpenSaveDialog("(*.json) Scene Entities\0*.json\0", 1);

					if (!path.empty())
						it->second = imgui_->EditString(path);
				}

				if (ImGui::Button(ICON_FA_TRASH" Delete"))
					ImGui::OpenPopup("delete path check");

				if (ImGui::BeginPopup("delete path check")) {
					ImGui::Text(("Deleting: " + it->first + " path: \n" + it->second).c_str());
					if (ImGui::Button("OK")) {

						if (it == editor->entity_paths_.begin())
							it = editor->entity_paths_.erase(it);
						else {

							it = editor->entity_paths_.erase(it);
							--it;
						}
						ImGui::CloseCurrentPopup();
					}

					if (ImGui::Button("Cancel"))
						ImGui::CloseCurrentPopup();

					ImGui::EndPopup();
				}


				ImGui::TreePop();
				break;
			}
		}

		if (ImGui::Button(ICON_FA_TIMES_CIRCLE " Clear All"))
			editor->entity_paths_.clear();
	}
	else
		ImGui::TextColored(ImVec4{1.0f, 0.0f, 0.0f, 1.0f}, "No Path Set");

}

void EntityPathWindow::AddPaths(Level* editor) {
	std::string path;
	if (entities_) {

		if (ImGui::BeginCombo("##Archetypes", "Archetypes")) {

			for (EntityManager::EntityArchetypeMapTypeIt entityIT = entities_->GetArchetypes().begin(); entityIT != entities_->GetArchetypes().end(); ++entityIT) {
				
				if (ImGui::Selectable(entityIT->first.c_str())) {
					path = imgui_->OpenSaveDialog("(*.json) Scenes/Archetypes\0*.json\0", 1);

					if (!path.empty()) 
						editor->AddNewEntityPath(entityIT->first, imgui_->EditString(path));
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine(0, 3); imgui_->ImguiHelp("Select an archetype\nto set the save filepath");
	}

}
