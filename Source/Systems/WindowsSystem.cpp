#include "Systems/WindowsSystem.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/Debug.h"
#include <memory>

FILE* file;
WindowsSystem* WindowsSystem::w_instance = nullptr;

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
		WindowsSystem::Instance()->SetWinWidth(rect.right - rect.left);
		WindowsSystem::Instance()->SetWinHeight(rect.bottom - rect.top);
	default:
		return DefWindowProc(hwnd, msg, param, lparam);
	}
}

// Default constructor for WindowsSystem Class
WindowsSystem::WindowsSystem() : 
	wcex{}, 
	msg{}, 
	hwnd{},
	width_{}, 
	height_{}
{
}

void WindowsSystem::Init() {

	if (!glfwInit()) {
		std::cout << "GLFW init has failed - abort program!!!" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	// In case a GLFW function fails, an error is reported to callback function
	//glfwSetErrorCallback(GLHelper::error_cb);

	//width_ = 800;
	//height_ = 600;
	DeSerialize();

	// Before asking GLFW to create an OpenGL context, we specify the minimum constraints
	// in that context:
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // window dimensions are static

	ptr_window = glfwCreateWindow(width_, height_, windows_name_.c_str()/*"Veggie Escape"*/, NULL, NULL);

	if (!ptr_window) {
		std::cerr << "GLFW unable to create OpenGL context - abort program\n";
		glfwTerminate();
		std::exit(EXIT_FAILURE);
	}
	
	glfwMakeContextCurrent(ptr_window);

	// Input
	glfwSetInputMode(ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Unable to initialize GLEW - error: "
			<< glewGetErrorString(err) << " abort program" << std::endl;
		std::exit(EXIT_FAILURE);
	}

	M_DEBUG->WriteDebugMessage("Window System Init\n");
}

void WindowsSystem::DeSerialize() {

	M_DEBUG->WriteDebugMessage("Initializing Windows System\n");

	// Parse the stringstream into document (DOM) format
	rapidjson::Document doc;
	DeSerializeJSON("Resources/EntityConfig/window.json", doc);

	// Treats entire filestream at index as array and ensure that it is an array
	const rapidjson::Value& value_arr = doc["Window"];
	//assert(value_arr.IsArray());
	DEBUG_ASSERT(value_arr.IsArray(), "Entry does not exist in JSON");

	//stores the data into a stream that is easier to read data from
	std::stringstream stream;

	// Iterate through the body of the "Header"
	for (rapidjson::Value::ConstValueIterator it = value_arr.Begin(); it != value_arr.End(); ++it) {

		// Each value is essentially a container for multiple members
		// IsObject enforces that the member is an object that will contain data:key pairs
		const rapidjson::Value& member = *it;

		//assert(member.IsObject());
		DEBUG_ASSERT(member.IsObject(), "Entry does not exist in JSON");

		//assume that only contains width & height

		for (rapidjson::Value::ConstMemberIterator it2 = member.MemberBegin(); it2 != member.MemberEnd(); ++it2) {

			stream << it2->value.GetString() << " ";
		}
	}

	stream >> windows_name_ >> width_ >> height_;

	std::cout << windows_name_ << " " << width_ << " " << height_ << std::endl;
}

void WindowsSystem::Update(float frametime)
{
	UNREFERENCED_PARAMETER(frametime);
}

std::string WindowsSystem::GetName()
{
	return "WindowsSystem";
}

void WindowsSystem::SendMessageD(Message* m)
{
	UNREFERENCED_PARAMETER(m);
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
	if (!w_instance)
		w_instance = new WindowsSystem;
	return w_instance;
}

void WindowsSystem::UnloadInstance() {
	// Delete WindowsSystem class
	delete w_instance;
	w_instance = nullptr;
}

HWND WindowsSystem::GetHandle() {
	return hwnd;
}

LPTSTR WindowsSystem::GetWindowClass()
{
	return windowClass;
}

std::string WindowsSystem::GetWindowName() const {

	return windows_name_;
}

int WindowsSystem::GetWinWidth() const {
	return width_;
}

int WindowsSystem::GetWinHeight() const {
	return height_;
}

void WindowsSystem::SetWinWidth(int _width) {
	width_ = _width;
}

void WindowsSystem::SetWinHeight(int _height) {
	height_ = _height;
}

void CreateDebugWindow() {
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