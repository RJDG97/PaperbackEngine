#pragma once
#ifndef _INPUT_SYSTEM_H_
#define _INPUT_SYSTEM_H_

#include <Windows.h>
#include "Systems/ISystem.h"
#include <iostream>
#include "MathLib/Vector2D.h"

class InputSystem : public ISystem {
	bool currentkey_[164];
	bool previouskey_[164];
	bool keyreleased_[164];
	bool b_change_;
	bool b_toggle_;
	Vector2D initial_;
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
	void Init() override;
	void Update(float frametime) override;
	std::string GetName() override { return "Input System"; }
	void SendMessageD(Message* m) override;
};

extern InputSystem sys_input_;

#endif