#include "WindowsSystem.h"
#include "GraphicsSystem.h"
#include <memory>

FILE* file;
WindowsSystem* WindowsSystem::w_Instance = nullptr;

WindowsSystem* WINDOWSSYSTEM;

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
WindowsSystem::WindowsSystem() : wcex{}, msg{}, hwnd(), wWidth{ 0 }, wHeight{ 0 }
{
	WINDOWSSYSTEM = this;
}

void WindowsSystem::Init() {

	if (!glfwInit()) {
		std::cout << "GLFW init has failed - abort program!!!" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	// In case a GLFW function fails, an error is reported to callback function
	//glfwSetErrorCallback(GLHelper::error_cb);

	wWidth = 800;
	wHeight = 600;

	// Before asking GLFW to create an OpenGL context, we specify the minimum constraints
	// in that context:
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window dimensions are static

	ptr_window = glfwCreateWindow(wWidth, wHeight, "Placeholder", NULL, NULL);

	if (!ptr_window) {
		std::cerr << "GLFW unable to create OpenGL context - abort program\n";
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(ptr_window);

	/* MOVE THIS OVER TO THE INPUT SYSTEM PLS
	glfwSetFramebufferSizeCallback(ptr_window, GLHelper::fbsize_cb);
	glfwSetKeyCallback(ptr_window, GLHelper::key_cb);
	glfwSetMouseButtonCallback(ptr_window, GLHelper::mousebutton_cb);
	glfwSetCursorPosCallback(ptr_window, GLHelper::mousepos_cb);
	glfwSetScrollCallback(ptr_window, GLHelper::mousescroll_cb);
	*/
	// this is the default setting ...
	glfwSetInputMode(ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// Part 2: Initialize entry points to OpenGL functions and extensions
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Unable to initialize GLEW - error: "
			<< glewGetErrorString(err) << " abort program" << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void WindowsSystem::Update(float frametime)
{

}

std::string WindowsSystem::GetName()
{
	return "WindowsSystem";
}

void WindowsSystem::SendMessageD(Message* m)
{
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