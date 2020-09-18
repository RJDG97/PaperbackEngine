#include "InputSystem.h"
#include "WinKeyCodes.h"
#include "WindowsSystem.h"
#include "Core.h"
#include "MenuState.h"

InputSystem sys_input;

InputSystem::InputSystem() : currentkey{ 0 }, 
							 previouskey{ 0 }, 
							 keyreleased{ 0 }, 
							 bChange{ false },
							 bToggle{ false },
							 initial{}
{}

bool InputSystem::CheckCurrentInput(int key) {
	return (GetAsyncKeyState(key));
}

bool InputSystem::CheckTriggeredInput(int key) {
	previouskey[key] = currentkey[key];
	if (GetAsyncKeyState(key)) {
		currentkey[key] = true;
		if (previouskey[key])
			return false;
	}
	else {
		currentkey[key] = false;
		if (previouskey[key])
			keyreleased[key] = true;
		else
			keyreleased[key] = false;
		return false;
	}
	return true;
}

bool InputSystem::CheckReleasedInput(int key) {
	return (keyreleased[key]);
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
	if (sys_input.CheckTriggeredInput(trigger)) {
		std::cout << "Change Key to: ";
		bChange = 1;
	}

	if (bChange) {
		if (sys_input.KeyPoll(key))
		{
			bChange = 0;
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
	if (!bToggle) {
		bToggle = true;
		GetCursorPosition(initial);
	}
	else if (CheckReleasedInput(key)) {
		bToggle = false;
		GetCursorPosition(pos);
		pos.x = max(pos.x, initial.x) - min(pos.x, initial.x);
		pos.x = (pos.x) < 0 ? (-pos.x) : pos.x;
		pos.y = max(pos.y, initial.y) - min(pos.y, initial.y);
		pos.y = (pos.y) < 0 ? (-pos.y) : pos.y;

		std::cout << "delta x: " << pos.x << std::endl;
		std::cout << "delta y: " << pos.y << std::endl;
	}
}

void InputSystem::init() {

}

void InputSystem::update(float frametime) {

	for (int i = 0x30; i < 0x5A; ++i) {
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
				Message_CustomState msg{ &m_MenuState, MessageIDTypes::GSM_ChangeState }; // pass in another existing state?
				CORE->BroadcastMessage(&msg);
				break;
			}
			case 0x32:	//'2'
			{
				Message_CustomState msg{ &m_MenuState, MessageIDTypes::GSM_PushState }; // push maybe game state
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
				break;
			default:
				std::cout << "Input System: [Error] Key is not bound" << std::endl;
			}
		}
	}
}

void InputSystem::SendMessageD(Message* m) {
	(void) m;
}