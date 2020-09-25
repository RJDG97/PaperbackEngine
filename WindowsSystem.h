#pragma once
#ifndef _WINDOWS_SYSTEM_H_
#define _WINDOWS_SYSTEM_H_

#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <windows.h>
#include <iostream>
#include "ISystem.h"

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);

class WindowsSystem : public ISystem{
	// Singleton declaration
	static WindowsSystem* w_Instance;
	// Windows class declaration
	WNDCLASSEX wcex;
	LPTSTR windowClass;
	// Message class declaration
	MSG msg;
	// Windows handle
	HWND hwnd;
	// Store windows dimensions
	int wWidth, wHeight;

public:

	// Constructor for WindowsSystem
	WindowsSystem();
	// Initializes the window, cursor, background
	void Init();

	void Update(float frametime);

	std::string GetName();

	void SendMessageD(Message* m);

	// Processes messages
	void ProcessMessage();

	// Static function declaration for single WindowsSystem
	static WindowsSystem* Instance();

	// Unload WindowsSystem
	void UnloadInstance();

	HWND getHandle();

	LPTSTR getWindowClass();

	int getWinWidth() const;

	int getWinHeight() const;

	void setWinWidth(int _width);

	void setWinHeight(int _height);
};

void createDebugWindow();

#endif