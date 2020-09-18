#pragma once
#ifndef _INPUT_SYSTEM_H_
#define _INPUT_SYSTEM_H_

#include <Windows.h>
#include "ISystem.h"
#include <iostream>
#include "Vector2D.h"

class InputSystem : public ISystem {
	bool currentkey[164];
	bool previouskey[164];
	bool keyreleased[164];
	bool bChange;
	bool bToggle;
	Vector2D initial;
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

	// ISystem
	void init();
	void update(float frametime);
	std::string GetName() { return "Input System"; }
	void SendMessageD(Message* m);
};

extern InputSystem sys_input;

#endif