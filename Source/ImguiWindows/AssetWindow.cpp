#include "ImguiWindows\AssetWindow.h"
#include <cstring>

void AssetWindow::Init() {
	imgui_ = &*CORE->GetSystem<ImguiSystem>();
	texture_ = &*CORE->GetManager<TextureManager>();
	path_selection_ = "Resources";

	folder_to_del_ = {};

	multifiles_ = {};

	b_create = false;
	b_makefolder = false;

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

	if (b_makefolder) 
		ImGui::OpenPopup("Add Folder");

	if (ImGui::BeginPopup("Add Folder")) {

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

			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
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

		std::string source_dir = imgui_->OpenSaveDialog("(*.*) All Files\0* *.*\0", 0, 1);
		std::string destination_dir = path_selection_.generic_string();

		if (!source_dir.empty()) {

			if (source_dir.find("|") != source_dir.npos) {

				multifiles_ = MultiFileSelection(source_dir);
				for (int i = 0; i < multifiles_.size(); i++)
						fs::copy(multifiles_[i], destination_dir, fs::copy_options::skip_existing);
			}
			else
				fs::copy(source_dir, destination_dir, fs::copy_options::skip_existing); // only 1 file selected
		}
	}

	imgui_->ImguiHelp("Copy File(s) to Current Directory", 0);

	ImGui::MenuItem(ICON_FA_TRASH);
	imgui_->ImguiHelp("Trash Bin. WARNING:\n CANNOT UNDO so be careful", 0);

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
					folder_to_del_ = path;
				}				
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::MenuItem(ICON_FA_PLUS ICON_FA_IMAGE);
	imgui_->ImguiHelp("Add in New Texture", 0);
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payLoadtex = ImGui::AcceptDragDropPayload("UPDATED_PATH")) {

			if (payLoadtex->DataSize == sizeof(std::string)) {

				path = *((std::string*)payLoadtex->Data);

				if (path.find(".png") != path.npos) {

					imgui_->b_addtexture = true;
					imgui_->SetImageAdd(path);
				}
			}
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::EndMenuBar();
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

void AssetWindow::DeleteWholeFolder() {

	ImVec2 centre = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(centre, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal(ICON_FA_TRASH "Are you Sure?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Folder: "); ImGui::SameLine(0, 3); ImGui::TextColored(GOLDENORANGE, folder_to_del_.c_str()); ImGui::SameLine(0, 3);
		ImGui::Text("is NOT EMPTY!\nYou sure you want to delete?\nThis cannot be undone!!!");

		imgui_->CustomImGuiButton(REDDEFAULT, REDHOVERED, REDACTIVE);
		if (ImGui::Button("Yes"))
		{
			fs::remove_all(folder_to_del_);
			ImGui::CloseCurrentPopup();
			folder_to_del_ = {};
			b_deletefolder = false;
		}

		ImGui::PopStyleColor(3);
		ImGui::SameLine(0, 3);

		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
			folder_to_del_ = {};
			b_deletefolder = false;
		}

		ImGui::EndPopup();
	}
}

std::vector<std::string> AssetWindow::MultiFileSelection(std::string appended_files) {

	std::vector<std::string> selectedfiles = {};
	char* nexttoken = {}; // to store the remaining words (for strtok_s use)

	char files[2048];
	memset(files, 0, sizeof(files));
	strcpy_s(files, sizeof(files), appended_files.c_str());

	char* token = strtok_s(files, "|", &nexttoken); // get the directory

	std::string directory = token, tempdir = token;

	while (token != NULL) {

		if (directory != token) {
			tempdir += "\\" + std::string(token);
			selectedfiles.push_back(tempdir);
			tempdir = directory;
		}

		token = strtok_s(NULL, "|", &nexttoken);
	}

	return selectedfiles;
}

std::string AssetWindow::FileString(std::string icon, std::string file_name) {

	return (icon + std::string(" ") + file_name);
}

std::string AssetWindow::DirectoryName(fs::directory_entry directory) {

	std::string tempPath = directory.path().generic_string();
	return tempPath.substr(tempPath.find_last_of("/") + 1);
}

