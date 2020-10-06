#pragma once
#ifndef _WINDOWS_SYSTEM_H_
#define _WINDOWS_SYSTEM_H_

#include <windows.h>
#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include "Systems/ISystem.h"

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);

class WindowsSystem : public ISystem{

	static WindowsSystem* w_instance;
	// Windows class declaration
	WNDCLASSEX wcex;
	LPTSTR windowClass;
	// Message class declaration
	MSG msg;
	// Windows handle
	HWND hwnd;
	// Store windows dimensions
	int width_, height_;
	std::string windows_name_;

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
  \fn Serialize()

  \brief Initializes data members of windows system
*/
/******************************************************************************/
	void Serialize();

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
  \fn GetHandle()

  \brief Returns a handle to the window
*/
/******************************************************************************/
	HWND GetHandle();

/******************************************************************************/
/*!
  \fn GetWindowClass()

  \brief Returns a pointer to the window class
*/
/******************************************************************************/
	LPTSTR GetWindowClass();

/******************************************************************************/
/*!
  \fn GetWindowName()

  \brief Gets the window's name
*/
/******************************************************************************/
	std::string GetWindowName() const;

/******************************************************************************/
/*!
  \fn GetWinWidth()

  \brief Gets the window's width
*/
/******************************************************************************/
	int GetWinWidth() const;

/******************************************************************************/
/*!
  \fn GetWinHeight()

  \brief Get the window's height
*/
/******************************************************************************/
	int GetWinHeight() const;

/******************************************************************************/
/*!
  \fn SetWinWidth()

  \brief Sets the window's new width
*/
/******************************************************************************/
	void SetWinWidth(int _width);

/******************************************************************************/
/*!
  \fn SetWinHeight()

  \brief Sets the window's new height
*/
/******************************************************************************/
	void SetWinHeight(int _height);
};

void CreateDebugWindow();

#endif