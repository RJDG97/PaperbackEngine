/**********************************************************************************
*\file         MiscWindow.cpp
*\brief        Contains definition of functions and variables used for
*			   the Miscellaneous Windows (eg. Layer Window and Editor Settings)

*\author	   Ee Ling Adele, Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "ImguiWindows/MiscWindow.h"

void MiscWindow::Init() {

    imgui_ = &*CORE->GetSystem<ImguiSystem>();
    layer_ = &*CORE->GetManager<LayerManager>();
    graphics_ = &*CORE->GetSystem<GraphicsSystem>();
    input_ = &*CORE->GetSystem<InputSystem>();

    assetconsole_ = &*imgui_->GetWindow<AssetConsoleWindow>();

	b_draw = b_grid = b_light = b_Nlayer = b_swap = b_remove = b_notempty = false;

    mousePos_ = { 0,0 };

    newlayertype_ = layerjson_load_ = layername_ = {};
    label_ = assetconsole_->GetFileName(imgui_->chosenlayer_);
    newlayerint = {};

    renderercount = 0;

    layerDelete_ = -1;
}

void MiscWindow::Update() {

	GraphicsLayerWindow();
	EditorSettingsWindow();
    NewLayerPopUp();
    LayerPopUps();
}

void MiscWindow::EditorSettingsWindow() {

	if (imgui_->b_settings) {

		ImGui::Begin("Editor Settings", &imgui_->b_settings);

		DragEntityCheckBox();

		ImGui::SameLine(0, 7);

		if (ImGui::Checkbox("Bounding Boxes", &b_draw)) {

			Message msg(MessageIDTypes::DEBUG_ALL);
			CORE->BroadcastMessage(&msg);
		}

		ImGui::Checkbox("Toggle Lighting", &b_light);
		graphics_->EnableLighting(b_light);

		ImGui::SameLine(0, 3);

		if (imgui_->GetCamera()) {

			ImGui::Checkbox("Draw Grid", &b_grid);

			if (!imgui_->EditorMode()) {

				if (input_->IsMousePressed(0))
					mousePos_ = input_->GetUpdatedCoords();

				ImGui::Text("Current Cursor Position: %.2f, %.2f", mousePos_.x, mousePos_.y);
			}
		}

		if (b_grid)
			imgui_->DrawGrid();
		
		ImGui::End();
	}
}

void MiscWindow::GraphicsLayerWindow() {

	if (imgui_->b_layers) {

		ImGui::Begin("Layers", &imgui_->b_layers, ImGuiWindowFlags_MenuBar);

        ImGui::PushFont(imgui_->img_font_);

        LayerWindowMenuBar();

        LayerDropDown();

        ImGui::Separator();
        
        LayerList();

        ImGui::PopFont();

		ImGui::End();
	}
}

void MiscWindow::LayerWindowMenuBar() {

    int del {-1};

	if (ImGui::BeginMenuBar()) {

        ImGui::PushFont(imgui_->img_font_);

        imgui_->ImguiHelp("Hover Over the Layers to See the Type");

        if (ImGui::MenuItem(ICON_FA_PLUS_SQUARE))
            b_Nlayer = true;
        
        imgui_->ImguiHelp("Adds onto the current set of Layers", 0);
        

        ImGui::MenuItem(ICON_FA_TRASH);
        
        if (ImGui::BeginDragDropTarget()) {

            if (const ImGuiPayload* payloadlayer = ImGui::AcceptDragDropPayload("LAYERDELETE")) {

                if (payloadlayer->DataSize == sizeof(int)) {

                    del = *((int*)payloadlayer->Data);

                    if (CheckLayerEmpty(del)) {

                        layer_->DeleteLayer(del);
                        b_remove = true;
                    }
                    else
                        b_notempty = true;
                }
            }
            ImGui::EndDragDropTarget();
        }

        imgui_->ImguiHelp("Drag unwanted layers here.", 0);

        if (ImGui::MenuItem(ICON_FA_SAVE))
            SerializeLayers();
        
        imgui_->ImguiHelp("Save the current layers", 0);

        ImGui::Checkbox(ICON_FA_EXCHANGE_ALT, &b_swap);
        imgui_->ImguiHelp("Toggle to swap layer order", 0);

        ImGui::PopFont();

        ImGui::EndMenuBar();
    }
}

void MiscWindow::LayerDropDown() {

    std::string layerfile = {};
    std::string filename = {};

    ImGui::PushItemWidth(250.0f);

    if (ImGui::BeginCombo("##Layerjson", (label_.empty() ? "Choose Layer Json to use" : label_.c_str()))) {

        for (auto& layerjson : fs::directory_iterator("Resources/Layers")) {

            if (fs::is_regular_file(layerjson) && layerjson.path().extension() == ".json") {
                
                layerfile = layerjson.path().filename().generic_string().c_str();

                if (ImGui::Selectable(layerfile.c_str())) {

                    filename = assetconsole_->FindUnderscore(layerfile);

                    layer_->LoadLevelLayers(filename); //deserialize

                    imgui_->chosenlayer_ = layerjson.path().generic_string().c_str();

                    label_ = assetconsole_->GetFileName(imgui_->chosenlayer_);

                    layerjson_load_ = filename; // for serialization later
                }
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();

}

void MiscWindow::LayerList() {

    ImGui::Text("Layers");

    if (!layer_->GetRenderLayers()->empty()) {

        std::map<int, RenderLayer> renderlayers = *layer_->GetRenderLayers();

        for (auto it = layer_->GetRenderLayers()->begin(); it != layer_->GetRenderLayers()->end(); ++it) {

            ImGui::Text(("Layer " + std::to_string(it->first) + ": ").c_str());

            ImGui::SameLine(0, 5);

            ImGui::Selectable(it->second.GetName().c_str(), false, 0, ImVec2(120.0f, 0.0f));
            
            if (b_swap && ImGui::IsItemActive() && !ImGui::IsItemHovered()) {

                int next = it->first + (ImGui::GetMouseDragDelta(0).y < 0.0f ? - 1 : 1);

                if (next >= 0 && next < renderlayers.size()) {

                    layer_->SwapLayer(it->first, next);
                    ImGui::ResetMouseDragDelta();
                }
            }

            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID | ImGuiDragDropFlags_SourceNoDisableHover)) {

                ImGui::SetDragDropPayload("LAYERDELETE", &it->first, sizeof(int));
                ImGui::Text(it->second.GetName().c_str());
                ImGui::EndDragDropSource();
            }
            
            imgui_->ImguiHelp(GetLayerType(it->second.GetLayerType()).c_str(), 0);

            //bool visible = it->second.GetVisible();
            // bool ysort = it->second.GetYSorted();

            //ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - 75);

            //if(ImGui::Button(imgui_->VisibleIcon(it->second.GetVisible(), ICON_FA_EYE, ICON_FA_EYE_SLASH).c_str())) {
            //    it->second.GetVisible() ? it->second.SetVisible(false) : it->second.SetVisible(true); 
            //}

            // if (ImGui::Button("WHY")) {
            //     it->second.GetVisible() ? it->second.SetVisible(false) : it->second.SetVisible(true);
            // }

            // std::cout << "Layer Name: " << it->second.GetName() << std::endl;
            // std::cout << "Visibility: " << it->second.GetVisible() << std::endl;

            //it->second.SetVisible(visible);

            //ImGui::SameLine(0, 3);

            //if (ImGui::Button(imgui_->VisibleIcon(ysort, ICON_FA_SORT_NUMERIC_UP , ICON_FA_RANDOM).c_str()))
            //    ysort  = !ysort; 
        
            //imgui_->ImguiHelp("Toggle to sort the\nelements in the layer", 0);
            //it->second.SetYSorted(ysort);
        }
    }
}

void MiscWindow::NewLayerPopUp() {

    std::string newlayer = {};

    if (b_Nlayer)
        ImGui::OpenPopup("Add New Layer");
        
    imgui_->SetPopupPosition();

    if (ImGui::BeginPopup("Add New Layer")) {

    	char buffer[256];
    	memset(buffer, 0, sizeof(buffer));
    	strcpy_s(buffer, sizeof(buffer), newlayer.c_str());

        ImGui::Text(ICON_FA_EDIT " Adding New Layer to: "); ImGui::SameLine(0, 3);

        ImGui::TextColored(GOLDENORANGE, imgui_->chosenlayer_.c_str());;

        ImGui::PushItemWidth(220.0f);

        if (ImGui::BeginCombo("##Layertype", newlayertype_.empty() ?  "Choose Layer Type" : ("Adding: " + newlayertype_).c_str())) {

            for (size_t i = 0; i < IM_ARRAYSIZE(layerType_); ++i) {

                if (ImGui::Selectable(layerType_[i])) {

                    newlayertype_ = layerType_[i];
                    newlayerint = i;
                }
            }
            ImGui::EndCombo();
        }

        ImGui::PopItemWidth();

        if (!newlayertype_.empty()) {

           	if (ImGui::InputTextWithHint("##newLayername", "Enter a Name & press Enter", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CharsNoBlank)) {

                if (!std::string(buffer).empty()) {

                    layer_->AddLayer(std::string(buffer), static_cast<LayerType>(newlayerint));
                    SerializeLayers();
                }
            }
        }

        if(ImGui::Button("Close This Panel")) {

            b_Nlayer = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void MiscWindow::LayerPopUps() {
    
    std::string msg = {};

    if (b_remove) {
        ImGui::BeginPopup("Layer removed");
        msg = assetconsole_->GetFileName(imgui_->chosenlayer_) + " has been removed.";
    }

    imgui_->PopUpMessage("Layer removed", msg.c_str());
    b_remove = false;

    if (b_notempty) 
        ImGui::OpenPopup("NonEmpty Layer");
    
    imgui_->SetPopupPosition();

    if (ImGui::BeginPopupModal("NonEmpty Layer", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::TextColored(REDDEFAULT, "UNABLE TO REMOVE LAYER!!!");
        ImGui::Text("There are "); ImGui::SameLine(0, 2); ImGui::TextColored(GOLDENORANGE, std::to_string(renderercount).c_str()); ImGui::SameLine(0, 3);
        ImGui::Text("Entity(ies) currently using: "); ImGui::SameLine(0, 3); ImGui::TextColored(GOLDENORANGE, layername_.c_str());

        if(ImGui::Button("Ok")) {
            
            ImGui::CloseCurrentPopup();
            b_notempty = false;
        }
        
        ImGui::EndPopup();
    }
}

bool MiscWindow::CheckLayerEmpty(int indexcheck) {

    for (auto it = layer_->GetRenderLayers()->begin(); it != layer_->GetRenderLayers()->end(); ++it) {

        if (it->first == indexcheck)
            if (it->second.GetRenderers()->empty())
                return true;
            else {

                renderercount = it->second.GetRenderers()->size();
                layername_ = it->second.GetName();
                return false;
            }
    }
    return false;
}

void MiscWindow::SerializeLayers() {

    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

    std::string despath = imgui_->chosenlayer_;

    std::ofstream filestream(despath);

    if(filestream.is_open()) {

        writer.StartObject();

        for (auto fileit = layer_->GetRenderLayers()->begin(); fileit != layer_->GetRenderLayers()->end(); ++fileit) {

            writer.Key(fileit->second.GetName().c_str());
            writer.String((std::to_string(fileit->second.GetYSorted()) + " " + GetLayerType(fileit->second.GetLayerType())).c_str());
        }

        writer.EndObject();
        filestream << sb.GetString();
    }

    filestream.close();

    if (!layerjson_load_.empty()) {

        layer_->LoadLevelLayers(layerjson_load_);
        assetconsole_->b_load = true;
    }
}

void MiscWindow::DragEntityCheckBox() {

	bool lock = imgui_->GetLockBool();
	ImGui::Checkbox("Drag Entity", &lock);
	ImGui::SameLine(); imgui_->ImguiHelp("Check this box\nto Drag Entities around");
	imgui_->SetLockBool(lock);
}

std::string MiscWindow::GetLayerType(int layer) {

    for (size_t i = 0; i < sizeof(layerType_); ++i) {

         if (i == layer)
            return layerType_[i];
    }

    return std::string{};
}