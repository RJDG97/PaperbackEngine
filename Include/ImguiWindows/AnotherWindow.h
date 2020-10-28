#pragma once
#ifndef _ANOTHER_WINDOW_H_
#define _ANOTHER_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Components/Transform.h"
#include "Components/Scale.h"
#include <Windows.h>
#include "Systems/ImguiSystem.h"
#include "Systems/Collision.h"
#include "Engine/Core.h"


class AnotherWindow : public IWindow{

public:
	void Init() override;
	void Update() override;
	void Component(std::pair<Entity*, std::vector<ComponentTypes>>);


private:
	ImguiSystem* imgui_system_;

};
#endif
