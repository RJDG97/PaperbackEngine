#include "Systems/InputSystem.h"
#include "Systems/WindowsSystem.h"
#include "GameStates/PlayState.h"
#include "GameStates/MenuState.h"
#include "Systems/Debug.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/ImguiSystem.h"
#include "Manager/AMap.h"
#include "Script/GeneralScripts.h"

InputSystem sys_input_;

void SendTouchMessage(size_t key_val) {

	Message_Input msg(MessageIDTypes::M_BUTTON_TRIGGERED, key_val);
	CORE->BroadcastMessage(&msg);
}

void SendHoldMessage(size_t key_val) {

	Message_Input msg(MessageIDTypes::M_BUTTON_PRESS, key_val);
	CORE->BroadcastMessage(&msg);
}

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
		// When a button is touched, dispatch a message with the key that's touched
		SendTouchMessage(key);
	}

	// All these are categorized under pressed as well
	// but since they are temporary, not converting
	if (action == GLFW_PRESS) {
		switch (key) {

		case GLFW_KEY_B: 	//'B'
		{
			//for debug bomb
			Message msg(MessageIDTypes::DEBUG_ALL);
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_F:
		{
			CORE->GetSystem<WindowsSystem>()->ToggleFullScreen();
			break;
		}
		case GLFW_KEY_U:
		{
			Message msg(MessageIDTypes::CHANGE_ANIMATION_1);
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_I:
		{
			Message msg(MessageIDTypes::CHANGE_ANIMATION_2);
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_O:
		{
			Message msg(MessageIDTypes::FLIP_SPRITE_X);
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_P:
		{
			Message msg(MessageIDTypes::FLIP_SPRITE_Y);
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_T:
		{
			//GeneralScripts::map_->Pathing({-5, 0}, {0, 0});
			//GeneralScripts::map_->DrawMap();
		}
		}
	}
}

/******************************************************************************/
/*!
  \fn CheckWindowInFocus(GLFWwindow* window, int focused)

  \brief Callback updates whenever window is not in focus
*/
/******************************************************************************/
void CheckWindowInFocus(GLFWwindow* window, int focus) {

	UNREFERENCED_PARAMETER(window);

	if (focus) {
		//in focus

		if (CORE->GetCorePauseStatus()) {

			if (CORE->GetSystem<Game>()->GetStateName() == "Play" && CORE->GetGamePauseStatus())
				return;

			Message msg{ MessageIDTypes::BGM_PAUSE };
			CORE->BroadcastMessage(&msg);

			CORE->ResetCorePauseStatus();
		}
	}
	else {
		//not in focus
		if (!CORE->GetCorePauseStatus()) {

			CORE->ToggleCorePauseStatus();

			Message msg{ MessageIDTypes::BGM_PAUSE };
			CORE->BroadcastMessage(&msg);
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
	glfwSetWindowFocusCallback(ptr_window_, CheckWindowInFocus);
}

// Update for any input related controls
void InputSystem::Update(float frametime) {

	(void)frametime;

	unsigned char input_flag = 0; //used for checking what directional buttons are held
	
	/*
	if (IsKeyPressed(GLFW_KEY_LEFT)) {
		//input_flag |= LEFT_FLAG;
		SendHoldMessage(GLFW_KEY_LEFT);
	}
	if (IsKeyPressed(GLFW_KEY_UP)) {
		//input_flag |= UP_FLAG;
		SendHoldMessage(GLFW_KEY_UP);
	}
	if (IsKeyPressed(GLFW_KEY_RIGHT)) {
		//input_flag |= RIGHT_FLAG;
		SendHoldMessage(GLFW_KEY_RIGHT);
	}
	if (IsKeyPressed(GLFW_KEY_DOWN)) {
		//input_flag |= DOWN_FLAG;
		SendHoldMessage(GLFW_KEY_DOWN);
	}
	*/

	if (IsKeyPressed(GLFW_KEY_A)) {
		//input_flag |= LEFT_FLAG;
		SendHoldMessage(GLFW_KEY_A);
	}
	if (IsKeyPressed(GLFW_KEY_W)) {
		//input_flag |= UP_FLAG;
		SendHoldMessage(GLFW_KEY_W);
	}
	if (IsKeyPressed(GLFW_KEY_S)) {
		//input_flag |= RIGHT_FLAG;
		SendHoldMessage(GLFW_KEY_S);
	}
	if (IsKeyPressed(GLFW_KEY_D)) {
		//input_flag |= DOWN_FLAG;
		SendHoldMessage(GLFW_KEY_D);
	}

	if (IsKeyPressed(GLFW_KEY_UP))
		input_flag |= UP_FLAG;
	if (IsKeyPressed(GLFW_KEY_DOWN))
		input_flag |= DOWN_FLAG;
	if (IsKeyPressed(GLFW_KEY_LEFT))
		input_flag |= LEFT_FLAG;
	if (IsKeyPressed(GLFW_KEY_RIGHT))
		input_flag |= RIGHT_FLAG;

	/*
	if (IsKeyPressed(GLFW_KEY_Z))
		input_flag |= Z_FLAG;
	if (IsKeyPressed(GLFW_KEY_X))
		input_flag |= X_FLAG;
	if (IsKeyPressed(GLFW_KEY_C))
		input_flag |= C_FLAG;
	if (IsKeyPressed(GLFW_KEY_V))
		input_flag |= V_FLAG;
	*/
	
	if (IsKeyPressed(GLFW_KEY_COMMA)) {
	
		SendHoldMessage(GLFW_KEY_COMMA);
	}
	if (IsKeyPressed(GLFW_KEY_PERIOD)) {
		
		SendHoldMessage(GLFW_KEY_PERIOD);
	}
	if (IsKeyPressed(GLFW_KEY_SEMICOLON)) {

		SendHoldMessage(GLFW_KEY_SEMICOLON);
	}
	if (IsKeyPressed(GLFW_KEY_APOSTROPHE)) {

		SendHoldMessage(GLFW_KEY_APOSTROPHE);
	}

	if (IsMouseTriggered(GLFW_MOUSE_BUTTON_LEFT)) {

		Message msg(MessageIDTypes::M_MOUSE_PRESS);
		CORE->BroadcastMessage(&msg);
	}

	Message_PlayerInput cam_msg{ MessageIDTypes::C_MOVEMENT, input_flag };
	CORE->BroadcastMessage(&cam_msg);
}

// Sends Messages
void InputSystem::SendMessageD(Message* m) {
	(void)m;
}

Vector2D InputSystem::GetUpdatedCoords()
{
	Vector2D cursor_ = GetCursorPosition();
	Vector2D cameraPos_ = CORE->GetSystem<CameraSystem>()->GetMainCamera()->GetVector2DCameraPosition();

	float zoom_ = *CORE->GetSystem<CameraSystem>()->GetMainCamera()->GetCameraZoom();
	float Gscale = CORE->GetGlobalScale();

	return ((cursor_ / (zoom_)) + cameraPos_) / Gscale;

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