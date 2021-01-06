/**********************************************************************************
*\file         EntityPathWindow.cpp
*\brief        Contains definition of functions and variables used for
*			   the EntityPathWindow to set the saving json path for each
*			   archetype type

*\author	   Ee Ling Adele, Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "ImguiWindows/EntityPathWindow.h"
#include "Engine/Core.h"

void EntityPathWindow::Init() {
    imgui_ = &*CORE->GetSystem<ImguiSystem>();
    entities_ = &*CORE->GetManager<EntityManager>();
	factory_ = &*CORE->GetSystem <EntityFactory>();

	b_wrong_loc = false;
}

void EntityPathWindow::Update() {

	if (imgui_->b_edit_path) {

		ImGui::Begin("Set Entity Save Path", &imgui_->b_edit_path);

		Level* editor = factory_->GetLevel("Editor");

		ManagePaths(editor);

		ImGui::Separator();

		AddPaths(editor);

		ImGui::End();
	}

	if (b_wrong_loc)
		ImGui::OpenPopup("Wrong Location Set");

	imgui_->PopUpMessage("Wrong Location Set","Wrong Location Set.\nSet the Path to be within the 'Resources' folder!");
	b_wrong_loc = false;
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

							if (imgui_->CheckString(newPath, "json"))
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

					ImGui::SameLine(0, 3);

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

		if (ImGui::BeginCombo("##Archetypes", "Archetypes Available")) {

			for (EntityManager::EntityArchetypeMapTypeIt entityIT = entities_->GetArchetypes().begin(); entityIT != entities_->GetArchetypes().end(); ++entityIT) {
				
				auto it = editor->entity_paths_.find(entityIT->first);

				if (it == editor->entity_paths_.end()) { //only set each archetype path 1 time

					if (ImGui::Selectable(entityIT->first.c_str())) {
						path = imgui_->OpenSaveDialog("(*.json) Scenes/Archetypes\0*.json\0", 1);

						if (!path.empty()) {
							if (imgui_->CheckString(path, "Resources"))
								editor->AddNewEntityPath(entityIT->first, imgui_->EditString(path));
							else
								b_wrong_loc = true;
						}
					}
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine(0, 3); imgui_->ImguiHelp("Select an archetype\nto set the save filepath");
	}
}
