#pragma once
#ifndef _ANOTHER_WINDOW_H_
#define _ANOTHER_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Components/Transform.h"
#include <Windows.h>
#include "Systems/ImguiSystem.h"
#include "Components/Motion.h"
#include "Engine/Core.h"


class AnotherWindow : public IWindow{

public:
	void Init() override;
	void Update() override;

private:
ImguiSystem* imgui_system_;

};
#endif
