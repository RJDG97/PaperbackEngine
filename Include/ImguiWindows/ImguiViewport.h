#pragma once
#ifndef _IMGUI_VIEWPORT_H_
#define _IMGUI_VIEWPORT_H_

#include "ImguiWindows/IWindow.h"
#include "Systems/GraphicsSystem.h"
#include "Systems/ImguiSystem.h"

class ImguiViewport : public IWindow
{
public:

	
/******************************************************************************/
/*!
	\fn Init()

	\brief Initializes the ImGui Viewport Window
*/
/******************************************************************************/
	void Init() override;

/******************************************************************************/
/*!
	\fn Update()

	\brief Updates the ImGui Viewport
*/
/******************************************************************************/
	void Update() override;

private:

	// vec2 to store the avaliable viewport size from imgui
	ImVec2 viewport_size_;

	// pointer to the graphics system to get the framebuffer
	GraphicsSystem* editor_viewport_;
	
	//std::vector<const char*> comp_types_{ "Name", "Motion", "Transform", "Health", "Camera",
	//"Controller", "Texture Renderer", "Animation Renderer", "Text Renderer", "AABB", "AI",
	//"Scale", "Status", "Point Light", "Cone Light", "Basic AI", "Clickable", "Input Controller" };
	//const char* component_types_[] = {  };

};

#endif 