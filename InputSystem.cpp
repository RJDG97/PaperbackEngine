#include "InputSystem.h"

InputSystem sys_input;

InputSystem::InputSystem() : currentkey{ 0 }, previouskey{ 0 }, keyreleased{ 0 }, change{ 0 } {}

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
		change = 1;
	}

	if (change) {
		if (sys_input.KeyPoll(key))
		{
			change = 0;
			std::cout << GetKeyValue(key) << std::endl;
		}
	}
}

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