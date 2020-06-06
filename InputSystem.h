#pragma once
#ifndef _INPUT_SYSTEM_H_
#define _INPUT_SYSTEM_H_

#include <Windows.h>
#include <iostream>
#include "Vector2D.h"

class InputSystem {
	bool currentkey[164];
	bool previouskey[164];
	bool keyreleased[164];
	bool bChange;

	Vector2D initial;
	bool bToggle;
public:
	InputSystem();
	bool CheckCurrentInput(int key);
	bool CheckTriggeredInput(int key);
	bool CheckReleasedInput(int key);

	bool KeyPoll(int& key);
	void UpdateKeyInput(int trigger, int& key);
	char GetKeyValue(int key);
	void GetCursorPosition(Vector2D& pos);
	void GetCursorPositionDelta(Vector2D& pos, int key);
};

extern InputSystem sys_input;

#endif