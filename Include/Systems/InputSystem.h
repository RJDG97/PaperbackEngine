#ifndef _INPUT_SYSTEM_H_
#define _INPUT_SYSTEM_H_

#include <Windows.h>
#include "Systems/ISystem.h"
#include <iostream>
#include "MathLib/Vector2D.h"

#include "GraphicsSystem.h"
#include "Engine/Core.h"

class InputSystem : public ISystem {
	bool debug_;

	int curkeystates[350];
	int prekeystates[350];
	int curmousebuttonstates[3];
	int premousebuttonstates[3];

	GLFWwindow* ptr_window_;
	Vector2D cursor_pos;
	int state = 0, scroll_ = 0;

public:

	void SetKeyState(int keycode, int action);
	void SetMouseState(int button, int action);

	bool IsKeyPressed(int keycode);
	bool IsKeyTriggered(int keycode);
	
	void SetCursorPosition(double xPos, double yPos);
	Vector2D GetCursorPosition();

	bool IsMousePressed(int button);
	bool IsMouseTriggered(int keycode);

	// ISystem
	void Init() override;
	void Update(float frametime) override;
	std::string GetName() override { return "Input System"; }
	void SendMessageD(Message* m) override;
};

extern InputSystem sys_input_;

#endif