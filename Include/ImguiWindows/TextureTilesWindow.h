#pragma once
#ifndef _TEXTURE_TILES_WINDOW_H_
#define _TEXTURE_TILES_WINDOW_H_

#include "ImguiWindows/IWindow.h"
#include "Systems/ImguiSystem.h"
#include "Manager/EntityManager.h"
#include "Manager/ComponentManager.h"
#include "Systems/Factory.h"

class TextureTilesWindow : public IWindow
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

	ImguiSystem* imgui_;
	EntityManager* entities_;
	TextureManager* texture_;
	GraphicsSystem* graphics_;
	AnimationManager* animation_;

	std::string selectedtex_;
};
#endif
