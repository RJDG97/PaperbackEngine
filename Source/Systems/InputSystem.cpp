#include "Systems/InputSystem.h"
#include "Systems/WindowsSystem.h"
#include "GameStates/PlayState.h"
#include "GameStates/MenuState.h"
#include "Systems/Debug.h"
#include "Systems/GraphicsSystem.h"

InputSystem sys_input_;

/******************************************************************************/
/*!
  \fn CursorPositionCallback(GLFWwindow* window, double xPos, double yPos)

  \brief Function is constantly called by glfw for cursor position
		 Updates cursor positions
*/
/******************************************************************************/
void CursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	UNREFERENCED_PARAMETER(window);
	CORE->GetSystem<InputSystem>()->SetCursorPosition(xPos, yPos);
}

/******************************************************************************/
/*!
  \fn CursorEnterCallback(GLFWwindow* window, int entered)

  \brief Callback updates whenever cursor enters or exits the window
*/
/******************************************************************************/
void CursorEnterCallback(GLFWwindow* window, int entered)
{
	UNREFERENCED_PARAMETER(window);
	if (entered)
	{}
	else
	{}
}


/******************************************************************************/
/*!
  \fn MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)

  \brief Callback updates whenever a mouse button is clicked
*/
/******************************************************************************/
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(mods);
	CORE->GetSystem<InputSystem>()->SetMouseState(button, action);
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		break;
	}
}

/******************************************************************************/
/*!
  \fn ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)

  \brief Callback updates whenever a mouse is scrolled
*/
/******************************************************************************/
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(xOffset);
	UNREFERENCED_PARAMETER(yOffset);
	// in case we need to scroll input
}

