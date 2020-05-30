#include "Source.h"

#include "WindowsSystem.h"
#include <memory>

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
        // FrameRateController
        PE_FrameRate.FrameRateLoop();

        // Process windows messages
        WindowsSystem::Instance()->ProcessMessage();



        /********Input Test********/

        static int forward = PE_W;

        if (sys_input.CheckTriggeredInput(PE_S))
            std::cout << "Pressed" << std::endl;

        if (sys_input.CheckCurrentInput(PE_S))
            std::cout << "S" << std::endl;

        if (sys_input.CheckReleasedInput(PE_S))
            std::cout << "Released" << std::endl;

        if (sys_input.CheckTriggeredInput(forward))
            std::cout << "Forward" << std::endl;
        std::cin >> forward;

        sys_input.UpdateKeyInput(PE_E, forward);

        /********Input Test********/

        /********Frame Test********/

        if (sys_input.CheckTriggeredInput(PE_TAB))
        {
            std::cout << PE_FrameRate.GetFrames() << std::endl;
            std::cout << PE_FrameRate.Dt << std::endl;
            std::cout << PE_FrameRate.TimeElapsed(s) << std::endl;
        }

        // Change FPS
        if (sys_input.CheckTriggeredInput(PE_1))
            PE_FrameRate.SetFPS(90);
        if (sys_input.CheckTriggeredInput(PE_2))
            PE_FrameRate.SetFPS(60);

        /********Frame Test********/

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
        freopen_s(&file, "CONOUT$", "wt", stderr);
        freopen_s(&file, "CONOUT$", "wt", stdin);

        SetConsoleTitle(Name);
    }
}