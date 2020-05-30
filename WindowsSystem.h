#pragma once
#ifndef _WINDOWS_SYSTEM_H_
#define _WINDOWS_SYSTEM_H_

#include <windows.h>

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);

class WindowsSystem {
	// Singleton declaration
	static WindowsSystem* w_Instance;
	// Windows class declaration
	WNDCLASS wc;
	// Message class declaration
	MSG msg;
	// Windows handle
	HWND hwnd;



public:
	
	// Constructor for WindowsSystem
	WindowsSystem();
	// Initializes the window, cursor, background
	void Init(HINSTANCE _currentInstance, const char* _windowName, int _x, int _y,
		LPCSTR _className = "MyWindowsSystem",
		HCURSOR _cursor = LoadCursor(nullptr, IDC_ARROW),
		HBRUSH _bgColor = (HBRUSH)COLOR_WINDOW);

	// Processes messages
	void ProcessMessage();

	// Static function declaration for single WindowsSystem
	static WindowsSystem* Instance();

	// Unload WindowsSystem
	void UnloadInstance();

	HWND getHandle();
};

#endif