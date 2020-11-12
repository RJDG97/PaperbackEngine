#include "ImguiWindows/EntityPathWindow.h"
#include "Engine/Core.h"

void EntityPathWindow::Init() {
    imgui_ = &*CORE->GetSystem<ImguiSystem>();
    entities_ = &*CORE->GetManager<EntityManager>();
	factory_ = &*CORE->GetSystem <EntityFactory>();
}

void EntityPathWindow::Update() {
	std::string path, name;
	size_t counter = 0;
	if (imgui_->b_editpath) {
		ImGui::Begin("Add Archetype to Scene", &imgui_->b_editpath);
		Level* editor = factory_->GetLevel("Editor");
		if (!editor->entity_paths_.empty()) {
			for (Level::EntityPathsIt it = editor->entity_paths_.begin(); it != editor->entity_paths_.end(); ++it) {
				++counter;
				if (ImGui::TreeNodeEx((void*)(size_t)counter, 0, it->first.c_str())) {
					ImGui::Text(it->second.c_str());

					if (ImGui::Button(ICON_FA_PENCIL" Update Current Path")) {

						path = imgui_->OpenSaveDialog("(*.json) Scenes/Archetypes\0*.json\0", 1);

						if (!path.empty())
							it->second = imgui_->EditString(path);
					}

					if (ImGui::Button(ICON_FA_TRASH_O " Delete Current Path")) {

						if (it == editor->entity_paths_.begin())
							it = editor->entity_paths_.erase(it);
						else
						{
							it = editor->entity_paths_.erase(it);
							--it;
						}
					}

					ImGui::TreePop();
					break;
				}

			}

			if (ImGui::Button(ICON_FA_TIMES_CIRCLE " Clear All Paths"))
				editor->entity_paths_.clear();
		}

		ImGui::Separator();

		if (entities_) {
			if (ImGui::BeginCombo("##Archetypes", "Archetypes")) {
				for (EntityManager::EntityArchetypeMapTypeIt entityIT = entities_->GetArchetypes().begin(); entityIT != entities_->GetArchetypes().end(); ++entityIT) {
					name = entityIT->first;
					if (ImGui::Selectable(entityIT->first.c_str())) {
						path = imgui_->OpenSaveDialog("(*.json) Scenes/Archetypes\0*.json\0", 1);

						if (!path.empty()) {

							size_t pos = path.find("Resources");

							std::string file = path.substr(pos);

							editor->AddNewEntityPath(name, file);
						}

					}
				}
				ImGui::EndCombo();
			}
		}
		ImGui::End();
	}

}