/**********************************************************************************
*\file         WindowsSystem.h
*\brief        Contains declaration of functions and variables used for
*			   the Windows System
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _WINDOWS_SYSTEM_H_
#define _WINDOWS_SYSTEM_H_

#include <windows.h>
#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include "Systems/ISystem.h"

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
	bool fullscreen_;

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
  \fn DeSerialize()

  \brief Initializes data members of windows system
*/
/******************************************************************************/
	void DeSerialize();

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

/******************************************************************************/
/*!
  \fn ToggleFullScreen()

  \brief Toggles the screen of the window between fullscreen and windowed
*/
/******************************************************************************/
	void ToggleFullScreen();
};

/******************************************************************************/
/*!
  \fn CreateDebugWindow()

  \brief Create a debug console window
*/
/******************************************************************************/
void CreateDebugWindow();

#endif