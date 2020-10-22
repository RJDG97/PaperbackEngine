#include "ImguiWindows/ImguiWindow.h"

void ImguiWindow::Init() {
    b_showdemo = true;
    b_showanother = false;
    clearcolor_ = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
}

void ImguiWindow::Update() {

    if (b_showdemo)
        ImGui::ShowDemoWindow(&b_showdemo);
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &b_showdemo);            // Edit bools storing our window open/close state
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
}