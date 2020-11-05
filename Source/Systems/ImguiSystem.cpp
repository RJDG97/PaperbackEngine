#include "Systems/ImguiSystem.h"
#include "ImguiWindows/ImguiViewport.h"
#include "ImguiWindows/EntityWindow.h"
#include "ImguiWindows/ImguiMenuBar.h"
#include "ImguiWindows/ArchetypeWindow.h"
#include "Systems/Game.h"

void ImguiSystem::Init(){
    // Adding window to Imgui's Window map
    //AddWindow<ImguiViewport>();
    AddWindow<ImguiMenuBar>();
    AddWindow<EntityWindow>();
    AddWindow<ArchetypeWindow>();

    win = &*CORE->GetSystem<WindowsSystem>();
    collision_system_ = &*CORE->GetSystem<Collision>();
    //input_sys_ = &*CORE->GetSystem<InputSystem>();
    entities_ = &*CORE->GetManager<EntityManager>();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking (Merging of windows)
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    
    //io.WantCaptureKeyboard = true;
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    //check = false;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Initialize Imgui Window that is attached to the Imgui System
    for (WindowIt begin = imgui_window_arr_.begin(); begin != imgui_window_arr_.end(); ++begin) {
        begin->second->Init();
    }
   
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(win->ptr_window, true);
    ImGui_ImplOpenGL3_Init(NULL);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.

    // end of Imgui init

    b_dock_space_open = true;
    b_fullscreen_persistant = true;
    b_fullscreen = b_fullscreen_persistant;

    dock_space_flags_ = ImGuiDockNodeFlags_PassthruCentralNode;
    window_flags_ = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    b_debug = false;
    b_lock_entity = false;
    b_imguimode = false;

    new_entity_ = nullptr;
}

void ImguiSystem::Update(float frametime){

    UNREFERENCED_PARAMETER(frametime);

    if (b_imguimode){
       //glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiIO& io = ImGui::GetIO();
        if (b_dock_space_open)
        {
            DockSpaceFlagSet();

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::SetNextWindowBgAlpha(0.0f); // set the transparency of the docking central node
            ImGui::Begin("DockSpace", &b_dock_space_open, window_flags_);
            ImGui::PopStyleVar();

            if (b_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {

                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dock_space_flags_);
            }

            // For all windows attached.
            /* Has to be called between the ImGui::Begin("DockSpace"); and the corresponding ImGui::End()
             for windows to be dockable in the docking space */

            for (WindowIt begin = imgui_window_arr_.begin(); begin != imgui_window_arr_.end(); ++begin) {
                begin->second->Update();
            }

            ImGui::End(); // end of docking space
        }
        else {

            for (WindowIt begin = imgui_window_arr_.begin(); begin != imgui_window_arr_.end(); ++begin) {

                begin->second->Update();
            }
        }

        ImguiRender();
    }
}

void ImguiSystem::DockSpaceFlagSet() {

    b_fullscreen = b_fullscreen_persistant;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
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

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dock_space_flags_ & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags_ |= ImGuiWindowFlags_NoBackground;
}

void ImguiSystem::ImguiRender()
{
    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(win->ptr_window, &display_w, &display_h);
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

void ImguiSystem::Draw(){
}

std::string ImguiSystem::GetName(){

    return "ImGui System";
}

void ImguiSystem::SendMessageD(Message* m){
    switch (m->message_id_) {
    case MessageIDTypes::M_MOUSE_PRESS:
    {
        if (!b_lock_entity){
            selected_entity_ = collision_system_->SelectEntity();
            new_entity_ = entities_->GetEntity(selected_entity_);
            b_lock_entity = true;
        }
        break;
    }
    case MessageIDTypes::DEBUG_ALL:
    {
        if (CORE->GetSystem<Game>()->GetStateName() == "Play" || CORE->GetSystem<Game>()->GetStateName() == "Menu") {
            b_debug = !b_debug;
            b_imguimode = !b_imguimode;
        }
        break;
    }
    default:
        break;
    }
}

EntityID ImguiSystem::GetSelectedEntity(){

    return selected_entity_;
}

void ImguiSystem::ResetSelectedEntity(){
    new_entity_ = nullptr;
    b_lock_entity = false;
}

bool ImguiSystem::GetDebugBool(){
    return b_debug;
}

void ImguiSystem::SetDebugBool(bool debug){
    b_debug = debug;
}

bool ImguiSystem::GetLockBool(){
    return b_lock_entity;
}

void ImguiSystem::SetLockBool(bool debug){

    b_lock_entity = debug;
}

void ImguiSystem::ImguiHelp(const char* description){

    ImGui::TextDisabled("?");
    if (ImGui::IsItemHovered()){

        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(description);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

Entity* ImguiSystem::GetEntity()
{
    return new_entity_;
}

void ImguiSystem::SetEntity(Entity* newentity)
{
    new_entity_ = newentity;
}

ImguiSystem::~ImguiSystem(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
