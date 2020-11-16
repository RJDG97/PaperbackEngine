#include "Systems/ImguiSystem.h"
#include "Manager/Amap.h"
#include "ImguiWindows/AssetFileSystem.h"
#include "ImguiWindows/ImguiViewport.h"
#include "ImguiWindows/EntityWindow.h"
#include "ImguiWindows/EntityPathWindow.h"
#include "ImguiWindows/ArchetypeWindow.h"
#include "ImguiWindows/SystemWindow.h"
#include "ImguiWindows/AssetWindow.h"


// Expose the Win32 API
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

void ImguiSystem::Init(){
    // Adding window to Imgui's Window map
    //AddWindow<ImguiViewport>();
    AddWindow<AssetWindow>();
    AddWindow<EntityWindow>();
    AddWindow<ArchetypeWindow>();
    AddWindow<EntityPathWindow>();
    AddWindow<SystemWindow>();

    win_ = &*CORE->GetSystem<WindowsSystem>();
    collision_system_ = &*CORE->GetSystem<Collision>();
    input_sys_ = &*CORE->GetSystem<InputSystem>();
    entities_ = &*CORE->GetManager<EntityManager>();
    factory_ = &*CORE->GetSystem <EntityFactory>();

    // Setup Dear ImGui context
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
    img_font_ = io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FA, 14.0f, &icons_config, icon_ranges);
    bold_font_ = io.Fonts->AddFontFromFileTTF("Resources/Font/Grandstander-Bold.ttf", 16.0f);
    // end of Imgui init

    b_dock_space_open = true;
    b_fullscreen_persistant = true;
    b_fullscreen = b_fullscreen_persistant;

    dock_space_flags_ = ImGuiDockNodeFlags_PassthruCentralNode;
    window_flags_ = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    b_windows = true;
    b_lock_entity = false;
    b_imguimode = false;

    //Imgui Window Bools
    b_entitywin = true;
    b_archetypewin = true;
    b_component = true;
    b_display = false;
    b_editpath = false;
    b_showpop = false;
    b_asset = true;

    new_entity_ = nullptr;

    scene_filter_ =
    "(*.json) Paperback Engine Scene\0*.json\0"
    "(*.*) All Files\0* *.*\0";

    texture_filter_ =    
    "(*.jpg) JPG\0* .jpg\0"
    "(*.png) Spritesheets/Textures\0* .png\0"
    "(*.*) All Files\0* *.*\0";

    //std::vector<File::fs::directory_entry> tempList;

    //for (auto& directory : File::RecursiveDirectoryList("Resources")) {
    //    tempList.push_back(directory);
    //    std::cout << directory.path().generic_string() << std::endl;
    //}
    //
    //for (auto& directory : tempList)
    //    for (auto& file : File::ListOfFiles(directory))
    //        directory_map_[directory.path().generic_string()].push_back(file);

}

void ImguiSystem::Update(float frametime) {

    UNREFERENCED_PARAMETER(frametime);
	
    if (b_imguimode) {

        ImguiInput();
    	
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

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

            if (b_showpop)
                ImGui::OpenPopup("Save Confirmation");

            PopUpMessage("Save Confirmation", "Level Entities have been saved \ninto the respective json files");
            b_showpop = false;

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
            if (ImGui::MenuItem(ICON_FA_FOLDER_OPEN " Open Scene", "Ctrl+O"))
                OpenFile();
            if (ImGui::MenuItem(ICON_FA_FLOPPY_O " Save", "Ctrl+S")) {
                SaveFile();
                b_showpop = true;
            }

            if (ImGui::MenuItem(ICON_FA_TIMES " Create New Scene"))
                NewScene();

            ImGui::Separator();
            if (ImGui::MenuItem(ICON_FA_REPLY " Return to Menu")){

                b_imguimode = false;
                FACTORY->DestroyAllEntities();
                new_entity_ = {};
                CORE->GetSystem<Game>()->ChangeState(&m_MenuState);
            }
            if (ImGui::MenuItem(ICON_FA_POWER_OFF " Exit"))
                CORE->SetGameActiveStatus(false);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Archetypes")) {

            if (ImGui::MenuItem(ICON_FA_FLOPPY_O " Save Archetypes"))
                SaveArchetype();
            if (ImGui::MenuItem(ICON_FA_UPLOAD " Load Archetypes"))
                LoadArchetype();
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_FA_PENCIL_SQUARE_O " Edit Archetype Path"))
                b_editpath = true;

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(ICON_FA_WINDOW_RESTORE " ImGui Windows")) {

            ImGui::Checkbox("Toggle Scene Hierachy", &b_entitywin);
            ImGui::Checkbox("Toggle Component Viewer", &b_component);
            ImGui::Checkbox("Toggle Archetype Window", &b_archetypewin);
            ImGui::Checkbox("Toggle Asset Browser", & b_asset);
            ImGui::Checkbox("See System Performance", &b_display);

            ImGui::EndMenu();
        }
        ImGui::PopFont();
    }
    ImGui::EndMenuBar();
}


