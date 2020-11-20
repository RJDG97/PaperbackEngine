#include "ImguiWindows\AssetWindow.h"

void AssetWindow::Init() {
	imgui_ = &*CORE->GetSystem<ImguiSystem>();
	path_selection_ = "";
	b_create = false;
	b_makefolder = false;
}

void AssetWindow::Update() {
	float windowW = ImGui::GetContentRegionAvailWidth(), windowH = ImGui::GetContentRegionAvail().y;
	bool create_dir = false;
	if (imgui_->b_asset) {
		ImGui::PushFont(imgui_->img_font_);

		ImGui::Begin("Asset Browser", &imgui_->b_asset);
		// Print out all directories
		ImGui::BeginChild("File Directories", {windowW /7, windowH}, true);
		ImGui::Text("Folders/Directories:");
		FileDirectoryCheck("Resources");
		ImGui::EndChild();

		ImGui::SameLine();

		//Print out all files in the directory
		ImGui::BeginChild("Files", { windowW /4 , windowH }, true, ImGuiWindowFlags_MenuBar);

		ImGui::BeginMenuBar();
		
		if (ImGui::BeginMenu("File")) {
			
			if (ImGui::MenuItem("New File")) {

				imgui_->OpenSaveDialog("(*.*) All Files\0* *.*\0", 1);
			}
			if (ImGui::MenuItem("New Folder")) {
				b_create = true;
				b_makefolder = true;
			}
			
			if (ImGui::MenuItem("Copy File to directory...") && !path_selection_.empty()) {

				std::string source_dir = imgui_->OpenSaveDialog("(*.*) All Files\0* *.*\0", 0);

				if (!source_dir.empty()) {

					std::string destination_dir = path_selection_.generic_string();
					fs::copy(source_dir, destination_dir, fs::copy_options::skip_existing);

				}
			}

			ImGui::EndMenu();
		}

		ImGui::Text(FileString(ICON_FA_FOLDER_OPEN, path_selection_.generic_string()).c_str());
		ImGui::SameLine(0, 3); ImGui::Text("Files:");

		ImGui::EndMenuBar();

		PrintFileType();

		ImGui::EndChild();

		ImGui::End();
		ImGui::PopFont();
	}

	if (b_create) {

		ImGui::Begin("add directory", &b_create);

		if (b_makefolder) {

			std::string folderName;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), folderName.c_str());

			if (ImGui::InputText("##newfile", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {

				if (!path_selection_.empty())

					fs::create_directory(path_selection_ / buffer);
				else 
					fs::create_directory((fs::path)"Resources" / buffer);

				b_makefolder = false;
				b_create = false;
			}
			
		}
		
		ImGui::End();
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

	size_t counter = 0;
	std::string fileName = {};
	bool opened = false;

	static ImGuiTextFilter filter;
	filter.Draw(ICON_FA_FILTER, 200.0f);

	if (!path_selection_.empty()) {
		for (auto& file : fs::directory_iterator(path_selection_)) {
			++counter;
			if (fs::is_regular_file(file)) {

				fileName = file.path().filename().generic_string();

				if (filter.PassFilter(fileName.c_str())) {

					if (file.path().extension() == ".png")
						ImGui::Text(FileString(ICON_FA_FILE_IMAGE, fileName).c_str());
					else if (file.path().extension() == ".json")
						ImGui::Text(FileString(ICON_FA_FILE_CODE, fileName).c_str());
					else if (file.path().extension() == ".mp3" || file.path().extension() == ".wav")
						ImGui::Text(FileString(ICON_FA_FILE_AUDIO, fileName).c_str());
					else if (file.path().extension() == ".txt")
						ImGui::Text(FileString(ICON_FA_FILE_WORD, fileName).c_str());
					else if (file.path().extension() == ".ttf")
						ImGui::Text(FileString(ICON_FA_FONT, fileName).c_str());
					else
						ImGui::Text(FileString(ICON_FA_FILE, fileName).c_str()); // will update when needed
				}
					
			}

			if (fs::is_directory(file)) {

				if (ImGui::Selectable(FileString(ICON_FA_FOLDER, DirectoryName(file)).c_str()))
					path_selection_ = file;
			}

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID | ImGuiDragDropFlags_SourceNoDisableHover)) {
			
				selected_file_.assign(file.path().generic_string().c_str());
				ImGui::SetDragDropPayload("UPDATED_PATH", &selected_file_, sizeof(std::string));
				ImGui::Text(selected_file_.c_str());
				ImGui::EndDragDropSource();
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