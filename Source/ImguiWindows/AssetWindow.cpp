#include "ImguiWindows\AssetWindow.h"

void AssetWindow::Init() {
	imgui_ = &*CORE->GetSystem<ImguiSystem>();
	path_selection_ = "";
}

void AssetWindow::Update() {
	float windowW = ImGui::GetContentRegionAvailWidth(), windowH = ImGui::GetContentRegionAvail().y;

	if (imgui_->b_asset) {
		ImGui::PushFont(imgui_->img_font_);

		ImGui::Begin("Asset Browser", &imgui_->b_asset);
		// Print out all directories
		ImGui::BeginChild("File Directories", {windowW /7, windowH}, true);
		ImGui::Text("Directories:");
		FileDirectoryCheck("Resources");
		ImGui::EndChild();

		ImGui::SameLine();

		//Print out all files in the directory
		ImGui::BeginChild("Files", { windowW / 3, windowH }, true, ImGuiWindowFlags_MenuBar);

		ImGui::BeginMenuBar();
		ImGui::Text((ICON_FA_FOLDER_OPEN + path_selection_.generic_string()).c_str());
		ImGui::SameLine(0, 3); ImGui::Text("Files:");
		ImGui::EndMenuBar();

		PrintFileType();
		ImGui::EndChild();

		ImGui::End();
		ImGui::PopFont();
	}
}

void AssetWindow::FileDirectoryCheck(fs::path filedirectory) {
	size_t counter = 0;
	bool opened = false;
	
	for (auto& directory : fs::directory_iterator(filedirectory)) {
		++counter;
		if (fs::is_directory(directory)) {

			ImGuiTreeNodeFlags flags = ((path_selection_ == directory) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

			std::string dir = DirectoryName(directory);

			opened = ImGui::TreeNodeEx((void*)(size_t)counter, flags, FileString(ICON_FA_FOLDER, dir).c_str());
				
			if (ImGui::IsItemClicked())
				path_selection_ = directory;

			if (opened) {
				FileDirectoryCheck(directory);
				ImGui::TreePop();
			}
		}
	}
}

void AssetWindow::PrintFileType() {

	static ImGuiTextFilter filter;
	filter.Draw();
	std::string fileName = {};
	if (!path_selection_.empty()) {
		for (auto& file : fs::directory_iterator(path_selection_)) {

			if (fs::is_regular_file(file)) {

				fileName = file.path().filename().generic_string();

				if (filter.PassFilter(fileName.c_str())) {

					if (file.path().extension() == ".png")
						ImGui::Text(FileString(ICON_FA_FILE_IMAGE_O, fileName).c_str());
					if (file.path().extension() == ".json")
						ImGui::Text(FileString(ICON_FA_FILE_CODE_O, fileName).c_str());
					if (file.path().extension() == ".mp3" || file.path().extension() == ".wav")
						ImGui::Text(FileString(ICON_FA_FILE_AUDIO_O, fileName).c_str());
					if (file.path().extension() == ".txt")
						ImGui::Text(FileString(ICON_FA_FILE_TEXT_O, fileName).c_str());
					if (file.path().extension() == ".ttf")
						ImGui::Text(FileString(ICON_FA_FONT, fileName).c_str());
				}
					
			}

			if (fs::is_directory(file)) {
				if (ImGui::Selectable(FileString(ICON_FA_FOLDER, DirectoryName(file)).c_str()))
					path_selection_ = file;
			}

		}
	}


}

std::string AssetWindow::FileString(std::string icon, std::string file_name) {

	return (icon + std::string(" ") + file_name);
}

std::string AssetWindow::DirectoryName(fs::directory_entry directory) {

	std::string tempPath = directory.path().generic_string();
	return tempPath.substr(tempPath.find_last_of("/") + 1);

}





