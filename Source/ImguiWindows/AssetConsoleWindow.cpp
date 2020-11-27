#include "ImguiWindows/AssetConsoleWindow.h"

void AssetConsoleWindow::Init() {

	imgui_ = &*CORE->GetSystem<ImguiSystem>();
	texture_ = &*CORE->GetManager<TextureManager>();
	chosen_json_ = {};
	filesdel_ = {};
	jsonfiles_ = {};

	b_tex = false;
	b_anim = false;
	b_audio = false;
}

void AssetConsoleWindow::Update() {

	if (imgui_->b_addtexture)
		AddTextureAnimation();
}

void AssetConsoleWindow::AddTextureAnimation() {

	ImGui::Begin("Add/Remove/Update Assets", &imgui_->b_addtexture);
	ImGui::Text("Select which files you want to update.");

	ImGui::Checkbox("Texture", &b_tex);
	ImGui::Checkbox("Audio", &b_audio);
	ImGui::Checkbox("Animation", &b_anim);

	ImGui::Separator();

	if (b_tex && imgui_->GetImageAdd().find(".png") != imgui_->GetImageAdd().npos) {
		ImGui::Text("Choose File to modify");

		SelectTextureJson();
		DisplayJson();
		AddBlankJson();

		if (!chosen_json_.empty())
			AddNewTexture();
	}

	ImGui::End();
}

