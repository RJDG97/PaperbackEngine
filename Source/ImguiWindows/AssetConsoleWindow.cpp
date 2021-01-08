/**********************************************************************************
*\file         AssetConsoleWindow.cpp
*\brief        Contains definition of functions and variables used for
*			   the AssetConsoleWindow for the management of assets added into the
*			   engine (Eg. Audio, Textures, Animation)

*\author	   Ee Ling Adele, Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "ImguiWindows/AssetConsoleWindow.h"

void AssetConsoleWindow::Init() {

	imgui_ = &*CORE->GetSystem<ImguiSystem>();
	texture_ = &*CORE->GetManager<TextureManager>();
	sound_ = &*CORE->GetSystem<SoundSystem>();
	graphics_ = &*CORE->GetSystem<GraphicsSystem>();
	component_ = &*CORE->GetManager<ComponentManager>();
	entities_ = &*CORE->GetManager<EntityManager>();

	archetype_ = &*imgui_->GetWindow<ArchetypeWindow>();

	chosen_json_ = {};
	filesdel_ = {};
	jsonfiles_ = {};
	tex_info_ = {};
	audio_info = {};
	listEntity_ = {};

	b_unload = false;
	b_wrong_type = false;
	b_load = false;

	type = AddFile::ADDNOTHING;
}

void AssetConsoleWindow::Update() {

	if (imgui_->b_addtexture)
		AddTextureAnimation();

	UnloadPopup();
	WrongTypePopup();
}

void AssetConsoleWindow::AddTextureAnimation() {

	ImGui::Begin("Add/Remove/Update Assets", &imgui_->b_addtexture);
	ImGui::Text("Select what type of files you want to update."); ImGui::SameLine(0, 5);
	imgui_->ImguiHelp("Press the'Save & Reload Textures' button after making\nany confirmed changes, and before changing to another file.\nIf you change file before confirming your changes,\nall previous amendments will be undone");
	ImGui::Text("Type of Assets:");
	ImGui::RadioButton("Texture", &(int&)type, (int)AddFile::ADDTEXTURE);
	ImGui::RadioButton("Audio", &(int&)type, (int)AddFile::ADDAUDIO);
	ImGui::RadioButton("Animation", &(int&)type, (int)AddFile::ADDANIMATION);

	ImGui::Separator();

	if(type == AddFile::ADDTEXTURE) {

		SelectAssetJson();
		DisplayTextureJson();
		AddBlankJson();

		if (!chosen_json_.empty())
			AddNewAsset();
	}
	if (type == AddFile::ADDAUDIO) {

		SelectAssetJson();
		DisplayAudioJson();

		if (!chosen_json_.empty())
			AddNewAsset();
	}

	ImGui::End();

	if (b_load)
		ImGui::OpenPopup("Reloading Completed");
	imgui_->PopUpMessage("Reloading Completed", "Files have been reloaded");
	b_load = false;
}

void AssetConsoleWindow::LoadTextureJson(std::string level_name) {

	rapidjson::Document texture_doc;
	std::string path = "Resources/AssetsLoading/" + level_name + "_texture.json";

	DeSerializeJSON(path, texture_doc);

	const rapidjson::Value& files_arr = texture_doc;
	DEBUG_ASSERT(files_arr.IsObject(), "Level JSON does not exist in proper format");

	for (rapidjson::Value::ConstMemberIterator it = files_arr.MemberBegin(); it != files_arr.MemberEnd(); ++it) {

		std::string tilename{ it->name.GetString()};
		std::stringstream fileinfo;
		fileinfo << it->value.GetString();

		TextureInfo tex;
		fileinfo >> tex.path >> tex.column >> tex.row;

		tex_info_[tilename] = tex;
	}
}

void AssetConsoleWindow::LoadSoundJson(std::string level_name) {

	rapidjson::Document doc;

	DeSerializeJSON(level_name, doc);

	const rapidjson::Value& audio_arr = doc;
	DEBUG_ASSERT(audio_arr.IsObject(), "Entry does not exist in JSON");

	for (rapidjson::Value::ConstMemberIterator it = audio_arr.MemberBegin(); it != audio_arr.MemberEnd(); ++it) {

		const rapidjson::Value& value_arr = it->value;

		std::string audio_label{ it->name.GetString() };

		for (rapidjson::Value::ConstValueIterator val_it = value_arr.Begin(); val_it != value_arr.End(); ++val_it) {

			const rapidjson::Value& member = *val_it;

			std::stringstream ss;

			for (rapidjson::Value::ConstMemberIterator it2 = member.MemberBegin(); it2 != member.MemberEnd(); ++it2) {

				ss << it2->value.GetString() << " ";
			}

			AudioInfo aud;

			ss >> aud.path;

			ss >> aud.volume >> aud.min_distance
			   >> aud.vol_falloff >> aud.loop;

			audio_info[audio_label] = aud;
		}
	}
}

void AssetConsoleWindow::SelectAssetJson() {
	std::string filename = {};
	std::string file = {};

	if (ImGui::BeginCombo("##json", (chosen_json_.empty() ? "Choose File" : chosen_json_.c_str()))) {

		for (auto& assetjson : fs::directory_iterator("Resources/AssetsLoading")) {

			switch(type)
			{
			case AddFile::ADDTEXTURE:
			{
				if (fs::is_regular_file(assetjson) && assetjson.path().extension() == ".json" && imgui_->CheckString(assetjson.path().filename().generic_string(), "texture")) {

					filename = assetjson.path().filename().generic_string().c_str();

					if (ImGui::Selectable(filename.c_str())) {

						file = FindUnderscore(filename);

						tex_info_.clear(); // clear the map before changing over to another json

						LoadTextureJson(file);

						chosen_json_ = assetjson.path().generic_string().c_str();

						json_to_load_ = file;
					}
				}
			}
				break;

			case AddFile::ADDAUDIO:
			{
				if (fs::is_regular_file(assetjson) && assetjson.path().extension() == ".json" && imgui_->CheckString(assetjson.path().filename().generic_string(), "sounds")) {

					file = assetjson.path().filename().generic_string().c_str();

					if (ImGui::Selectable(file.c_str())) {

						filename = assetjson.path().generic_string().c_str();

						audio_info.clear();

						LoadSoundJson(filename);

						chosen_json_ = assetjson.path().generic_string().c_str();

						json_to_load_ = filename;
					}
				}
			}
				break;
			}
		}
		ImGui::EndCombo();
	}
}

void AssetConsoleWindow::DisplayAudioJson() {
	if (!chosen_json_.empty()) {

		if (ImGui::TreeNodeEx((chosen_json_ + " details:").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {

			for (auto it = audio_info.begin(); it != audio_info.end(); ++it) {

				if (ImGui::TreeNode(it->first.c_str())) {

					if (ImGui::TreeNode("Current Path of Texture: %s", it->second.path.c_str())) {

						if (ImGui::BeginDragDropTarget()) {

							if (const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("UPDATED_PATH")) {

								if (payLoad->DataSize == sizeof(std::string))

									it->second.path = *((std::string*)payLoad->Data);
							}
							ImGui::EndDragDropTarget();
						}

						if (ImGui::Button("Update")) {

							std::string selectedpath = imgui_->OpenSaveDialog("(*.png) Spritesheets/Textures\0* .png\0", 0);

							if (!selectedpath.empty())
								it->second.path = selectedpath;
						}

						ImGui::TreePop();
					}

					if (ImGui::TreeNode("Audio Settings")) {

						static char buffer_vol[32];
						static char buffer_min[32];
						static char buffer_falloff[32];
						static char buffer_loop[32];

						ImGui::Text("Current Volume Set: %.2f", it->second.volume); ImGui::SameLine(0, 5); imgui_->ImguiHelp("Enter a value between 0.0f and 1.0f");

						if (ImGui::InputTextWithHint("##vol", "Enter a value between 0.0f and 1.0f" ,buffer_vol, sizeof(buffer_vol), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal))
							it->second.volume = std::stof(buffer_vol);

						ImGui::Separator();
						ImGui::Text("Details below, don't have to worry about them. Its mainly for the sound emitter usage");

						ImGui::Text("Min Distance Needed: %.2f", it->second.min_distance);
						if (ImGui::InputText("##min", buffer_min, sizeof(buffer_min), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal))
							it->second.min_distance = std::stof(buffer_min);

						ImGui::Text("Volume Fallioff: %.2f", it->second.vol_falloff);
						if (ImGui::InputText("##fall", buffer_falloff, sizeof(buffer_falloff), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal))
							it->second.vol_falloff = std::stof(buffer_falloff);

						ImGui::Text("Looping? : %d", it->second.loop); ImGui::SameLine(0, 5); imgui_->ImguiHelp("Enter in either 1 or 0 for this field");
						if (ImGui::InputTextWithHint("##loop", "Enter in either 1 or 0 for this field", buffer_loop, sizeof(buffer_loop), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsDecimal))
							it->second.loop = std::stoi(buffer_loop);

						ImGui::TreePop();
					}

					if (ImGui::Button("Remove"))
						ImGui::OpenPopup("Remove Sound?");

					ImVec2 centre = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(centre, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

					if (ImGui::BeginPopupModal("Remove Sound?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

						std::string del = it->first;
						ImGui::Text("Removing Texture: %s\nMake Sure no object is using this texturer?", it->first.c_str());
						// later add to show which textures are using it
						imgui_->CustomImGuiButton(REDDEFAULT, REDHOVERED, REDACTIVE);
						if (ImGui::Button("YES")) {

							if (it == audio_info.begin())
								it = audio_info.erase(it);
							else {

								it = audio_info.erase(it);
								--it;
							}

							filesdel_.push_back(del);

							for (int i = 0; i < filesdel_.size(); ++i)
								sound_->RemoveSound(filesdel_[i]);

							filesdel_.clear();

							b_unload = true;

							ImGui::CloseCurrentPopup();
						}

						ImGui::PopStyleColor(3);
						ImGui::SameLine(0, 5);

						if (ImGui::Button("Cancel"))
							ImGui::CloseCurrentPopup();

						ImGui::EndPopup();

					}
					ImGui::TreePop();
					break;
				}
			}
			ImGui::TreePop();
		}
	}

	ImGui::Separator();

	if (ImGui::Button("Save & Reload Textures"))
		SerializeJson();
}

void AssetConsoleWindow::DisplayTextureJson() {

	std::string entityname = {};
	std::vector<std::string> listtextures = {};
	GLuint texture_ID = {};
	Texture* texture = {};

	if (!chosen_json_.empty()) {

		if (ImGui::TreeNodeEx((chosen_json_ + " details:").c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {

			for (auto it = tex_info_.begin(); it != tex_info_.end(); ++it) {

				if (ImGui::TreeNode(it->first.c_str())) {

					if (ImGui::TreeNode("Current Path of Texture:  %s", it->second.path.c_str())) {

						if (ImGui::BeginDragDropTarget()) {

							if (const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("UPDATED_PATH")) {

								if (payLoad->DataSize == sizeof(std::string))

									it->second.path = *((std::string*)payLoad->Data);
							}
							ImGui::EndDragDropTarget();
						}

						if (ImGui::Button("Update")) {

							std::string selectedpath = imgui_->OpenSaveDialog("(*.png) Spritesheets/Textures\0* .png\0", 0);

							if (!selectedpath.empty())
								it->second.path = selectedpath;
						}
						ImGui::TreePop();
					}

					if (ImGui::TreeNodeEx("Row & Columns", ImGuiTreeNodeFlags_DefaultOpen)) {

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
						ImGui::OpenPopup("Remove Texture");

					imgui_->SetPopupPosition();

					if (ImGui::BeginPopupModal("Remove Texture", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
						std::string del = it->first;

						listtextures = texture_->GetTileset(del)->GetTileNames(); // get the list of textures
						
						for (int i = 0; i < listtextures.size(); ++i) {

							texture = texture_->GetTexture(listtextures[i]); //get the actual texture
							texture_ID = texture->GetTilesetHandle(); //get the ID (to check thru the entities)
							listArchetype_ = archetype_->CheckArchetypeTexture(listtextures[i]); // check if archetype has texture
						}
						listEntity_ = graphics_->EntitiesWithThisTexture(texture_ID);

						if (!listArchetype_.empty()) { // entities are using the texture

							ImGui::TextColored(REDDEFAULT, "UNABLE TO REMOVE TEXTURE:\n'%s' ", it->first.c_str());
							ImGui::Text("Currently been used by:");

							ImGui::TextColored(GOLDENORANGE, "Archetype(s):");
							for (int j = 0; j < listArchetype_.size(); ++j)
								ImGui::Text(listArchetype_[j].c_str());

							ImGui::Separator();

							if (!listEntity_.empty()) {
								ImGui::TextColored(GOLDENORANGE, "Entity(ies):");
								for (int i = 0; i < listEntity_.size(); ++i)
									ImGui::Text((component_->GetComponent<Name>(listEntity_[i])->GetName() + std::to_string(listEntity_[i])).c_str());

								ImGui::Separator();
							}

							if (ImGui::Button("OK"))
								ImGui::CloseCurrentPopup();
						}
						else {

							ImGui::Text("'%s' is not being used by any Archetypes\nor Active Entities (of the CURRENT SCENE)", del.c_str());
							ImGui::Text("You may remove the texture");

							if (ImGui::Button("YES")) {

								if (it == tex_info_.begin())
									it = tex_info_.erase(it);
								else {

									it = tex_info_.erase(it);
									--it;
								}

								filesdel_.push_back(del);

								for (int i = 0; i < filesdel_.size(); i++)
									texture_->UnloadTileset(filesdel_[i]);

								filesdel_.clear();

								b_unload = true;

								ImGui::CloseCurrentPopup();
							}
							ImGui::SameLine(0, 3);

							if (ImGui::Button("Cancel"))
								ImGui::CloseCurrentPopup();

						}

						ImGui::EndPopup();
					}
					ImGui::TreePop();
					break;
				}
			}
			ImGui::TreePop();
		}
	}

	ImGui::Separator();

	if (ImGui::Button("Save & Reload Textures"))
		SerializeJson();
}


void AssetConsoleWindow::AddNewAsset() {

	if (ImGui::CollapsingHeader("Add New Asset")) {

		std::string folderName = {};

		static char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), folderName.c_str());

		ImGui::Text("Any New Assets Added In, other than the Label and the Path\nrest of the details will be set with Default Value. Update them accordingly if need be");

		if (!imgui_->GetAssetAdd().empty()) {

			if(type == AddFile::ADDTEXTURE) {

				TextureInfo newtex;
				ImGui::TextColored(GOLDENORANGE, "Adding in a NEW Texture");
				ImGui::Text("Path: %s", imgui_->GetAssetAdd().c_str());

				UpdatePath();

				if (ImGui::InputTextWithHint("##newTexturename", "Enter a Name & press Enter", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank)) {

					if (!std::string(buffer).empty()) {

						if (imgui_->CheckString(imgui_->GetAssetAdd(), ".png")) {

							newtex.path = imgui_->GetAssetAdd();
							newtex.column = newtex.row = 1;
							tex_info_[std::string(buffer)] = newtex;

							imgui_->SetAssetAdd({});
							newtex = {};
						}
						else
							b_wrong_type = true;
					}
				}
			}

			if (type == AddFile::ADDAUDIO) {

				AudioInfo newsound;
				ImGui::TextColored(SKYBLUE, "Adding in a NEW Audio");
				ImGui::Text("Path: %s", imgui_->GetAssetAdd().c_str());

				UpdatePath();

				if (ImGui::InputTextWithHint("##newAudioname", "Enter a Name & press Enter", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank)) {

					if (!std::string(buffer).empty()) {

						if (imgui_->CheckString(imgui_->GetAssetAdd(), ".mp3")) {

							newsound.path = imgui_->GetAssetAdd();
							newsound.volume = 1.0f;
							newsound.min_distance = newsound.vol_falloff = 0.0f;
							newsound.loop = 0;

							audio_info[std::string(buffer)] = newsound;

							imgui_->SetAssetAdd({});
						}
						else
							b_wrong_type = true;
					}
				}
			}
		}
		else {

			ImGui::Text("No Texture Set, Drag One here or to the add icon in the asset browser");

			if (ImGui::BeginDragDropTarget()) {

				if (const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("UPDATED_PATH")) {

					if (payLoad->DataSize == sizeof(std::string))

						imgui_->SetAssetAdd(*((std::string*)payLoad->Data));
				}
				ImGui::EndDragDropTarget();
			}
		}
	}
}

void AssetConsoleWindow::UpdatePath() {

	if (ImGui::BeginDragDropTarget()) {

		if (const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("UPDATED_PATH")) {

			if (payLoad->DataSize == sizeof(std::string))

				imgui_->SetAssetAdd(*((std::string*)payLoad->Data));
		}
		ImGui::EndDragDropTarget();
	}
}

void AssetConsoleWindow::WrongTypePopup() {

	if (b_wrong_type)
		ImGui::OpenPopup("Wrong File Type");

	ImVec2 centre = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(centre, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Wrong File Type", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

		if (type == AddFile::ADDTEXTURE)
			ImGui::Text("The Asset you are trying to load in:\n%s\nis of the wrong format.\nOnly Png Image Files", imgui_->GetAssetAdd().c_str());

		else if (type == AddFile::ADDAUDIO)
			ImGui::Text("The Asset you are trying to load in:\n%s\nis of the wrong format.\nOnly Mp3 Audio Files", imgui_->GetAssetAdd().c_str());


		if (ImGui::Button("OK")) {

			ImGui::CloseCurrentPopup();
			b_wrong_type = false;
		}

		ImGui::EndPopup();
	}
}

void AssetConsoleWindow::UnloadPopup() {

	if (b_unload) {

		if (type == AddFile::ADDTEXTURE)
			ImGui::OpenPopup("Texture Unloaded");
		if (type == AddFile::ADDAUDIO)
			ImGui::OpenPopup("Sound Unloaded");
	}

	imgui_->PopUpMessage("Texture Unloaded", "Texture has been unloaded from the engine");
	b_unload = false;

	imgui_->PopUpMessage("Sound Unloaded", "Sound has been unloaded from the engine");
	b_unload = false;

}

void AssetConsoleWindow::AddBlankJson() {

	if (ImGui::CollapsingHeader("Add Blank Json File")) {
		std::string pathtext;
		char filebuffer[256];
		memset(filebuffer, 0, sizeof(filebuffer));
		strcpy_s(filebuffer, sizeof(filebuffer), pathtext.c_str());
		fs::path filepath{};

		ImGui::Text("New Files will be reflected in the dropdown above and in the asset browser");
		if (ImGui::InputTextWithHint("##row", "Enter FileName & press ENTER", filebuffer, sizeof(filebuffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank)) {

			if (!std::string(filebuffer).empty()) {

				 pathtext = filebuffer;
				 if (type == AddFile::ADDTEXTURE)
					filepath = std::string("Resources/AssetsLoading/" + pathtext + "_texture.json").c_str();

				 if (type == AddFile::ADDANIMATION)
					filepath = std::string("Resources/AssetsLoading/" + pathtext + "_animation.json").c_str();

				std::ofstream destfile(filepath, std::ios::binary | std::ios::app);

				destfile << "{\n\n}"; // prepare the file as a blank json
				destfile.close();
			}
		}
	}
}

void AssetConsoleWindow::SerializeJson() {

	if (type == AddFile::ADDTEXTURE) {

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

		if (!json_to_load_.empty()) {
			texture_->TextureBatchLoad(json_to_load_);

			b_load = true;
		}

	}

	if (type == AddFile::ADDAUDIO) {

		rapidjson::StringBuffer sb;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

		std::string despath = chosen_json_;

		std::ofstream filestream(despath);

		if (filestream.is_open()) {

			writer.StartObject();

			for (auto& [name, soundfile] : audio_info) {

				writer.Key(name.c_str());

				writer.StartArray();

				writer.StartObject();
				writer.Key("path");
				writer.String(soundfile.path.c_str());

				writer.Key("volume");
				writer.String(std::to_string(soundfile.volume).c_str());

				writer.Key("min distance");
				writer.String(std::to_string(soundfile.min_distance).c_str());

				writer.Key("volume falloff");
				writer.String(std::to_string(soundfile.vol_falloff).c_str());

				writer.Key("loop");
				writer.String(std::to_string(soundfile.loop).c_str());

				writer.EndObject();

				writer.EndArray();
			}

			writer.EndObject();
			filestream << sb.GetString();
		}

		filestream.close();

		if (!json_to_load_.empty()) {
			sound_->DeSerialize(json_to_load_);
			b_load = true;
		}
	}
}

std::string AssetConsoleWindow::FindUnderscore(std::string filename) {
	if (imgui_->CheckString(filename, "_"))
		return (filename.substr(0, filename.find("_")));
	else
		return std::string();
}

bool AssetConsoleWindow::GetUnloadBool() {

	return b_unload;
}

void AssetConsoleWindow::SetUnloadBool(bool updated_bool) {

	b_unload = updated_bool;
}
