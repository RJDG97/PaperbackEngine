#include "ImguiWindows/ImguiMenuBar.h"
#include "Engine/Core.h"

// expose the win32 API for the open/save dialog
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

void ImguiMenuBar::Init()
{
   win = &*CORE->GetSystem<WindowsSystem>();
}

void ImguiMenuBar::Update()
{
    const char* filter = "json File (*.json)\0* .json\0";
    if(ImGui::BeginMenuBar()){
        if (ImGui::BeginMenu("File")){
            if (ImGui::MenuItem("Open", "Ctrl+O"))
                OpenSaveDialog(filter, 0);
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                OpenSaveDialog(filter, 1);
            ImGui::EndMenu();
        }
    }
    ImGui::EndMenuBar();
}

std::string ImguiMenuBar::OpenSaveDialog(const char* filter, int save)
{
	OPENFILENAMEA ofn;
    CHAR szFile[260] = { 0 };

    // init OPENFILENAME
    ZeroMemory(&ofn, sizeof(OPENFILENAME));
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = glfwGetWin32Window(win->ptr_window);
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

    return std::string(); // returns an empty string if user cancels
}
