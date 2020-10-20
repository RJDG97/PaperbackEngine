#include "Systems/ImguiSystem.h"
#include "Imgui/ImguiWindow.h"
#include "Imgui/AnotherWindow.h"
#include "Engine/Core.h"

void ImguiSystem::Init(){
    // Adding window to Imgui's Window map
    AddWindow<ImguiWindow>();
    AddWindow<AnotherWindow>();

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking (Merging of windows)
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

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
    
    win = &*CORE->GetSystem<WindowsSystem>();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(win->ptr_window, true);
    ImGui_ImplOpenGL3_Init(NULL);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.

}

void ImguiSystem::Update(float frametime){

    UNREFERENCED_PARAMETER(frametime);

    ImGuiIO& io = ImGui::GetIO();

    if (b_imguimode)
    {
       // glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // For all windows attached, 
        for (WindowIt begin = imgui_window_arr_.begin(); begin != imgui_window_arr_.end(); ++begin) {
            begin->second->Update();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(win->ptr_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        //glClearColor(clearcolor_.x, clearcolor_.y, clearcolor_.z, clearcolor_.w);
        // glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        //glfwSwapBuffers(win->ptr_window);
    }
}

void ImguiSystem::Draw(){
}

std::string ImguiSystem::GetName(){
	return std::string();
}

void ImguiSystem::SendMessageD(Message* m){
    UNREFERENCED_PARAMETER(m);
}

ImguiSystem::~ImguiSystem(){
    //CleanUp
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
