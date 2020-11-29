#include "Systems/ImguiSystem.h"
#include "Manager/Amap.h"

#include "ImguiWindows/ImguiViewport.h"
#include "ImguiWindows/EntityWindow.h"
#include "ImguiWindows/EntityPathWindow.h"
#include "ImguiWindows/ArchetypeWindow.h"
#include "ImguiWindows/SystemWindow.h"
#include "ImguiWindows/AssetWindow.h"
#include "ImguiWindows/AssetConsoleWindow.h"
#include "ImguiWindows/TextureTilesWindow.h"

#include "GameStates/MenuState.h"

// Expose the Win32 API
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

void ImguiSystem::Init(){
    // Adding window to Imgui's Window map
    //AddWindow<ImguiViewport>();
    AddWindow<ArchetypeWindow>();
    AddWindow<EntityWindow>();
    AddWindow<EntityPathWindow>();
    AddWindow<AssetWindow>();
    AddWindow<AssetConsoleWindow>();
    AddWindow<TextureTilesWindow>();
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

    //Imgui Window Bools
    b_archetypewin = true;
    b_entitywin = true;
    b_component = true;
    b_display = false;
    b_editpath = false;
    b_show_pop = false;
    b_asset = false;
    b_editcomp = false;
    b_addtexture = false;
    b_showtex = false;
    b_add_path = false;

    b_lock_entity = false;
    b_imgui_mode = false;
    b_save_check = false;

    selected_entity_ = nullptr;

    img_to_add_ = {};

    current_loaded_path_ = {};

    scene_filter_ =
        "(*.json) Paperback Engine Scene\0*.json\0";
       // "(*.*) All Files\0* *.*\0";

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
    // end of Imgui init

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

            if (b_show_pop)
                ImGui::OpenPopup("Save Confirmation");

            PopUpMessage("Save Confirmation", "Level Entities have been saved \ninto the respective json files");
            b_show_pop = false;

            if (b_add_path)
                ImGui::OpenPopup("No Path Set");
            PopUpMessage("No Path Set", "No Entity save path has been set\n'Archetype' >> 'Set Entity Path'");
            b_add_path = false;
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

            for (WindowIt begin = imgui_window_arr_.begin(); begin != imgui_window_arr_.end(); ++begin)
                begin->second->Update();

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

    colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.45f);
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
            if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open Scene")) {
                selected_entity_ = {};
                OpenFile();
            }

            if (ImGui::MenuItem(ICON_FA_SAVE " Save")) {

                if (!editor_->entity_paths_.empty()) {
                    SaveFile(current_loaded_path_);
                    b_show_pop = true;
                }
                else
                    b_add_path = true;
            }

            if (ImGui::MenuItem("Save Scene As...")) {
                if (!editor_->entity_paths_.empty()) {

                    std::string path = OpenSaveDialog(scene_filter_, 1);
                    SaveFile(path);

                    b_show_pop = true;
                }
                else
                    b_add_path = true;
            }

            if (ImGui::MenuItem(ICON_FA_TIMES " Create New Scene")) {
                selected_entity_ = {};
                NewScene();
            }

            ImGui::Separator();

            if (ImGui::MenuItem(ICON_FA_REPLY " Return to Menu")){

                b_imgui_mode = false;
                FACTORY->DestroyAllEntities();
                selected_entity_ = {};
                CORE->GetSystem<Game>()->ChangeState(&m_MenuState);

            }
            if (ImGui::IsItemHovered())
                ImguiHelp("Have you save the current\nworking scene. There is no auto recovery", 0);

            if (ImGui::MenuItem(ICON_FA_POWER_OFF " Exit"))
                CORE->SetGameActiveStatus(false);

            if (ImGui::IsItemHovered())
                ImguiHelp("Have you save the current\nworking scene. There is no auto recovery", 0);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Archetypes")) {

            if (ImGui::MenuItem(ICON_FA_SAVE " Save Archetypes"))
                SaveArchetype();
            if (ImGui::MenuItem(ICON_FA_FILE_IMPORT " Load Archetypes"))
                LoadArchetype();
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_FA_EDIT " Edit Archetype Path"))
                b_editpath = true;

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(ICON_FA_WINDOW_RESTORE " ImGui Windows")) {

            ImGui::Checkbox("Toggle Scene Hierachy", &b_entitywin);
            ImGui::Checkbox("Toggle Component Viewer", &b_component);
            ImGui::Checkbox("Toggle Archetype Window", &b_archetypewin);
            ImGui::Separator();
            ImGui::Checkbox("Toggle Asset Browser", & b_asset);
            ImGui::Checkbox("Toggle Asset Console Window", &b_addtexture);
            ImGui::Checkbox("Toggle Texture Window", &b_showtex);
            ImGui::Separator();
            ImGui::Checkbox("See System Performance", &b_display);

            ImGui::EndMenu();
        }

        // placeholder
        if (ImGui::BeginMenu(ICON_FA_COGS " Control Panel")) {

            if (ImGui::TreeNode(ICON_FA_MUSIC " Audio Settings")) {

                VolumeControl();

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Vignette Size Settings")) {

                ImVec2 inputsize = { graphics_->GetVignetteSize().x, graphics_->GetVignetteSize().y };

                float lineHeight = bold_font_->FontSize + ImGui::GetStyle().FramePadding.y * 2.0f;
                ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

                CustomImGuiButton(REDDEFAULT, REDHOVERED, REDACTIVE);
                ImGui::PushFont(bold_font_);
                if (ImGui::Button("X", buttonSize))
                    inputsize.x = 600.0f;

                ImGui::PopStyleColor(3);
                ImGui::PushItemWidth(150.0f);
                ImGui::DragFloat("##Xsize", &inputsize.x, 0.1f, 0.0f, 0.0f);
                ImGui::PopItemWidth();
                ImGui::SameLine(0, 3);

                CustomImGuiButton(GREENDEFAULT, GREENHOVERED, GREENACTIVE);
                if (ImGui::Button("Y", buttonSize))
                    inputsize.y = 320.0f;

                ImGui::PopStyleColor(3);
                ImGui::PushItemWidth(150.0f);
                ImGui::DragFloat("##Ysize", &inputsize.y, 0.1f, 0.0f, 0.0f);

                ImGui::PopItemWidth();
                ImGui::PopFont();
                
                graphics_->SetVignetteSize({ inputsize.x, inputsize.y });

                ImGui::TreePop();
            }

            ImGui::EndMenu();
        }

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

