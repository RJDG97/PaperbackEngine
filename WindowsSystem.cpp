#include "WindowsSystem.h"
#include <memory>

FILE* file;
WindowsSystem* WindowsSystem::w_Instance = nullptr;

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
	switch (msg) {
	case WM_DESTROY:
		FreeConsole();
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		if (MessageBox(hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(hwnd);
		}
		return 0;
	case WM_SIZE:
		RECT rect;
		GetClientRect(hwnd, &rect);
		WindowsSystem::Instance()->setWinWidth(rect.right - rect.left);
		WindowsSystem::Instance()->setWinHeight(rect.bottom - rect.top);
	default:
		return DefWindowProc(hwnd, msg, param, lparam);
	}
}

// Default constructor for WindowsSystem Class
WindowsSystem::WindowsSystem() : wc{}, msg{}, hwnd(), wWidth{ 0 }, wHeight{ 0 } {}

void WindowsSystem::Init(HINSTANCE _currentInstance, const char* _windowName, int _x, int _y, LPCSTR _className, HCURSOR _cursor, HBRUSH _bgColor) {

	UNREFERENCED_PARAMETER(_windowName);
	UNREFERENCED_PARAMETER(_className);

	LPCWSTR Name = L"MyWindows";
	wc.hInstance = _currentInstance;								// Current instance
	wc.lpszClassName = Name;										// Name of class
	wc.hCursor = _cursor;											// Cursor type (Style)
	wc.hbrBackground = _bgColor;									// Background color of console window
	wc.lpfnWndProc = WindowProcessMessages;							// Process messages (Not done)
	RegisterClass(&wc);												// Register the Windows Class

	hwnd = CreateWindow(Name, Name,									// Name of class, name of console window
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,							// Window style
		CW_USEDEFAULT, CW_USEDEFAULT,								// Window initial position
		_x, _y,														// Window size
		nullptr, nullptr, nullptr, nullptr);

	RECT rect = { 0 };
	GetClientRect(hwnd, &rect);
	wWidth = rect.right - rect.left;
	wHeight = rect.bottom - rect.top;
}

void WindowsSystem::ProcessMessage() {
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		// Related to keyboard input
		TranslateMessage(&msg);
		// Tells OS to call winproc
		DispatchMessage(&msg);
	}
}

WindowsSystem* WindowsSystem::Instance() {
	// Create a new WindowsSystem class
	if (!w_Instance)
		w_Instance = new WindowsSystem;
	return w_Instance;
}

void WindowsSystem::UnloadInstance() {
	// Delete WindowsSystem class
	delete w_Instance;
	w_Instance = nullptr;
}

HWND WindowsSystem::getHandle() {
	return hwnd;
}

int WindowsSystem::getWinWidth() const {
	return wWidth;
}

int WindowsSystem::getWinHeight() const {
	return wHeight;
}

void WindowsSystem::setWinWidth(int _width) {
	wWidth = _width;
}

void WindowsSystem::setWinHeight(int _height) {
	wHeight = _height;
}

void createDebugWindow() {
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