void AssetConsoleWindow::DeSerializeTextureJSON(const std::string& filename, rapidjson::Document& doc) {

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

void AssetConsoleWindow::LoadTextureJson(std::string level_name) {

	rapidjson::Document textures;
	std::string path = "Resources/AssetsLoading/" + level_name + "_texture.json";

	DeSerializeTextureJSON(path, textures);

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

void AssetConsoleWindow::SelectTextureJson() {
	std::string filename = {};
	std::string file = {};

	if (ImGui::BeginCombo("##json", (chosen_json_.empty() ? "Choose File" : chosen_json_.c_str()))) {

		for (auto& texjson : fs::directory_iterator("Resources/AssetsLoading")) {

			if (fs::is_regular_file(texjson) && texjson.path().extension() == ".json" && texjson.path().filename().generic_string().find("texture") != texjson.path().filename().generic_string().npos) {

				filename = texjson.path().filename().generic_string().c_str();

				if (ImGui::Selectable(filename.c_str())) {

					file = FindUnderscore(filename);

					tex_info_.clear(); // clear the map before changing over to another json

					LoadTextureJson(file);
					chosen_json_ = texjson.path().generic_string().c_str();
				}

				file = FindUnderscore(filename);
				jsonfiles_.push_back(file);

			}
		}
		ImGui::EndCombo();
	}
}

void AssetConsoleWindow::DisplayJson() {

	if (!chosen_json_.empty()) {

		if (ImGui::TreeNodeEx((chosen_json_ + " details:").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {

			for (auto it = tex_info_.begin(); it != tex_info_.end(); ++it) {

				if (ImGui::TreeNode(it->first.c_str())) {

					if (ImGui::TreeNode("Current Path of Texture:  %s", it->second.path.c_str())) {

						if (ImGui::Button("Update")) {

							std::string selectedpath = imgui_->OpenSaveDialog("(*.png) Spritesheets/Textures\0* .png\0", 1);

							if (!selectedpath.empty())
								it->second.path = selectedpath;
						}
						ImGui::TreePop();
					}

					if (ImGui::BeginDragDropTarget()) {

						if (const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("UPDATED_PATH")) {

							if (payLoad->DataSize == sizeof(std::string)) {

								std::string updatedpath = *((std::string*)payLoad->Data);

								it->second.path = updatedpath;
							}
						}
						ImGui::EndDragDropTarget();
					}

					if (ImGui::TreeNodeEx("Row & Columns")) {

						static char bufferR[256];
						static char bufferC[256];

						ImGui::Text("Number of Columns: %d, Number of Rows: %d", it->second.column, it->second.row);

						ImGui::PushItemWidth(150.0f);
						ImGui::Text("Columns: "); ImGui::SameLine(0, 3);
						if (ImGui::InputText("##col", bufferC, sizeof(bufferC), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal)) {
							std::string colinput = bufferC;
							it->second.column = std::stoi(colinput);
						}

						ImGui::Text("Rows: "); ImGui::SameLine(0, 3);
						if (ImGui::InputText("##row", bufferR, sizeof(bufferR), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal)) {
							std::string rowinput = bufferR;
							it->second.row = std::stoi(rowinput);
						}

						ImGui::PopItemWidth();
						ImGui::TreePop();
					}

					if (ImGui::Button("Remove"))
						ImGui::OpenPopup("Sure?");

					if (ImGui::BeginPopup("Sure?")) {
						std::string del = it->first;
						ImGui::Text("Removing Texture: %s\nAre You Sure?", it->first.c_str());

						imgui_->CustomImGuiButton(REDDEFAULT, REDHOVERED, REDACTIVE);
						if (ImGui::Button("YES")) {

							if (it == tex_info_.begin())
								it = tex_info_.erase(it);
							else {

								it = tex_info_.erase(it);
								--it;
							}

							filesdel_.push_back(del);
							ImGui::CloseCurrentPopup();
						}

						ImGui::PopStyleColor(3);
						ImGui::SameLine(0, 3);
						if (ImGui::Button("Cancel"))
							ImGui::CloseCurrentPopup();

						ImGui::EndPopup();
					}

					if (ImGui::Button("Load into Editor"))
						texture_->CreateTileset(it->second.path.c_str(), it->second.column, it->second.row, it->first.c_str());

					ImGui::TreePop();
					break;
				}
			}
			ImGui::TreePop();
		}
	}

	ImGui::Separator();

	if (ImGui::Button("Save & Reload Textures")) {

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

		for (int i = 0; i < filesdel_.size(); i++)
			texture_->UnloadTileset(filesdel_[i]);

		filesdel_.clear();

		for (int j = 0; j < jsonfiles_.size(); j++)
			texture_->TextureBatchLoad(jsonfiles_[j]);

		jsonfiles_.clear();
	}
}

void AssetConsoleWindow::AddNewTexture() {

	if (ImGui::CollapsingHeader("Add New Texture")) {
		TextureInfo newtex;
		std::string folderName = {};
		ImGui::Text("When a new texture is added in, Rows & Columns will be added with default value of 1\nUpdate it if needed");
		ImGui::Text("Adding Texture Path: %s", imgui_->GetImageAdd().c_str());
		static char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), folderName.c_str());

		if (ImGui::InputTextWithHint("##newTexturename", "Enter a Name & press Enter", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank)) {
			newtex.path = imgui_->GetImageAdd();
			newtex.column = newtex.row = 1;
			tex_info_[std::string(buffer)] = newtex;

			imgui_->SetImageAdd({});
			newtex = {};
		}
	}
}

void AssetConsoleWindow::AddBlankJson() {

	if (ImGui::CollapsingHeader("Add Blank Json File")) {
		std::string folderName;
		char filebuffer[256];
		memset(filebuffer, 0, sizeof(filebuffer));
		strcpy_s(filebuffer, sizeof(filebuffer), folderName.c_str());

		ImGui::Text("New Files will be reflected in the dropdown above and in the asset browser");
		if (ImGui::InputTextWithHint("##row", "Enter FileName & press ENTER", filebuffer, sizeof(filebuffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank)) {

			if (!std::string(filebuffer).empty()) {

				std::string pathtext = filebuffer;
				fs::path filepath = std::string("Resources/AssetsLoading/" + pathtext + "_texture.json").c_str();
				std::ofstream destfile(filepath, std::ios::binary | std::ios::app);

				destfile << "{\n\n}"; // prepare the file as a blank json
				destfile.close();
			}
		}
	}
}

std::string AssetConsoleWindow::FindUnderscore(std::string filename) {
	if (filename.find("_") != filename.npos)
		return (filename.substr(0, filename.find("_")));
	else
		return std::string();
}
