#include "InputSystem.h"
#include "WinKeyCodes.h"
#include "WindowsSystem.h"
#include "Core.h"
#include "PlayState.h"
#include "MenuState.h"

InputSystem sys_input_;

InputSystem::InputSystem() : currentkey_{ 0 }, 
							 previouskey_{ 0 }, 
							 keyreleased_{ 0 }, 
							 b_change_{ false },
							 b_toggle_{ false },
							 initial_{}
{}

bool InputSystem::CheckCurrentInput(int key) {
	return (GetAsyncKeyState(key));
}

bool InputSystem::CheckTriggeredInput(int key) {
	previouskey_[key] = currentkey_[key];
	if (GetAsyncKeyState(key)) {
		currentkey_[key] = true;
		if (previouskey_[key])
			return false;
	}
	else {
		currentkey_[key] = false;
		if (previouskey_[key])
			keyreleased_[key] = true;
		else
			keyreleased_[key] = false;
		return false;
	}
	return true;
}

bool InputSystem::CheckReleasedInput(int key) {
	return (keyreleased_[key]);
}

// In general unused
bool InputSystem::KeyPoll(int& key) {
	for (int i = 0; i < 164; ++i) {
		if (CheckTriggeredInput(i)) {
			key = i;
			return true;
		}
	}
	return false;
}

void InputSystem::UpdateKeyInput(int trigger, int& key) {
	if (sys_input_.CheckTriggeredInput(trigger)) {
		std::cout << "Change Key to: ";
		b_change_ = 1;
	}

	if (b_change_) {
		if (sys_input_.KeyPoll(key))
		{
			b_change_ = 0;
			std::cout << GetKeyValue(key) << std::endl;
		}
	}
}

// In general unused
char InputSystem::GetKeyValue(int key)
{
	switch (key)
	{
	case 0x41: return 'A';
		break;
	case 0x42: return 'B';
		break;
	case 0x43: return 'C';
		break;
	case 0x44: return 'D';
		break;
	case 0x45: return 'E';
		break;
	case 0x46: return 'F';
		break;
	case 0x47: return 'G';
		break;
	case 0x48: return 'H';
		break;
	case 0x49: return 'I';
		break;
	case 0x4A: return 'J';
		break;
	case 0x4B: return 'K';
		break;
	case 0x4C: return 'L';
		break;
	case 0x4D: return 'M';
		break;
	case 0x4E: return 'N';
		break;
	case 0x4F: return 'O';
		break;
	case 0x50: return 'P';
		break;
	case 0x51: return 'Q';
		break;
	case 0x52: return 'R';
		break;
	case 0x53: return 'S';
		break;
	case 0x54: return 'T';
		break;
	case 0x55: return 'U';
		break;
	case 0x56: return 'V';
		break;
	case 0x57: return 'W';
		break;
	case 0x58: return 'X';
		break;
	case 0x59: return 'Y';
		break;
	case 0x5A: return 'Z';
		break;
	default:
		return '/';
	}
}

// Returns current cursor position
void InputSystem::GetCursorPosition(Vector2D& pos) {
	POINT p;
	if (GetCursorPos(&p)) {
		if (ScreenToClient(WindowsSystem::Instance()->getHandle(), &p)) {
			pos.x = static_cast<float>(p.x - WindowsSystem::Instance()->getWinWidth() / 2);
			pos.y = static_cast<float>((-p.y) + WindowsSystem::Instance()->getWinHeight() / 2);
			std::cout << "x: " << pos.x << std::endl;
			std::cout << "y: " << pos.y << std::endl;
		}
	}
}

// Distance between initial click and end click (Currently not working)
void InputSystem::GetCursorPositionDelta(Vector2D& pos, int key) {
	if (!b_toggle_) {
		b_toggle_ = true;
		GetCursorPosition(initial_);
	}
	else if (CheckReleasedInput(key)) {
		b_toggle_ = false;
		GetCursorPosition(pos);
		pos.x = max(pos.x, initial_.x) - min(pos.x, initial_.x);
		pos.x = (pos.x) < 0 ? (-pos.x) : pos.x;
		pos.y = max(pos.y, initial_.y) - min(pos.y, initial_.y);
		pos.y = (pos.y) < 0 ? (-pos.y) : pos.y;

		std::cout << "delta x: " << pos.x << std::endl;
		std::cout << "delta y: " << pos.y << std::endl;
	}
}

void InputSystem::Init() {

}

