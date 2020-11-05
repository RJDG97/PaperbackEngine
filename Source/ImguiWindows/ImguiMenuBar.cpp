#include "ImguiWindows/ImguiMenuBar.h"
#include "Engine/Core.h"

// expose the win32 API for the open/save dialog
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <iostream>

void ImguiMenuBar::Init() {
	
   file_filter_ = 
   "(*.json) Scenes/Archetypes\0*.json\0"
   "(*.jpg) JPG\0* .jpg\0"
   "(*.png) Spritesheets/Textures\0* .png\0"
   "(*.*) All Files\0* *.*\0"; 

   win_ = &*CORE->GetSystem<WindowsSystem>();
   imgui_system_ = &*CORE->GetSystem<ImguiSystem>();
   input_ = &*CORE->GetSystem<InputSystem>();
   factory_ = &*CORE->GetSystem <EntityFactory>();
}

void ImguiMenuBar::Update() {
   
    if(ImGui::BeginMenuBar()){
        if (ImGui::BeginMenu("File")){
            if (ImGui::MenuItem("Open", "Ctrl+O"))
                OpenFile();
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                SaveFile();
            ImGui::EndMenu();
        }
    	if (ImGui::BeginMenu("Archetypes")) {
    		
    	    if (ImGui::MenuItem("Save Archetypes"))
                SaveArchetype();

            ImGui::EndMenu();
    	}
    }
    ImGui::EndMenuBar();
    ImguiInput();
}   

std::string ImguiMenuBar::OpenSaveDialog(const char* filter, int save)
{
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
    if (!save){
        if (GetOpenFileNameA(&ofn) == TRUE)
            return ofn.lpstrFile;
    }
    else{
        if (GetSaveFileNameA(&ofn) == TRUE)
            return ofn.lpstrFile;
    }

    return std::string(); // returns an empty string if user cancels/didnt select anything
}

void ImguiMenuBar::OpenFile(){

    OpenSaveDialog(file_filter_, 0);
}

void ImguiMenuBar::SaveFile(){
    OpenSaveDialog(file_filter_, 1);
}

void ImguiMenuBar::ImguiInput(){

    bool control = ImGui::IsKeyReleased(GetKey(ImGuiKey_ControlL)) || ImGui::IsKeyReleased(GetKey(ImGuiKey_ControlR));
    //bool shift = ImGui::IsKeyReleased(GetKey(ImGuiKey_ShiftL)) || ImGui::IsKeyReleased(GetKey(ImGuiKey_ShiftR));
   
    if (control)
    {
        if (ImGui::IsKeyReleased(GetKey(ImGuiKey_O)))
            OpenFile();

        else if (ImGui::IsKeyReleased(GetKey(ImGuiKey_S))){

            SaveFile();
        }
    }
}

int ImguiMenuBar::GetKey(ImGuiKey imguikey){
	
    return ImGui::GetKeyIndex(imguikey);
}

void ImguiMenuBar::SaveArchetype(){

    std::string path = OpenSaveDialog("(*.json) Scenes/Archetypes\0*.json\0", 1);

    if (!path.empty())
        factory_->SerializeArchetypes(path);
}