/******************************************************************************/
/*!
  \fn KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)

  \brief Callback updates whenever a key is pressed or released or repeated
*/
/******************************************************************************/
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	UNREFERENCED_PARAMETER(scancode);
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(mods);

	// if Triggered
	CORE->GetSystem<InputSystem>()->SetKeyState(key, action);
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_0:  //'0'
		{
			std::cout << "Debug: Decrementing entity HP" << std::endl;
			MessageHPDecre msg{ 2 }; // Entity id = 2
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_1:	//'1'
		{
			Message_CustomState msg{ &m_PlayState, MessageIDTypes::GSM_CHANGESTATE}; // pass in another existing state?
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_2:	//'2'
		{
			Message_CustomState msg{ &m_MenuState, MessageIDTypes::GSM_CHANGESTATE }; // push maybe game state
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_3:	//'3'
		{
			Message msg{ MessageIDTypes::GSM_POPSTATE };
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_4:	//'4'
		{
			MessageBGM_Play msg{ std::string{"Kachow"} };
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_5:	//'5'
		{
			Message msg(MessageIDTypes::BGM_STOP);
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_6:	//'6'
		{
			MessageBGM_Play msg{ std::string{"BGM"} };
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_7:	//'7'
		{
			Message msg(MessageIDTypes::BGM_MUTE);
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_8:	//'8'
		{
			Message msg(MessageIDTypes::BGM_PAUSE);
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_9:	//'9'
		{
			std::cout << "Debug: Rotating entity" << std::endl;
			MessageRotation msg{ 1 }; // Entity id = 1
			CORE->BroadcastMessage(&msg);
			break;
		}

		case GLFW_KEY_A: 	//'A'
			break;
		case GLFW_KEY_B: 	//'B'
		{
			//for debug bomb
			Message msg(MessageIDTypes::DEBUG_ALL);
			CORE->BroadcastMessage(&msg);
			break;
		}
		break;
		case GLFW_KEY_C:
			break;
		case GLFW_KEY_D:
			break;
		case GLFW_KEY_E: 	//'E'
		{
			Message_PlayerInput msg(MessageIDTypes::M_BUTTON_PRESS, GLFW_KEY_E);
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_F: 	//'F'
			break;
		case GLFW_KEY_G:
			break;
		case GLFW_KEY_H:
			break;
		case GLFW_KEY_I:
			break;
		case GLFW_KEY_J:
			break;
		case GLFW_KEY_K:
			break;
		case GLFW_KEY_L:
			break;
		case GLFW_KEY_M:
			break;
		case GLFW_KEY_N:
			break;
		case GLFW_KEY_O:
			break;
		case GLFW_KEY_P:
			break;
		case GLFW_KEY_Q:
			break;
		case GLFW_KEY_R: 	//'R'
		{
			Message_PlayerInput msg(MessageIDTypes::M_BUTTON_PRESS, GLFW_KEY_R);
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_S: 	//'S'
			break;
		case GLFW_KEY_T:
			break;
		case GLFW_KEY_U:
			break;
		case GLFW_KEY_V:
			break;
		case GLFW_KEY_W:
			break;
		case GLFW_KEY_X:
			break;
		case GLFW_KEY_Y:
			break;
		case GLFW_KEY_Z:
		{
			Message msg{ MessageIDTypes::FTY_PURGE };
			CORE->BroadcastMessage(&msg);
			break;
		}
		default:
			//std::cout << "Input System: [Error] Key is not bound" << std::endl;
			break;
		}
	}
}

// Initialise all the glfw callbacks relating to input
void InputSystem::Init() {
	M_DEBUG->WriteDebugMessage("Input System Init\n");

	ptr_window_ = CORE->GetSystem<WindowsSystem>()->ptr_window;

	glfwSetKeyCallback(ptr_window_, KeyCallback);
	glfwSetCursorPosCallback(ptr_window_, CursorPositionCallback);
	glfwSetCursorEnterCallback(ptr_window_, CursorEnterCallback);
	glfwSetMouseButtonCallback(ptr_window_, MouseButtonCallback);
	glfwSetScrollCallback(ptr_window_, ScrollCallback);
}

// Update for any input related controls
void InputSystem::Update(float frametime) {

	(void)frametime;

	unsigned char input_flag = 0; //used for checking what directional buttons are held

	//input.GetMouseCoord();

	if (IsKeyPressed(GLFW_KEY_LEFT))
		input_flag |= LEFT_FLAG;
	if (IsKeyPressed(GLFW_KEY_UP))
		input_flag |= UP_FLAG;
	if (IsKeyPressed(GLFW_KEY_RIGHT))
		input_flag |= RIGHT_FLAG;
	if (IsKeyPressed(GLFW_KEY_DOWN))
		input_flag |= DOWN_FLAG;


	if (IsKeyPressed(GLFW_KEY_COMMA)) {
	
		Message_PlayerInput msg(MessageIDTypes::M_BUTTON_PRESS, GLFW_KEY_COMMA);
		CORE->BroadcastMessage(&msg);
	}
	if (IsKeyPressed(GLFW_KEY_PERIOD)) {
		
		Message_PlayerInput msg(MessageIDTypes::M_BUTTON_PRESS, GLFW_KEY_PERIOD);
		CORE->BroadcastMessage(&msg);
	}
	if (IsKeyPressed(GLFW_KEY_SEMICOLON)) {

		Message_PlayerInput msg(MessageIDTypes::M_BUTTON_PRESS, GLFW_KEY_SEMICOLON);
		CORE->BroadcastMessage(&msg);
	}
	if (IsKeyPressed(GLFW_KEY_APOSTROPHE)) {

		Message_PlayerInput msg(MessageIDTypes::M_BUTTON_PRESS, GLFW_KEY_APOSTROPHE);
		CORE->BroadcastMessage(&msg);
	}

	// Temporary placeholder before Input System conversion
	/*if (IsKeyPressed(GLFW_KEY_B)) {

		//for debug bomb
		Message msg(MessageIDTypes::DEBUG_ALL);
		CORE->BroadcastMessage(&msg);
	}*/

	if (IsMouseTriggered(GLFW_MOUSE_BUTTON_LEFT)) {

		Message msg(MessageIDTypes::M_MOUSE_PRESS);
		CORE->BroadcastMessage(&msg);
	}

	//send the message with the updated flag to be processed
	Message_PlayerInput input_msg{ MessageIDTypes::M_MOVEMENT, input_flag };
	CORE->BroadcastMessage(&input_msg);
}

// Sends Messages
void InputSystem::SendMessageD(Message* m) {
	(void)m;
}

// Set the param key state as the action param
void InputSystem::SetKeyState(int keycode, int action)
{
	// For comparing states later (e.g. triggered)
	prekeystates_[keycode] = curkeystates_[keycode];
	curkeystates_[keycode] = action;
}

// Set the param button state as the action param
void InputSystem::SetMouseState(int button, int action)
{
	premousebuttonstates_[button] = curmousebuttonstates_[button];
	curmousebuttonstates_[button] = action;
}

// Return if mouse button was pressed
bool InputSystem::IsMousePressed(int button)
{
	return curmousebuttonstates_[button];
}

// Return if mouse button was pressed once
bool InputSystem::IsMouseTriggered(int button)
{
	if (premousebuttonstates_[button] == GLFW_RELEASE && curmousebuttonstates_[button] == GLFW_PRESS)
	{
		premousebuttonstates_[button] = 1;
		return true;
	}
	return false;
}

// Return if key has been pressed
bool InputSystem::IsKeyPressed(int keycode)
{
	return curkeystates_[keycode];
}

// Return if key has been pressed once
bool InputSystem::IsKeyTriggered(int keycode)
{
	if (prekeystates_[keycode] == GLFW_RELEASE && curkeystates_[keycode] == GLFW_PRESS)
	{
		prekeystates_[keycode] = 1;
		return true;
	}
	return false;
}

// Retrieve current cursor position from callback
void InputSystem::SetCursorPosition(double xPos, double yPos)
{
	xPos -= CORE->GetSystem<WindowsSystem>()->GetWinWidth() / 2;
	yPos = (-yPos) + CORE->GetSystem<WindowsSystem>()->GetWinHeight() / 2;

	cursor_pos_ = { static_cast<float>(xPos), static_cast<float>(yPos) };
}

// Returns current cursor position
Vector2D InputSystem::GetCursorPosition() {
	return cursor_pos_;
}