void InputSystem::Update(float frametime) {

	(void)frametime;

	//for keys requiring to check buttons held down
	for (int i = 0x25; i <= 0x28; ++i) {
		if (CheckCurrentInput(i)) {
			switch (i)
			{
			case 0x25: //LEFT ARROW key
			case 0x26: //UP ARROW key
			case 0x27: //RIGHT ARROW key
			case 0x28: //DOWN ARROW key
			{
				//send message to game logc of button press
				Message_Input msg{ MessageIDTypes::M_ButtonPress, i };
				CORE->BroadcastMessage(&msg);
				break;
			}
			}
		}
	}

	for (int i = 0x30; i <= 0x5A; ++i) {
		if (CheckTriggeredInput(i)) {
			switch (i)
			{
			case 0x30:  //'0'
			{
				std::cout << "Debug: Decrementing entity HP" << std::endl;
				MessageHPDecre msg{ 2 }; // Entity id = 2
				CORE->BroadcastMessage(&msg);
				break;
			}
			case 0x31:	//'1'
			{
				Message_CustomState msg{ &m_PlayState, MessageIDTypes::GSM_ChangeState }; // pass in another existing state?
				CORE->BroadcastMessage(&msg);
				break;
			}
			case 0x32:	//'2'
			{
				Message_CustomState msg{ &m_MenuState, MessageIDTypes::GSM_ChangeState }; // push maybe game state
				CORE->BroadcastMessage(&msg);
				break;
			}
			case 0x33:	//'3'
			{
				Message msg{ MessageIDTypes::GSM_PopState };
				CORE->BroadcastMessage(&msg);
				break;
			}
			case 0x34:	//'4'
			{
				MessageBGM_Play msg{ std::string{"Kachow"} };
				CORE->BroadcastMessage(&msg);
				break;
			}
			case 0x35:	//'5'
			{
				Message msg(MessageIDTypes::BGM_Stop);
				CORE->BroadcastMessage(&msg);
				break;
			}
			case 0x36:	//'6'
			{
				MessageBGM_Play msg{ std::string{"BGM"} };
				CORE->BroadcastMessage(&msg);
				break;
			}
			case 0x37:	//'7'
			{
				Message msg(MessageIDTypes::BGM_Mute);
				CORE->BroadcastMessage(&msg);
				break;
			}
			case 0x38:	//'8'
			{
				Message msg(MessageIDTypes::BGM_Pause);
				CORE->BroadcastMessage(&msg);
				break;
			}
			case 0x39:	//'9'
			{
				std::cout << "Debug: Rotating entity" << std::endl;
				MessageRotation msg{ 1 }; // Entity id = 1
				CORE->BroadcastMessage(&msg);
				break;
			}
			case 0x41: 	//'A'
				// e.g. let A Pause game in play state and quit game in menu state

				//Method 1
				//check current state
				// run Fn1 if (curr state is play) and Fn2 if state is menu

				//Method 2
				//wrap custom message w/ custom message id targetting Game ISystem
				// send message
				// let Game sort out what to do
				break;
			case 0x42: 	//'B'
				break;
			case 0x43: 	//'C'
				break;
			case 0x44: 	//'D'
				break;
			case 0x45: 	//'E'
				break;
			case 0x46: 	//'F'
				break;
			case 0x47: 	//'G'
				break;
			case 0x48: 	//'H'
				break;
			case 0x49: 	//'I'
				break;
			case 0x4A: 	//'J'
				break;
			case 0x4B: 	//'K'
				break;
			case 0x4C: 	//'L'
				break;
			case 0x4D: 	//'M'
				break;
			case 0x4E: 	//'N'
				break;
			case 0x4F: 	//'O'
				break;
			case 0x50: 	//'P'
				break;
			case 0x51: 	//'Q'
				break;
			case 0x52: 	//'R'
				break;
			case 0x53: 	//'S'
				break;
			case 0x54: 	//'T'
				break;	
			case 0x55: 	//'U'
				break;	
			case 0x56: 	//'V'
				break;	
			case 0x57: 	//'W'
				break;	
			case 0x58: 	//'X'
				break;	
			case 0x59: 	//'Y'
				break;	
			case 0x5A: 	//'Z'
			{
				Message msg{ MessageIDTypes::FTY_Purge };
				CORE->BroadcastMessage(&msg);
				break;
			}
			default:
				std::cout << "Input System: [Error] Key is not bound" << std::endl;
			}
		}
	}
}

void InputSystem::SendMessageD(Message* m) {
	(void) m;
}