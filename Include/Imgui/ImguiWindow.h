#pragma once
#ifndef _IMGUI_WINDOW_H_
#define _IMGUI_WINDOW_H_

#include "ImGui/IWindow.h"

class ImguiWindow : public IWindow
{
	bool b_showdemo;
	bool b_showanother;
	ImVec4 clearcolor_;

public:
	void Init() override;
	void Update() override;
};

#endif 