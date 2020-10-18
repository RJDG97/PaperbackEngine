#include "Systems/ImguiSystem.h"
#include "Systems/WindowsSystem.h"
#include "Engine/Core.h"

void ImguiSystem::Init()
{
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

    ImGuiStyle& style = ImGui::GetStyle();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    WindowsSystem* win = CORE->GetSystem<WindowsSystem>();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(win->ptr_window, true);
    ImGui_ImplOpenGL3_Init(NULL);

}

void ImguiSystem::Update(float frametime)
{
    WindowsSystem* win = CORE->GetSystem<WindowsSystem>();
    ImGuiIO& io = ImGui::GetIO();

    if (b_imguimode)
    //while (!glfwWindowShouldClose(win->ptr_window))
    {
       // glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (b_showdemo)
            ImGui::ShowDemoWindow(&b_showdemo);
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &b_showdemo);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &b_showanother);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clearcolor_); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        if (b_showanother)
        {
            ImGui::Begin("Another Window", &b_showanother);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                b_showanother = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(win->ptr_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        //glClearColor(clearcolor_.x, clearcolor_.y, clearcolor_.z, clearcolor_.w);
       // glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

void ImguiSystem::Draw()
{
}

std::string ImguiSystem::GetName()
{
	return std::string();
}

void ImguiSystem::SendMessageD(Message* m)
{
}

ImguiSystem::~ImguiSystem()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
