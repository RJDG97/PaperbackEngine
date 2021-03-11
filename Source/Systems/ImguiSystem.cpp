/**********************************************************************************
*\file         ImguiSystem.cpp
*\brief        Contains definition of functions and variables used for
*			   the ImguiSystem and the Editor Windows

*\author	   Ee Ling Adele, Sim, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
               or disclosure of this file or its contents without the prior
               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Systems/ImguiSystem.h"
#include "Manager/Amap.h"

#include "ImguiWindows/EntityWindow.h"
#include "ImguiWindows/EntityPathWindow.h"
#include "ImguiWindows/ArchetypeWindow.h"
#include "ImguiWindows/SystemWindow.h"
#include "ImguiWindows/AssetWindow.h"
#include "ImguiWindows/AssetConsoleWindow.h"
#include "ImguiWindows/TextureTilesWindow.h"
#include "ImguiWindows/MiscWindow.h"

#include "GameStates/MenuState.h"

// Expose the Win32 API
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

void ImguiSystem::Init(){
    // Adding window to Imgui's Window map
    AddWindow<ArchetypeWindow>();
    AddWindow<EntityWindow>();
    AddWindow<EntityPathWindow>();
    AddWindow<AssetWindow>();
    AddWindow<AssetConsoleWindow>();
    AddWindow<TextureTilesWindow>();
    AddWindow<MiscWindow>();
    AddWindow<SystemWindow>();

    win_ = &*CORE->GetSystem<WindowsSystem>();
    collision_ = &*CORE->GetSystem<Collision>();
    input_ = &*CORE->GetSystem<InputSystem>();
    entities_ = &*CORE->GetManager<EntityManager>();
    factory_ = &*CORE->GetSystem <EntityFactory>();
    sound_ = &*CORE->GetSystem<SoundSystem>();
    graphics_ = &* CORE->GetSystem<GraphicsSystem>();
    amap_ = &*CORE->GetManager<AMap>();

    cam_arr_ = CORE->GetManager<ComponentManager>()->GetComponentArray<Camera>();

    camera_ = nullptr;
    editor_ = factory_->GetLevel("Editor");

    type = CloseApp::NONE;

    //Imgui Window Bools
    b_archetype_win = true;
    b_entity_win = true;
    b_component = true;
    b_display = false;
    b_edit_path = false;
    b_level_save = false;
    b_asset = false;
    b_edit_comp = false;
    b_add_texture = false;
    b_show_tex = false;
    b_add_path = false;
    b_windows = true;
    b_layers = false;

    b_lock_entity = false;
    b_imgui_mode = false;

    selected_entity_ = nullptr;
    b_close_confirm = false;

    img_to_add_ = {};
    current_loaded_path_ = {};

    archetype_path_ = "Resources/EntityConfig/archetypes.json";

    scene_filter_ = "(*.json) Paperback Engine Scene\0*.json\0";
       
    generic_filter_ = "(*.*) All Files\0* *.*\0";

//////////// Setup Dear ImGui context///////////////////////////

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking (Merging of windows)
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Initialize Imgui Window that is attached to the Imgui System
    for (WindowIt begin = imgui_window_arr_.begin(); begin != imgui_window_arr_.end(); ++begin)
        begin->second->Init();
   
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(win_->ptr_window, true);
    ImGui_ImplOpenGL3_Init(NULL);

    // Load Fonts
    io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/Font/Grandstander-Medium.ttf", 16.0f);
    static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
    img_font_ = io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, 14.0f, &icons_config, icon_ranges);
    bold_font_ = io.Fonts->AddFontFromFileTTF("Resources/Font/Grandstander-Bold.ttf", 16.0f);

    //////////// End iof ImGui Context Setup///////////////////////////

    b_dock_space_open = true;
    b_fullscreen_persistant = true;
    b_fullscreen = b_fullscreen_persistant;

    dock_space_flags_ = ImGuiDockNodeFlags_PassthruCentralNode;
    window_flags_ = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

}

void ImguiSystem::Update(float frametime) {

    UNREFERENCED_PARAMETER(frametime);
	
    if (b_imgui_mode) {
        
    	// if want to add shortcut, call fn here

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        camera_ = GetCamera();

        ImGuiIO& io = ImGui::GetIO();

        if (b_dock_space_open) {
            DockSpaceFlagSet();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::SetNextWindowBgAlpha(0.0f); // set the transparency of the docking central node
            ImGui::Begin("DockSpace", &b_dock_space_open, window_flags_);
            ImGui::PopStyleVar();

            if (b_fullscreen)
                ImGui::PopStyleVar(2);

        	// menu bar
            ImguiMenuBar();

            Popups();

            // for the selection of entity
            if (!EditorMode() && camera_) {

                if (input_->IsMousePressed(0)) {

                    Vector2D new_pos = input_->GetUpdatedCoords();

                    selected_entity_id_ = collision_->SelectEntity(new_pos);
                    selected_entity_ = entities_->GetEntity(selected_entity_id_);
                }
            }

            // DockSpace
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {

                ImGuiID dockspace_id = ImGui::GetID("DockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dock_space_flags_);
            }
        	
            ImGuiCustomStyle();

            if (b_windows) {

                for (WindowIt begin = imgui_window_arr_.begin(); begin != imgui_window_arr_.end(); ++begin)
                    begin->second->Update();
            }

            ImGui::End(); // end of docking space

        }

        ImguiRender();
    }
}

void ImguiSystem::DockSpaceFlagSet() {

    b_fullscreen = b_fullscreen_persistant;
    if (b_fullscreen) {

        ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        window_flags_ |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags_ |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if (dock_space_flags_ & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags_ |= ImGuiWindowFlags_NoBackground;
}

void ImguiSystem::ImGuiCustomStyle() {

    ImVec4* colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.75f);
}

void ImguiSystem::ImguiRender() {

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(win_->ptr_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImguiSystem::ImguiMenuBar() {
	
    if (ImGui::BeginMenuBar()) {

        ImGui::PushFont(img_font_);

        if (ImGui::BeginMenu(ICON_FA_FOLDER " File")) {

            if (ImGui::MenuItem(ICON_FA_TIMES " New Scene")) {
                b_close_confirm = true;
                type = CloseApp::CREATENEWSCENE;
            }

            if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open Scene")) {
                selected_entity_ = {};
                editor_->entity_paths_.clear();
                OpenFile();
                if (!current_loaded_path_.empty())
                    LoadJsonPaths(current_loaded_path_);
            }

            if (ImGui::MenuItem(ICON_FA_SAVE " Save")) {

                if (!editor_->entity_paths_.empty()) {

                    if (!current_loaded_path_.empty()) {

                        SaveFile(current_loaded_path_);
                        b_level_save = true;
                    }
                    else {

                        std::string path = OpenSaveDialog(scene_filter_, 1);
                        SaveFile(path);

                        if (!path.empty()) {

                            LoadJsonPaths(path);
                            b_level_save = true;
                        }
                    }
                }
                else
                    b_add_path = true;
            }
            ImguiHelp(("Current Set Path " + current_loaded_path_).c_str(), 0);

            if (ImGui::MenuItem("Save Scene As...")) {

                if (!editor_->entity_paths_.empty()) {

                    std::string path = OpenSaveDialog(scene_filter_, 1);
                    SaveFile(path);

                    if (!path.empty()) {

                        LoadJsonPaths(path);
                        b_level_save = true;
                    }
                }
                else
                    b_add_path = true;
            }

            if (ImGui::MenuItem("Save Entity Path Only")) {

                if (!editor_->entity_paths_.empty()) {
                    factory_->SerializeCurrentLevelEntities(); // save each entity to their respective path

                    b_level_save = true;
                }
                else
                    b_add_path = true;
            }

            ImguiHelp("This saves the individual paths of each type of entity in the scene.\nUser would have to manually update the scene json themselves.", 0);

            ImGui::Separator();

            if (ImGui::MenuItem(ICON_FA_REPLY " Return to Menu")) {

                b_close_confirm = true;
                type = CloseApp::RETURNMENU;
            }

            ImguiHelp("Did you save?", 0);

            if (ImGui::MenuItem(ICON_FA_POWER_OFF " Exit")) {

                b_close_confirm = true;
                type = CloseApp::EXITAPP;
            }

            ImguiHelp("Did you save?", 0);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Archetypes")) {

            if (ImGui::MenuItem(ICON_FA_SAVE " Save Archetype List As...")) {

                std::string path = OpenSaveDialog(scene_filter_, 0);
                SaveArchetype(path);
            }

            if (ImGui::MenuItem(ICON_FA_SAVE " Save Archetypes List"))
                SaveArchetype(archetype_path_);

            if (ImGui::MenuItem(ICON_FA_FILE_IMPORT " Load Archetypes"))
                LoadArchetype();

            ImGui::Separator();

            if (ImGui::MenuItem(ICON_FA_EDIT " Edit Archetype Path"))
                b_edit_path = true;

            ImGui::Separator();
            ImGui::Checkbox("Toggle Archetype Window", &b_archetype_win);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(ICON_FA_WINDOW_RESTORE " Editor Windows")) {

            ImGui::Checkbox("Toggle Visible Windows", &b_windows);
            ImGui::Checkbox("Toggle Scene Hierachy", &b_entity_win);
            ImGui::Checkbox("Toggle Inspector", &b_component);
            ImGui::Checkbox("Toggle Archetype Window", &b_archetype_win);
            //ImGui::Checkbox("Toggle Layer Hierachy", &b_layers);
            ImGui::Checkbox("Toggle Editor Settings", &b_settings);

            ImGui::Separator();

            ImGui::Checkbox("See System Performance", &b_display);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(ICON_FA_IMAGES " Assets")) {

            ImGui::Checkbox("Toggle Asset Browser", & b_asset);
            ImGui::Checkbox("Toggle Asset Console Window", &b_add_texture);
            ImguiHelp("For adding/updating assets", 0);
            ImGui::Checkbox("Toggle Texture Window", &b_show_tex);
            ImguiHelp("View All Loaded Textures", 0);

            ImGui::EndMenu();
        }

        if (ImGui::MenuItem(VisibleIcon(b_windows).c_str()))
            b_windows = !b_windows;

        ImguiHelp("Toggle Visible Windows", 0);

        ImGui::PopFont();
    }
    ImGui::EndMenuBar();
}

bool ImguiSystem::GetImguiBool() {

    return b_imgui_mode;
}

void ImguiSystem::SetImguiBool(bool mode) {

    b_imgui_mode = mode;
}

std::string ImguiSystem::GetName() {
	
    return "ImGui System";
}

EntityID ImguiSystem::GetSelectedEntity() {

    return selected_entity_id_;
}

void ImguiSystem::ResetSelectedEntity() {

    selected_entity_ = {};
    b_lock_entity = false;
}

bool ImguiSystem::GetLockBool() {

    return b_lock_entity;
}

void ImguiSystem::SetLockBool(bool debug) {

    b_lock_entity = debug;
}

Camera* ImguiSystem::GetCamera() {

    auto it = cam_arr_->begin();
    if (it != cam_arr_->end())
        return it->second;
    else
        return nullptr;
}

Entity* ImguiSystem::GetEntity() {

    return selected_entity_;
}

void ImguiSystem::SetEntity(Entity* newentity) {
	
    selected_entity_ = newentity;
}

Camera* ImguiSystem::GetExistingSceneCamera() {

    return camera_;
}

std::string ImguiSystem::GetAssetAdd() {

    return img_to_add_;
}

void ImguiSystem::SetAssetAdd(std::string image) {

    img_to_add_ = image;
}

std::string ImguiSystem::GetArchetypePath() {

    return archetype_path_;
}

void ImguiSystem::SetArchetypePath(std::string new_path) {

    archetype_path_ = new_path;
}

void ImguiSystem::SaveCheckPopUp(const char* window_name, int exit_type) {

    SetPopupPosition();

    if (ImGui::BeginPopupModal(window_name, NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::TextColored(REDDEFAULT, "Have you save whatever you've been working on ?");
        ImGui::Text("There's no turning back if you leave !");

        if (ImGui::Button("Yep I Did")) {

            if (exit_type == CloseApp::CREATENEWSCENE) { // create new scene

                selected_entity_ = {};
                type = CloseApp::NONE;
                NewScene();
                sound_->StopSound("All", true);
                ImGui::CloseCurrentPopup();
            }
            if (exit_type == CloseApp::RETURNMENU) { // return to menu
                b_imgui_mode = false;
                FACTORY->DestroyAllEntities();
                selected_entity_ = {};
                CORE->ResetGodMode();
                type = CloseApp::NONE;
                CORE->GetSystem<Game>()->ChangeState(&m_MenuState);
                ImGui::CloseCurrentPopup();
            }
            else if (exit_type == CloseApp::EXITAPP){ // exit the app

                type = CloseApp::NONE;
                CORE->SetGameActiveStatus(false);
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::SameLine(0, 10);

        if (ImGui::Button("Nope"))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
}

const char* ImguiSystem::GetSceneFilter() { 

    return scene_filter_;
}

const char* ImguiSystem::GetGenericFilter() { 

    return generic_filter_; 
}

void ImguiSystem::LoadJsonPaths(std::string path) {

    rapidjson::Document doc;
    DeSerializeJSON(path, doc);

    const rapidjson::Value& files_arr = doc;
    DEBUG_ASSERT(files_arr.IsObject(), "Level JSON does not exist in proper format");
    
    for (rapidjson::Value::ConstMemberIterator it = files_arr.MemberBegin(); it != files_arr.MemberEnd(); ++it) {

        std::string archetype_name{ it->name.GetString() };
        std::string filepath{ it->value.GetString() };

        editor_->AddNewEntityPath(archetype_name, filepath);
    }
}

void ImguiSystem::SaveArchetype(std::string path) {

    if (!path.empty())
        factory_->SerializeArchetypes(path);
}

void ImguiSystem::LoadArchetype() {

    std::string path = OpenSaveDialog(scene_filter_, 0);

    if (!path.empty()) {

        std::string file = EditString(path);

        factory_->DestroyAllArchetypes();
        factory_->CreateAllArchetypes(file);

        archetype_path_ = path;
    }
    selected_entity_ = {};
}

void ImguiSystem::OpenFile() {

    std::string path = OpenSaveDialog(scene_filter_, 0);

    if (!path.empty())
    {
        std::string file = EditString(path);
        std::string layertoload = {};

        factory_->DestroyAllEntities();

        if (CheckString(file, "Level")) {

            layertoload = "Play";
        }
        else if (CheckString(file, "Menu"))
            layertoload = "Menu";

        CORE->GetManager<LayerManager>()->LoadLevelLayers(layertoload);
        factory_->DeSerializeLevelEntities(file);
        CORE->GetManager<AMap>()->InitAMap(CORE->GetManager<EntityManager>()->GetEntities());
        CORE->GetSystem<PartitioningSystem>()->InitPartition();

        current_loaded_path_ = path;
    }
}

void ImguiSystem::SaveFile(std::string dest_path){

    factory_->SerializeCurrentLevelEntities(); // save each entity to their respective path

    rapidjson::StringBuffer sb;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);

    if (!dest_path.empty()) {

        std::ofstream filestream(dest_path);

        if (filestream.is_open()) {

            writer.StartObject();

            for (Level::EntityPathsIt it = editor_->entity_paths_.begin(); it != editor_->entity_paths_.end(); ++it) {

                writer.Key(it->first.c_str());
                writer.String(it->second.c_str());
            }

            writer.EndObject();

            filestream << sb.GetString();
        }

        filestream.close();
    }
}

void ImguiSystem::NewScene() {

    factory_->DestroyAllEntities();
    factory_->DeSerializeLevelEntities("Resources/EntityConfig/editor.json");
}

std::string ImguiSystem::OpenSaveDialog(const char* filter, int save, int multiselect) {
    OPENFILENAMEA ofn;
    CHAR szFile[2048] = { 0 };
    std::string data = {};
    // init OPENFILENAME
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = glfwGetWin32Window(win_->ptr_window);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;

    if (save)
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_NOCHANGEDIR;
    if (!save && !multiselect)
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR;
    if (!save && multiselect)
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR;

    if (!save) {

        if (GetOpenFileNameA(&ofn) == TRUE) {

            size_t len = strlen(ofn.lpstrFile);

            if (ofn.lpstrFile[len + 1] == 0)
                return ofn.lpstrFile;
            else
            {
                data = ofn.lpstrFile;
                ofn.lpstrFile += len + 1;

                while (ofn.lpstrFile[0]) {

                    data = data + "|" + ofn.lpstrFile;

                    len = strlen(ofn.lpstrFile);
                    ofn.lpstrFile += len + 1;
                }
                return data;
            }
        }
    }
    else {

        if (GetSaveFileNameA(&ofn) == TRUE) {

            if (!CheckString(ofn.lpstrFile, ".json"))
                return ofn.lpstrFile + std::string(".json");
            else
                return ofn.lpstrFile;
        }
    }

    return std::string(); // returns an empty string if user cancels/didnt select anything
}

void ImguiSystem::DrawGrid() {

    float global_scale = CORE->GetGlobalScale();
    float cam_zoom = *camera_->GetCameraZoom();
    Vector2D cam_pos = CORE->GetSystem<CameraSystem>()->GetMainCameraPos() * global_scale;

    float grid_spacing = global_scale * std::max( 1, 4 * static_cast<int>( 0.5f / cam_zoom));

    std::vector<std::pair<glm::vec2, glm::vec2>> points;

   for (float i = -global_scale * 100; i < global_scale * 100; i += grid_spacing) {

       for (float j = -global_scale * 100; j < global_scale * 100; j += grid_spacing) {

            Vector2D scaled_window_size{ win_->GetWinWidth() / cam_zoom, win_->GetWinHeight() / cam_zoom };

            Vector2D bottom_left_edge = cam_pos - scaled_window_size;
            Vector2D top_right_edge = cam_pos + scaled_window_size;

            if (i < bottom_left_edge.x || i > top_right_edge.x ||
                j < bottom_left_edge.y || j > top_right_edge.y) {
                continue;
            }

            points.push_back({ { i, -global_scale * 100 }, { i, global_scale * 100 } });
            points.push_back({ { -global_scale * 100, j }, { global_scale * 100, j } });

            if (points.size() == graphics_->GetBatchSize()) {
                graphics_->DrawDebugLines(points, { 0.8f, 0.8f, 0.8f, 0.5f }, 1.0f);
                points.clear();
            }
       }
   }

   if (points.size() > 0) {
       graphics_->DrawDebugLines(points, { 1.0f, 1.0f, 1.0f, 0.5f }, 1.0f);
   }
}

void ImguiSystem::PopUpMessage(const char* windowName, const char* message) {

    SetPopupPosition();

    if (ImGui::BeginPopupModal(windowName, NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::Text(message);

        if (ImGui::Button("OK"))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
}

void ImguiSystem::DeletePopUp(const char* windowName, std::string objName, Entity* entity, std::shared_ptr<Component> component) {

    std::string warning = {};

    if (entity) {
        
        std::shared_ptr<Name> entityname = std::dynamic_pointer_cast<Name>(entity->GetComponent(ComponentTypes::NAME));
        warning = objName + " from " + entityname->GetName();
    }

    SetPopupPosition();

    if (ImGui::BeginPopupModal(windowName, NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f , 1.0f }, "Deleting: ");
        if (!entity)
            ImGui::Text(objName.c_str());
        else
            ImGui::Text(warning.c_str());

        ImGui::TextColored(ImVec4{ 0.863f, 0.078f, 0.235f , 1.0f }, "This cannot be undone");

        ImGui::Separator();

        CustomImGuiButton((ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f), (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f), (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));

        if (ImGui::Button("OK")) {
            if (!selected_entity_->GetID() && !entity) {
                entities_->DeleteArchetype(selected_entity_); //delete archetype
                selected_entity_ = nullptr;
            }
            else if (selected_entity_->GetID() && !entity) {
                entities_->DeleteEntity((selected_entity_)); //delete entities
                selected_entity_ = nullptr;
            }
            else if (!selected_entity_->GetID() && entity)
                entity->RemoveComponent(component); // delete component from archetype

            
            ImGui::CloseCurrentPopup();
        }

        ImGui::PopStyleColor(3);
        ImGui::SameLine(0, 4);

        if (ImGui::Button("Cancel"))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
}

std::string ImguiSystem::EditString(std::string filepath, const char* startpos) {

    return filepath.substr(filepath.find(startpos));
}

bool ImguiSystem::EditorMode() {

    if (CORE->GetSystem<Game>()->GetStateName() == "Editor")
        return ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow) || ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
    else
        return false;
}

void ImguiSystem::CustomImGuiButton(ImVec4 ButtonCol, ImVec4 HoveredCol, ImVec4 SelectCol) {

    ImGui::PushStyleColor(ImGuiCol_Button, ButtonCol);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HoveredCol);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, SelectCol);
}

void ImguiSystem::ImguiHelp(const char* description, int symbol) {

    if (symbol)
        ImGui::TextDisabled(ICON_FA_EXCLAMATION_CIRCLE);

    if (ImGui::IsItemHovered()) {

        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(description);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

bool ImguiSystem::CheckString(std::string path, const char* key) {

    if (path.find(key) != path.npos)
        return true;
    return false;
}

void ImguiSystem::SetPopupPosition() {

    ImVec2 centre = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(centre, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
}

void ImguiSystem::Popups() {

    if (b_level_save)
        ImGui::OpenPopup("Save Confirmation");

    PopUpMessage("Save Confirmation", "Level Json & the individual Level Entity(ies) have been saved");
    b_level_save = false;

    if (b_add_path)
        ImGui::OpenPopup("No Path Set");

    PopUpMessage("No Path Set", "Individual Entity save path has NOT been set\n'Archetype' >> 'Set Entity Path'");
    b_add_path = false;

    if (b_close_confirm)
        ImGui::OpenPopup("Exit Confirmation");

    SaveCheckPopUp("Exit Confirmation", type);
    b_close_confirm = false;

    if (b_entity_save)
        ImGui::OpenPopup("Individual Entity(ies) Saved");
    PopUpMessage("Individual Entity(ies) Saved", "Level Entities have been saved \ninto the respective json files.\nUpdate the Level Json File if needed");
    b_entity_save = false;
}

std::string ImguiSystem::VisibleIcon(bool windowbool) {

    if (windowbool)
        return ICON_FA_EYE;
    else
        return ICON_FA_EYE_SLASH;
}

ImguiSystem::~ImguiSystem() {
	
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImguiSystem::Draw() {}
void ImguiSystem::SendMessageD(Message* m) { (void)m; }