void ImguiSystem::SaveArchetype() {
    std::string path = OpenSaveDialog(scene_filter_, 1);
    if (!path.empty())
        factory_->SerializeArchetypes(path);
}

void ImguiSystem::LoadArchetype() {

    std::string path = OpenSaveDialog(scene_filter_, 0);
    if (!path.empty()) {

        std::string file = EditString(path);

        factory_->DestroyAllArchetypes();
        factory_->CreateAllArchetypes(file);
    }
}

void ImguiSystem::OpenFile() {
    std::string path = OpenSaveDialog(scene_filter_, 0);

    if (!path.empty())
    {
        std::string file = EditString(path);

        factory_->DestroyAllEntities();
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

        if (GetSaveFileNameA(&ofn) == TRUE)
            return ofn.lpstrFile;
    }

    return std::string(); // returns an empty string if user cancels/didnt select anything
}

void ImguiSystem::DrawGrid() {

    float global_scale = CORE->GetGlobalScale();
    float cam_zoom = *camera_->GetCameraZoom();

    float grid_spacing = global_scale * std::max( 1, 4 * static_cast<int>( 0.5f / cam_zoom));

    std::vector<std::pair<glm::vec2, glm::vec2>> points;

   for (float i = -global_scale * 100; i < global_scale * 100; i += grid_spacing) {

       for (float j = -global_scale * 100; j < global_scale * 100; j += grid_spacing) {

            Vector2D scaled_window_size{ win_->GetWinWidth() / cam_zoom, win_->GetWinHeight() / cam_zoom };

            Vector2D bottom_left_edge = camera_->GetVector2DCameraPosition() - scaled_window_size;
            Vector2D top_right_edge = camera_->GetVector2DCameraPosition() + scaled_window_size;

            if (i < bottom_left_edge.x || i > top_right_edge.x ||
                j < bottom_left_edge.y || j > top_right_edge.y)
            {
                continue;
            }

            points.push_back({ { i, -global_scale * 100 }, { i, global_scale * 100 } });
            points.push_back({ { -global_scale * 100, j }, { global_scale * 100, j } });

            if (points.size() == graphics_->GetBatchSize())
            {
                graphics_->DrawDebugLines(points, { 0.8f, 0.8f, 0.8f, 0.5f }, 1.0f);
                points.clear();
            }
       }
   }

   if (points.size() > 0)
   {
       graphics_->DrawDebugLines(points, { 1.0f, 1.0f, 1.0f, 0.5f }, 1.0f);
   }
}

void ImguiSystem::PopUpMessage(const char* windowName, const char* message) {

    ImVec2 centre = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(centre, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal(windowName, NULL, ImGuiWindowFlags_AlwaysAutoResize)) {

        ImGui::Text(message);

        if (ImGui::Button("OK")) {

            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void ImguiSystem::DeletePopUp(const char* windowName, std::string objName, Entity* entity, std::shared_ptr<Component> component) {

    ImVec2 centre = ImGui::GetMainViewport()->GetCenter();
    std::string warning = {};

    if (entity) {
        std::shared_ptr<Name> entityname = std::dynamic_pointer_cast<Name>(entity->GetComponent(ComponentTypes::NAME));
        warning = objName + " from " + entityname->GetName();
    }

    ImGui::SetNextWindowPos(centre, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
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
            if (!selected_entity_->GetID() && !entity) 
                entities_->DeleteArchetype(selected_entity_); //delete archetype
            else if (selected_entity_->GetID() && !entity)
                entities_->DeleteEntity((selected_entity_)); //delete entities
            else if (!selected_entity_->GetID() && entity)
                entity->RemoveComponent(component); // delete component from archetype

            SetEntity(nullptr);
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

void ImguiSystem::VolumeControl() {

    if (ImGui::Button(ICON_FA_PLAY ICON_FA_PAUSE " Play/Pause Sound"))
        sound_->PauseSound("all", true, true);
    if (ImGui::Button(ICON_FA_VOLUME_MUTE " Mute Sound"))
        sound_->MuteSound("all", true, true);

    ImGui::Separator();

    float inputVol = sound_->GetVolume();
    ImGui::Text("Volume:");
    ImGui::Text(ICON_FA_VOLUME_DOWN); ImGui::SameLine(0, 3);
    ImGui::PushItemWidth(250.0f);
    ImGui::SliderFloat("", &inputVol, 0.0f, 1.0f, "%.2f");
    ImGui::PopItemWidth(); ImGui::SameLine(0, 3);
    ImGui::Text(ICON_FA_VOLUME_UP);
    sound_->SetVolume(inputVol);
}

ImguiSystem::~ImguiSystem() {
	
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImguiSystem::Draw() {}
void ImguiSystem::SendMessageD(Message* m) { (void)m; }
