#include "ImguiWindows\AssetWindow.h"

void AssetWindow::Init() {
	imgui_ = &*CORE->GetSystem<ImguiSystem>();
	texture_ = &*CORE->GetManager<TextureManager>();
	path_selection_ = "Resources";
	folder_to_del_ = {};
	img_to_add_ = {};
	chosen_json_ = {};
	b_create = false;
	b_makefolder = false;
	b_addtexture = false;
	b_tex = false;
	b_anim = false;

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

	if (b_addtexture) {

		std::string filename = {};
		std::string file = {};

		ImGui::Begin("Add New Texture", &b_addtexture);

		ImGui::Checkbox("Add Texture", &b_tex);
		ImGui::Checkbox("Add Animation", &b_anim);

		// choose the json file to change to add to
		if (b_tex) {
			ImGui::Text("Choose File to modify");
			if (ImGui::BeginCombo("##json", (chosen_json_.empty() ? "Choose File" : chosen_json_.c_str()))) {

				for (auto& texjson : fs::directory_iterator("Resources/AssetsLoading")) {

					if (fs::is_regular_file(texjson) && texjson.path().extension() == ".json" && texjson.path().filename().generic_string().find("texture") != texjson.path().filename().generic_string().npos) {

						filename = texjson.path().filename().generic_string().c_str();

						if (ImGui::Selectable(filename.c_str())) {

							if (filename.find("_") != filename.npos)
								file = filename.substr(0, filename.find("_"));
							else
								file = filename.substr(0, filename.find(texjson.path().extension().generic_string().c_str()));

							tex_info_.clear(); // clear the map before changing over to another json
							LoadTextureJson(file);
							chosen_json_ = texjson.path().generic_string().c_str();
						}
					}
				}
				ImGui::EndCombo();
			}
		}
		// display json file info
		for (auto it = tex_info_.begin(); it != tex_info_.end(); ++it) {

			if (ImGui::TreeNodeEx(it->first.c_str())) {

				if (ImGui::TreeNode("Current Path:  %s", it->second.path.c_str())) {

					if (ImGui::Button("Update")) {

						std::string selectedpath = imgui_->OpenSaveDialog("(*.png) Spritesheets/Textures\0* .png\0", 1);

						if (!selectedpath.empty())
							it->second.path = selectedpath;
					}
					ImGui::TreePop();
				}
				if (ImGui::TreeNodeEx("Row & Columns")) {

					static char bufferR[256];
					static char bufferC[256];

					ImGui::Text("Number of Columns: %d, Number of Rows: %d", it->second.column, it->second.row);

					ImGui::PushItemWidth(150.0f);
					ImGui::Text("Columns: "); ImGui::SameLine(0, 3);
					if (ImGui::InputText("##col", bufferC, sizeof(bufferC), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank)) {
						std::string colinput = bufferC;
						it->second.column = std::stoi(colinput);
					}
					ImGui::Text("Rows: "); ImGui::SameLine(0, 3);
					if (ImGui::InputText("##row", bufferR, sizeof(bufferR), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank)) {
						std::string rowinput = bufferR;
						it->second.row = std::stoi(rowinput);
					}

					ImGui::PopItemWidth();
					ImGui::TreePop();
				}

				if (ImGui::Button("Delete")) {
					ImGui::OpenPopup("delete texture check");					
				}

				if (ImGui::BeginPopup("delete texture check")) {

					ImGui::Text("Removing Texture: %s\nAre You Sure?", it->first.c_str());
					
					if (ImGui::Button("YES")) {

						if (it == tex_info_.begin())
							it = tex_info_.erase(it);
						else {

							it = tex_info_.erase(it);
							--it;
						}
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine(0, 3);
					if (ImGui::Button("Cancel"))
						ImGui::CloseCurrentPopup();

					ImGui::EndPopup();
				}

				if (ImGui::Button("Load In"))
					texture_->CreateTileset(it->second.path.c_str(), it->second.column, it->second.row, it->first.c_str());

				if (ImGui::Button("Save")) {

					rapidjson::StringBuffer sb;
					rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

					std::string despath = chosen_json_;

					std::ofstream filestream(despath);
					if (filestream.is_open()) {

						writer.StartObject();

						for (auto fileit = tex_info_.begin(); fileit != tex_info_.end(); ++fileit) {

							writer.Key(fileit->first.c_str());
							writer.String((fileit->second.path + " " + std::to_string(fileit->second.column) + " " + std::to_string(fileit->second.row)).c_str());
						}

						writer.EndObject();
						filestream << sb.GetString();
					}

					filestream.close();
				}
				ImGui::TreePop();
				break;
			}
		}

		if (!chosen_json_.empty()) {
			if (ImGui::CollapsingHeader("Add New Texture")) {
				TextureInfo newtex;
				std::string folderName = {};
				ImGui::Text("When a new texture is added in, Rows & Columns will be added in with default value of 1\nUpdate it if needed");
				ImGui::Text("Adding Texture Path: %s", img_to_add_.c_str());
				static char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, sizeof(buffer), folderName.c_str());

				if (ImGui::InputTextWithHint("##newTexturename","Enter a Name & press Enter" , buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank)) {
					newtex.path = img_to_add_;
					newtex.column = newtex.row = 1;
					tex_info_[std::string(buffer)] = newtex;

					img_to_add_ = {};
					newtex = {};
				}
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


	ImGui::MenuItem(FileString(ICON_FA_PLUS, ICON_FA_IMAGE).c_str());
	imgui_->ImguiHelp("Add in New Texture", 0);
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payLoadtex = ImGui::AcceptDragDropPayload("UPDATED_PATH")) {

			if (payLoadtex->DataSize == sizeof(std::string)) {

				path = *((std::string*)payLoadtex->Data);

				b_addtexture = true;
				img_to_add_ = path;
			}
		}

		ImGui::EndDragDropTarget();
	}



	ImGui::EndMenuBar();

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

void AssetWindow::DeSerializeJSON(const std::string& filename, rapidjson::Document& doc) {

	std::ifstream input_file(filename.c_str());
	DEBUG_ASSERT(input_file.is_open(), "File does not exist");

	// Read each line separated by a '\n' into a stringstream
	std::stringstream json_doc_buffer;
	std::string input;

	while (std::getline(input_file, input))
		json_doc_buffer << input << "\n";

	// Close the file (.json) after
	input_file.close();

	// Parse the stringstream into document (DOM) format
	doc.Parse(json_doc_buffer.str().c_str());
}

void AssetWindow::LoadTextureJson(std::string level_name) {

	rapidjson::Document textures;
	std::string path = "Resources/AssetsLoading/" + level_name + "_texture.json";

	DeSerializeJSON(path, textures);

	const rapidjson::Value& files_arr = textures;
	DEBUG_ASSERT(files_arr.IsObject(), "Level JSON does not exist in proper format");


	for (rapidjson::Value::ConstMemberIterator it = files_arr.MemberBegin(); it != files_arr.MemberEnd(); ++it) {

		std::string tilename{ it->name.GetString() };
		std::stringstream fileinfo;
		fileinfo << it->value.GetString();
		
		TextureInfo tex;
		fileinfo >> tex.path >> tex.column >> tex.row;

		tex_info_[tilename] = tex;

	}
}

std::string AssetWindow::FileString(std::string icon, std::string file_name) {

	return (icon + std::string(" ") + file_name);
}

std::string AssetWindow::DirectoryName(fs::directory_entry directory) {

	std::string tempPath = directory.path().generic_string();
	return tempPath.substr(tempPath.find_last_of("/") + 1);

}

