#pragma once
#ifndef _IWINDOW_H_
#define _IWINDOW_H_

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

class IWindow
{
	public:
		virtual void Init() {};
		virtual void Update() = 0;
		virtual ~IWindow() = default;
};

#endif