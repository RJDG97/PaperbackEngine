#include "ImguiWindows\AssetWindow.h"


void AssetWindow::Init() {
	imgui_ = &*CORE->GetSystem<ImguiSystem>();
}

void AssetWindow::Update() {
	if (imgui_->b_asset) {
		ImGui::Begin("Asset Browser", &imgui_->b_asset);

		FileDirectoryCheck("Resources");

		ImGui::End();
	}
}

void AssetWindow::FileDirectoryCheck(fs::path directory) {
	size_t counter = 0;
	ImGui::PushFont(imgui_->img_font_);
	for (auto& directory : fs::directory_iterator(directory)) {
		++counter;
		if (fs::is_directory(directory)) {
			//auto& string;
			if (ImGui::TreeNodeEx((void*)(size_t)counter, 0, (ICON_FA_FOLDER + std::string(" ") + directory.path().generic_string()).c_str())) {

				if (fs::is_directory(directory))
					FileDirectoryCheck(directory);
				else if (fs::is_regular_file(directory))
					ImGui::Text(directory.path().generic_string().c_str());

				ImGui::TreePop();
			}
		}
		else if (fs::is_regular_file(directory)) {
			ImGui::Text(directory.path().generic_string().c_str());
		}

	}
	ImGui::PopFont();

}





