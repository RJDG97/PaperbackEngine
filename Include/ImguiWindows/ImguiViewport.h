#pragma once
#ifndef _IMGUI_VIEWPORT_H_
#define _IMGUI_VIEWPORT_H_

#include "ImguiWindows/IWindow.h"
#include "Systems/GraphicsSystem.h"

class ImguiViewport : public IWindow
{
	ImVec2 viewportsize_;
	GraphicsSystem* editorviewport;

public:
	void Init() override;
	void Update() override;
};

#endif 