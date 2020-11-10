#pragma once
#ifndef _SYSTEM_WINDOW_H_
#define _SYSTEM_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"

class SystemWindow : public IWindow {
public:
	void Init() override;
	void Update() override;

private:
	ImguiSystem* imgui_;
};

#endif

