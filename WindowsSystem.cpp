#include "WindowsSystem.h"
#include "GraphicsSystem.h"
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
WindowsSystem::WindowsSystem() : wcex{}, msg{}, hwnd(), wWidth{ 0 }, wHeight{ 0 } {}

void WindowsSystem::Init(HINSTANCE _currentInstance, const char* _windowName, int _x, int _y, LPCSTR _className, HCURSOR _cursor, HBRUSH _bgColor) {

	UNREFERENCED_PARAMETER(_windowName);
	UNREFERENCED_PARAMETER(_className);

	LPCWSTR Name = L"MyWindows";
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.hInstance = _currentInstance;								// Current instance
	wcex.lpszClassName = Name;										// Name of class
	wcex.hCursor = _cursor;											// Cursor type (Style)
	wcex.hbrBackground = _bgColor;									// Background color of console window
	wcex.lpfnWndProc = WindowProcessMessages;						// Process messages (Not done)
	windowClass = MAKEINTATOM(RegisterClassEx(&wcex));
	
	if (windowClass == 0) {
		std::cout << "RegisterClassEx() failed.";
		std::exit;
	}

	GraphicsSystem::OpenGLExtensionsInit(_currentInstance);

	hwnd = CreateWindow(Name, Name,									// Name of class, name of console window
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,							// Window style
		CW_USEDEFAULT, CW_USEDEFAULT,								// Window initial position
		_x, _y,														// Window size
		nullptr, nullptr, _currentInstance, nullptr);

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

LPTSTR WindowsSystem::getWindowClass()
{
	return windowClass;
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