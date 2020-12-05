/**********************************************************************************
*\file         InputSystem.h
*\brief        Contains declaration of functions and variables used for
*			   the Input System
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _INPUT_SYSTEM_H_
#define _INPUT_SYSTEM_H_

#include <Windows.h>
#include "Systems/ISystem.h"
#include <iostream>
#include "MathLib/Vector2D.h"

#include "GraphicsSystem.h"
#include "Engine/Core.h"

class InputSystem : public ISystem {
	bool debug_;

	int curkeystates_[350];
	int prekeystates_[350];
	int curmousebuttonstates_[3];
	int premousebuttonstates_[3];

	GLFWwindow* ptr_window_;
	Vector2D cursor_pos_;
	int state_ = 0, scroll_ = 0;

public:

/******************************************************************************/
/*!
  \fn SetKeyState(int keycode, int action)

  \brief Set the param key state as the action param
		 For IsKeyTriggered function
*/
/******************************************************************************/
	void SetKeyState(int keycode, int action);

/******************************************************************************/
/*!
  \fn SetMouseState(int button, int action)

  \brief Set the param button state as the action param
		 For IsMouseTriggered function
*/
/******************************************************************************/
	void SetMouseState(int button, int action);

/******************************************************************************/
/*!
  \fn IsKeyPressed(int keycode)

  \brief Return if key has been pressed
*/
/******************************************************************************/
	bool IsKeyPressed(int keycode);

/******************************************************************************/
/*!
  \fn IsKeyTriggered(int keycode)

  \brief Return if key has been pressed once
*/
/******************************************************************************/
	bool IsKeyTriggered(int keycode);
	
/******************************************************************************/
/*!
  \fn SetCursorPosition(double xPos, double yPos)

  \brief Retrieve current cursor position from callback
*/
/******************************************************************************/
	void SetCursorPosition(double xPos, double yPos);

/******************************************************************************/
/*!
  \fn GetCursorPosition()

  \brief Return cursor position
*/
/******************************************************************************/
	Vector2D GetCursorPosition();

/******************************************************************************/
/*!
  \fn IsMousePressed(int button)

  \brief Return if mouse button was pressed
*/
/******************************************************************************/
	bool IsMousePressed(int button);

/******************************************************************************/
/*!
  \fn IsMouseTriggered(int keycode)

  \brief Return if mouse button was pressed once
*/
/******************************************************************************/
	bool IsMouseTriggered(int button);

/******************************************************************************/
/*!
  \fn Init()

  \brief Initialises the system
*/
/******************************************************************************/
	void Init() override;

/******************************************************************************/
/*!
  \fn Update()

  \brief Computes the new velocity for every Motion component and then
		 updates the Transform components based on that new Motion component
*/
/******************************************************************************/
	void Update(float frametime) override;

/******************************************************************************/
/*!
  \fn GetName()

  \brief Returns the name of the system
*/
/******************************************************************************/
	std::string GetName() override { return "Input System"; }

/******************************************************************************/
/*!
  \fn SendMessageD()

  \brief Receives messages broadcasted from Core Engine and processes it
*/
/******************************************************************************/
	void SendMessageD(Message* m) override;

/******************************************************************************/
/*!
  \fn GetUpdatedCoords()

  \brief Get updated cursor coords in game coords
*/
/******************************************************************************/
	Vector2D GetUpdatedCoords();
};

#endif