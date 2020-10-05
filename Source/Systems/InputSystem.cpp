#include "Systems/InputSystem.h"
#include "Systems/WindowsSystem.h"
#include "GameStates/PlayState.h"
#include "GameStates/MenuState.h"
#include "Systems/Debug.h"
#include "Systems/GraphicsSystem.h"

void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos)
{
	UNREFERENCED_PARAMETER(window);

	CORE->GetSystem<InputSystem>()->SetCursorPosition(xPos, yPos);
}

void cursorEnterCallback(GLFWwindow* window, int entered)
{
	UNREFERENCED_PARAMETER(window);

	if (entered)
	{
		
	
	}
	else
	{

	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
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

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	UNREFERENCED_PARAMETER(window);
	UNREFERENCED_PARAMETER(xOffset);
	UNREFERENCED_PARAMETER(yOffset);
	// in case we need to scroll input
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
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
			// e.g. let A Pause game in play state and quit game in menu state

			//Method 1
			//check current state
			// run Fn1 if (curr state is play) and Fn2 if state is menu

			//Method 2
			//wrap custom message w/ custom message id targetting Game ISystem
			// send message
			// let Game sort out what to do
			break;
		case GLFW_KEY_B: 	//'B'
		{
			//for debug bomb
			Message msg(MessageIDTypes::DEBUG_ALL);
			CORE->BroadcastMessage(&msg);
			break;
		}
		break;
		case GLFW_KEY_C: 	//'C'
			break;
		case GLFW_KEY_D: 	//'D'
			break;
		case GLFW_KEY_E: 	//'E'
		{
			Message_PlayerInput msg(MessageIDTypes::M_BUTTON_PRESS, GLFW_KEY_E);
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_F: 	//'F'
			break;
		case GLFW_KEY_G: 	//'G'
			break;
		case GLFW_KEY_H: 	//'H'
			break;
		case GLFW_KEY_I: 	//'I'
			break;
		case GLFW_KEY_J: 	//'J'
			break;
		case GLFW_KEY_K: 	//'K'
			break;
		case GLFW_KEY_L: 	//'L'
			break;
		case GLFW_KEY_M: 	//'M'
			break;
		case GLFW_KEY_N: 	//'N'
			break;
		case GLFW_KEY_O: 	//'O'
			break;
		case GLFW_KEY_P: 	//'P'
			break;
		case GLFW_KEY_Q: 	//'Q'
			break;
		case GLFW_KEY_R: 	//'R'
		{
			Message_PlayerInput msg(MessageIDTypes::M_BUTTON_PRESS, GLFW_KEY_R);
			CORE->BroadcastMessage(&msg);
			break;
		}
		case GLFW_KEY_S: 	//'S'
			break;
		case GLFW_KEY_T: 	//'T'
			break;
		case GLFW_KEY_U: 	//'U'
			break;
		case GLFW_KEY_V: 	//'V'
			break;
		case GLFW_KEY_W: 	//'W'
			break;
		case GLFW_KEY_X: 	//'X'
			break;
		case GLFW_KEY_Y: 	//'Y'
			break;
		case GLFW_KEY_Z: 	//'Z'
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

void InputSystem::Init() {
	M_DEBUG->WriteDebugMessage("Input System Init\n");

	ptr_window_ = CORE->GetSystem<WindowsSystem>()->ptr_window;

	glfwSetKeyCallback(ptr_window_, keyCallback);
	glfwSetCursorPosCallback(ptr_window_, cursorPositionCallback);
	glfwSetCursorEnterCallback(ptr_window_, cursorEnterCallback);
	glfwSetMouseButtonCallback(ptr_window_, mouseButtonCallback);
	glfwSetScrollCallback(ptr_window_, scrollCallback);
}

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

	// Temporary placeholder before Input System conversion
	if (IsKeyPressed(GLFW_KEY_B)) {

		//for debug bomb
		Message msg(MessageIDTypes::DEBUG_ALL);
		CORE->BroadcastMessage(&msg);
	}

	if (IsMouseTriggered(GLFW_MOUSE_BUTTON_LEFT)) {

		Message msg(MessageIDTypes::M_MOUSE_PRESS);
		CORE->BroadcastMessage(&msg);
	}

	//send the message with the updated flag to be processed
	Message_PlayerInput input_msg{ MessageIDTypes::M_MOVEMENT, input_flag };
	CORE->BroadcastMessage(&input_msg);
}

void InputSystem::SendMessageD(Message* m) {
	(void)m;
}

void InputSystem::SetKeyState(int keycode, int action)
{
	prekeystates[keycode] = curkeystates[keycode];
	curkeystates[keycode] = action;
}

void InputSystem::SetMouseState(int button, int action)
{
	premousebuttonstates[button] = curmousebuttonstates[button];
	curmousebuttonstates[button] = action;
}

bool InputSystem::IsMousePressed(int button)
{
	return curmousebuttonstates[button];
}

bool InputSystem::IsMouseTriggered(int button)
{
	if (premousebuttonstates[button] == GLFW_RELEASE && curmousebuttonstates[button] == GLFW_PRESS)
	{
		premousebuttonstates[button] = 1;
		return true;
	}
	return false;
}

bool InputSystem::IsKeyPressed(int keycode)
{
	return curkeystates[keycode];
}

bool InputSystem::IsKeyTriggered(int keycode)
{
	if (prekeystates[keycode] == GLFW_RELEASE && curkeystates[keycode] == GLFW_PRESS)
	{
		prekeystates[keycode] = 1;
		return true;
	}
	return false;
}

void InputSystem::SetCursorPosition(double xPos, double yPos)
{
	xPos -= CORE->GetSystem<WindowsSystem>()->GetWinWidth() / 2;
	yPos = (-yPos) + CORE->GetSystem<WindowsSystem>()->GetWinHeight() / 2;

	cursor_pos = { static_cast<float>(xPos), static_cast<float>(yPos) };
}

// Returns current cursor position
Vector2D InputSystem::GetCursorPosition() {
	return cursor_pos;
}