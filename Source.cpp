#include "Source.h"

#include "WindowsSystem.h"
#include <memory>
#include <iostream>

int running = 1;
FILE* file;
void createdebugwindow();

int WINAPI WinMain(HINSTANCE currentInstance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdCount) {

    UNREFERENCED_PARAMETER(previousInstance);
    UNREFERENCED_PARAMETER(cmdLine);

    // Checking for memory leaks
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    createdebugwindow();
#endif

    // Register the window class
    WindowsSystem::Instance()->Init(currentInstance, "GAM200 MonkeyTypewriters", 800, 600);


    // Display Window
    ShowWindow(WindowsSystem::Instance()->getHandle(), cmdCount);
    UpdateWindow(WindowsSystem::Instance()->getHandle());

    while (running)
    {
        // Process windows messages
        WindowsSystem::Instance()->ProcessMessage();

    }

    WindowsSystem::Instance()->UnloadInstance();
    return 0;
}

void createdebugwindow() {
    // Create Debug Window
    if (AllocConsole())
    {
        LPCWSTR Name = L"Debug Window";
        // Message stuff (lets you use print in command)
        freopen_s(&file, "CONOUT$", "wt", stdout);
        std::cout << "hi" << std::endl;
        freopen_s(&file, "CONOUT$", "wt", stderr);
        freopen_s(&file, "CONOUT$", "wt", stdin);

        SetConsoleTitle(Name);
    }
}