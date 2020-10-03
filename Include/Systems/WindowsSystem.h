#pragma once
#ifndef _WINDOWS_SYSTEM_H_
#define _WINDOWS_SYSTEM_H_

#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <windows.h>
#include <iostream>
#include "Systems/ISystem.h"

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

	GLFWwindow* ptr_window;

/******************************************************************************/
/*!
  \fn WindowsSystem()

  \brief Constructor for WindowsSystem.
*/
/******************************************************************************/
	WindowsSystem();

/******************************************************************************/
/*!
  \fn Init()

  \brief Initializes the window, cursor, background
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
  \fn Update()

  \brief Empty implementation due to function being pure virtual
*/
/******************************************************************************/
	void Update(float frametime);

/******************************************************************************/
/*!
  \fn GetName()

  \brief Returns the name of the system
*/
/******************************************************************************/
	std::string GetName();

/******************************************************************************/
/*!
  \fn SendMessageD()

  \brief Empty implementation due to function being pure virtual
*/
/******************************************************************************/
	void SendMessageD(Message* m);

/******************************************************************************/
/*!
  \fn ProcessMessage()

  \brief Handles customizing of window's messages / events
*/
/******************************************************************************/
	void ProcessMessage();

/******************************************************************************/
/*!
  \fn Instance()

  \brief Static function to return a pointer to the windows instance
*/
/******************************************************************************/
	static WindowsSystem* Instance();

/******************************************************************************/
/*!
  \fn UnloadInstance()

  \brief Unloads with window instance
*/
/******************************************************************************/
	void UnloadInstance();
	
/******************************************************************************/
/*!
  \fn getHandle()

  \brief Returns a handle to the window
*/
/******************************************************************************/
	HWND getHandle();

/******************************************************************************/
/*!
  \fn getWindowClass()

  \brief Returns a pointer to the window class
*/
/******************************************************************************/
	LPTSTR getWindowClass();

/******************************************************************************/
/*!
  \fn getWinWidth()

  \brief Gets the window's width
*/
/******************************************************************************/
	int getWinWidth() const;

/******************************************************************************/
/*!
  \fn getWinHeight()

  \brief Get the window's height
*/
/******************************************************************************/
	int getWinHeight() const;

/******************************************************************************/
/*!
  \fn setWinWidth()

  \brief Sets the window's new width
*/
/******************************************************************************/
	void setWinWidth(int _width);

/******************************************************************************/
/*!
  \fn setWinHeight()

  \brief Sets the window's new height
*/
/******************************************************************************/
	void setWinHeight(int _height);
};

void createDebugWindow();

#endif