void ImguiSystem::CustomImGuiButton(ImVec4 ButtonCol, ImVec4 HoveredCol, ImVec4 SelectCol) {

    ImGui::PushStyleColor(ImGuiCol_Button, ButtonCol);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HoveredCol);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, SelectCol);
}

bool ImguiSystem::GetImguiBool() {

    return b_imguimode;
}

void ImguiSystem::SetImguiBool(bool mode) {

    b_imguimode = mode;
}

std::string ImguiSystem::GetName() {
	
    return "ImGui System";
}

EntityID ImguiSystem::GetSelectedEntity() {

    return selected_entity_;
}

void ImguiSystem::ResetSelectedEntity() {

    new_entity_ = {};
    b_lock_entity = false;
}

bool ImguiSystem::GetLockBool() {

    return b_lock_entity;
}

void ImguiSystem::SetLockBool(bool debug) {

    b_lock_entity = debug;
}

Entity* ImguiSystem::GetEntity() {

    return new_entity_;
}

void ImguiSystem::SetEntity(Entity* newentity) {
	
    new_entity_ = newentity;
}

void ImguiSystem::ImguiInput() {

	bool control = ImGui::IsKeyPressed(GLFW_KEY_LEFT_CONTROL) || ImGui::IsKeyPressed(GLFW_KEY_RIGHT_CONTROL);
	bool shift = ImGui::IsKeyPressed(GLFW_KEY_LEFT_SHIFT) || ImGui::IsKeyPressed(GLFW_KEY_RIGHT_SHIFT);

	if (control) {
		
		if (ImGui::IsKeyReleased(GLFW_KEY_O))
            OpenFile();
        if (shift && ImGui::IsKeyPressed(GLFW_KEY_S))
            SaveFile();
		if (ImGui::IsKeyPressed(GLFW_KEY_N))
            NewScene();
	}
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
    }
}

void ImguiSystem::SaveFile(){

    factory_->SerializeCurrentLevelEntities();
}

void ImguiSystem::NewScene() {

    factory_->DestroyAllEntities();
    factory_->DeSerializeLevelEntities("Resources/EntityConfig/editor.json");
}

std::string ImguiSystem::OpenSaveDialog(const char* filter, int save) {
    OPENFILENAMEA ofn;
    CHAR szFile[260] = { 0 };

    // init OPENFILENAME
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = glfwGetWin32Window(win_->ptr_window);
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filter;
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
    if (!save) {
        if (GetOpenFileNameA(&ofn) == TRUE)
            return ofn.lpstrFile;
    }
    else {
        if (GetSaveFileNameA(&ofn) == TRUE)
            return ofn.lpstrFile;
    }

    return std::string(); // returns an empty string if user cancels/didnt select anything
}

std::string ImguiSystem::EditString(std::string filepath, const char* startpos) {
    return filepath.substr(filepath.find(startpos));
}

void ImguiSystem::SendMessageD(Message* m) {
    switch (m->message_id_) {
    case MessageIDTypes::M_MOUSE_PRESS:
    {
        if (!b_lock_entity) {
            selected_entity_ = collision_system_->SelectEntity();
            new_entity_ = entities_->GetEntity(selected_entity_);
            b_lock_entity = true;
        }
        break;
    }
    default:
        break;
    }
}

void ImguiSystem::PopUpMessage(const char* windowName, const char* message) {
    ImVec2 centre = ImGui::GetMainViewport()->GetCenter();

    ImGui::SetNextWindowPos(centre, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopup(windowName)) {

        ImGui::Text(message);

        if (ImGui::Button("OK"))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }
}

void ImguiSystem::DeletePopUp(const char* windowName, std::string objName, Entity* entity, std::shared_ptr<Component> component) {

    ImVec2 centre = ImGui::GetMainViewport()->GetCenter();
    std::string warning;

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
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0 / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0 / 7.0f, 0.7f, 0.7f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0 / 7.0f, 0.8f, 0.8f));

        if (ImGui::Button("OK")) {
            if (!new_entity_->GetID() && !entity) //archetype id == 0
                entities_->DeleteArchetype(new_entity_); //delete archetype
            else if (new_entity_->GetID() && !entity)
                entities_->DeleteEntity((new_entity_)); //delete entities
            else if (!new_entity_->GetID() && entity) // entity: to detect if deleting component & check if its an entity/archetype
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

void ImguiSystem::ImguiHelp(const char* description) {

    ImGui::TextDisabled("?");
    if (ImGui::IsItemHovered()) {

        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(description);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

ImguiSystem::~ImguiSystem() {
	
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImguiSystem::Draw() {}