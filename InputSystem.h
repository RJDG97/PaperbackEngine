#pragma once
#ifndef _INPUT_SYSTEM_H_
#define _INPUT_SYSTEM_H_

#include <Windows.h>
#include <iostream>

class InputSystem {
	bool currentkey[164];
	bool previouskey[164];
	bool keyreleased[164];
	bool change;

public:
	InputSystem();
	bool CheckCurrentInput(int key);
	bool CheckTriggeredInput(int key);
	bool CheckReleasedInput(int key);

	// Not particularly sure
	bool KeyPoll(int& key);
	void UpdateKeyInput(int trigger, int& key);
	char GetKeyValue(int key);

};

extern InputSystem sys_input;

#endif