#include "ImguiWindows\AssetWindow.h"

void AssetWindow::Init() {
	imgui_ = &*CORE->GetSystem<ImguiSystem>();
	path_selection_ = "Resources";
	b_create = false;
	b_makefolder = false;
	folder_to_del = {};
}

void AssetWindow::Update() {
	float windowW = ImGui::GetContentRegionAvailWidth(), windowH = ImGui::GetContentRegionAvail().y;
	if (imgui_->b_asset) {
		ImGui::PushFont(imgui_->img_font_);

		ImGui::Begin("Asset Browser", &imgui_->b_asset);

		DisplayFolders(windowW, windowH);

		ImGui::SameLine();

		DisplayFolderFiles(windowW, windowH);

		ImGui::End();
		ImGui::PopFont();
	}

	if (b_create)
		MakeNewFolder();

	if (b_deletefolder)
		ImGui::OpenPopup(ICON_FA_TRASH "Are you Sure?");

	DeleteWholeFolder();
	
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

void AssetWindow::CheckFileType() {

	std::string fileName = {};

	static ImGuiTextFilter filter;
	filter.Draw(ICON_FA_FILTER, 200.0f);

	if (!path_selection_.empty()) {
		for (auto& file : fs::directory_iterator(path_selection_)) {
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

void AssetWindow::MakeNewFolder() {

	ImGui::Begin("Add Folder/Files", &b_create);

	if (b_makefolder) {

		std::string folderName;
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), folderName.c_str());
		ImGui::Text(ICON_FA_FOLDER " Adding a New Folder to: "); ImGui::SameLine(0, 3);
		ImGui::Text(path_selection_.generic_string().c_str());
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

void AssetWindow::FileMenuBar() {
	std::string path{};
	ImGui::BeginMenuBar();
	if (ImGui::MenuItem(ICON_FA_FOLDER_PLUS)) {
		b_create = true;
		b_makefolder = true;
	}

	imgui_->ImguiHelp("Add New Folder", 0);

	if (ImGui::MenuItem(ICON_FA_COPY) && !path_selection_.empty()) {

		std::string source_dir = imgui_->OpenSaveDialog("(*.*) All Files\0* *.*\0", 0);

		if (!source_dir.empty()) {

			std::string destination_dir = path_selection_.generic_string();
			fs::copy(source_dir, destination_dir, fs::copy_options::skip_existing);
		}
	}

	imgui_->ImguiHelp("Copy File to Current Directory", 0);

	ImGui::Text(ICON_FA_TRASH);
	imgui_->ImguiHelp("Trash Bin. WARNING:\n CANNOT UNDO so be careful");

	if (ImGui::BeginDragDropTarget()) {

		if (const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("UPDATED_PATH")) {

			if (payLoad->DataSize == sizeof(std::string)) {

				path = *((std::string*)payLoad->Data);

				if (fs::is_directory(path) && fs::is_empty(path) || fs::is_regular_file(path)) {
					fs::remove(path);
				}
				else if (fs::is_directory(path) && !fs::is_empty(path))
				{
					b_deletefolder = true;
					folder_to_del = path;
				}				
			}
		}

		ImGui::EndDragDropTarget();
	}


	ImGui::Text(FileString(ICON_FA_PLUS, ICON_FA_IMAGE).c_str());
	imgui_->ImguiHelp("Add in New Texture", 0);
	ImGui::EndMenuBar();

}

void AssetWindow::DeleteWholeFolder() {

	ImVec2 centre = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(centre, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal(ICON_FA_TRASH "Are you Sure?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Folder: "); ImGui::SameLine(0, 3); ImGui::TextColored(GOLDENORANGE, folder_to_del.c_str()); ImGui::SameLine(0, 3);
		ImGui::Text("is NOT EMPTY!\nYou sure you want to delete?\nThis cannot be undone!!!");

		imgui_->CustomImGuiButton(REDDEFAULT, REDHOVERED, REDACTIVE);
		if (ImGui::Button("Yes"))
		{
			fs::remove_all(folder_to_del);
			ImGui::CloseCurrentPopup();
			folder_to_del = {};
			b_deletefolder = false;
		}

		ImGui::PopStyleColor(3);
		ImGui::SameLine(0, 3);

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
			folder_to_del = {};
			b_deletefolder = false;
		}

		ImGui::EndPopup();
	}
}

void AssetWindow::DisplayFolders(float window_width, float window_height) {

	// Print out all directories
	ImGui::BeginChild("File Directories", { window_width / 7, window_height }, true);

	ImGui::Text("Folders/Directories:");

	if (ImGui::Selectable(ICON_FA_FOLDER " Resources"))
		path_selection_ = "Resources";

	FileDirectoryCheck("Resources");
	ImGui::EndChild();

}

void AssetWindow::DisplayFolderFiles(float window_width, float window_height) {

	ImGui::BeginChild("Files", { window_width / 3 , window_height }, true, ImGuiWindowFlags_MenuBar);
	ImGui::Text("Current Path:"); ImGui::SameLine(0, 3);
	ImGui::Text(FileString(ICON_FA_FOLDER_OPEN, path_selection_.generic_string()).c_str());

	FileMenuBar();
	CheckFileType();

	ImGui::EndChild();

}

std::string AssetWindow::FileString(std::string icon, std::string file_name) {

	return (icon + std::string(" ") + file_name);
}

std::string AssetWindow::DirectoryName(fs::directory_entry directory) {

	std::string tempPath = directory.path().generic_string();
	return tempPath.substr(tempPath.find_last_of("/") + 1);

}