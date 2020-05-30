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
		change = 1;
	}

	if (change) {
		if (sys_input.KeyPoll(key))
			change = 0;
	}
}