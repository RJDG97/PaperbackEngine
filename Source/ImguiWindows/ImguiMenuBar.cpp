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
            if (ImGui::MenuItem("Open Scene", "Ctrl+O"))
                OpenFile();
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                SaveFile();
            ImGui::EndMenu();
        }
    	if (ImGui::BeginMenu("Archetypes")) {
    		
    	    if (ImGui::MenuItem("Save Archetypes"))
                SaveArchetype();
    		//if (ImGui::MenuItem("Load Archetypes"))

            ImGui::EndMenu();
    	}
    }
	ImGui::EndMenuBar();
}   

void ImguiMenuBar::OpenFile(){

    std::string filepath = OpenSaveDialog(file_filter_, 0);

    if (!filepath.empty())
    {
        size_t pos = filepath.find("Resources");

        std::string file = filepath.substr(pos);
    	
        factory_->DeSerializeLevelEntities(file);
    }
}

void ImguiMenuBar::SaveFile(){
   OpenSaveDialog(file_filter_, 1);
}


int ImguiMenuBar::GetKey(ImGuiKey imguikey){
	
    return ImGui::GetKeyIndex(imguikey);
}

void ImguiMenuBar::SaveArchetype(){

    std::string path = OpenSaveDialog("(*.json) Scenes/Archetypes\0*.json\0", 1);

    if (!path.empty())
        factory_->SerializeArchetypes(path);
}
