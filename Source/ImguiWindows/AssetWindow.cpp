#include "ImguiWindows\AssetWindow.h"


void AssetWindow::Init() {
	imgui_ = &*CORE->GetSystem<ImguiSystem>();
	path_selection_ = "";
}

void AssetWindow::Update() {

	if (imgui_->b_asset) {
		ImGui::Begin("Asset Browser", &imgui_->b_asset);
		ImGui::BeginChild("HA", {ImGui::GetContentRegionAvailWidth()/3, ImGui::GetContentRegionAvail().y}, true);
		FileDirectoryCheck("Resources");
		ImGui::EndChild();
		ImGui::SameLine();
		ImGui::BeginChild("tes", { ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvail().y }, true);
		if (!path_selection_.empty()) {
			for (auto& ref : fs::directory_iterator(path_selection_))
			{
				if (fs::is_regular_file(ref))
					ImGui::Text(ref.path().filename().generic_string().c_str());

			}
		}
		ImGui::EndChild();
		ImGui::End();
	}
}

void AssetWindow::FileDirectoryCheck(fs::path filedirectory) {
	size_t counter = 0;
	bool opened = false;

	ImGui::PushFont(imgui_->img_font_);
		for (auto& directory : fs::directory_iterator(filedirectory)) {
			++counter;
			if (fs::is_directory(directory)) {

				ImGuiTreeNodeFlags flags = ((path_selection_ == directory) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

				opened = ImGui::TreeNodeEx((void*)(size_t)counter, flags, (ICON_FA_FOLDER + std::string(" ") + directory.path().generic_string()).c_str());
					
				if (ImGui::IsItemClicked()) {
					path_selection_ = directory;
				}

				if (opened) {
					FileDirectoryCheck(directory);
					ImGui::TreePop();
				}

				
			}

		}

	ImGui::PopFont();

}





