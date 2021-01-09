/**********************************************************************************
*\file         Collectible_Script.h
*\brief        Contains definition of functions and variables used for
*			   the Collectible Script
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#ifndef _BUTTON_SCRIPT_H_
#define _BUTTON_SCRIPT_H_

#include "Entity/Entity.h"
#include "Engine/Core.h"
#include "Systems/GraphicsSystem.h"
#include "Manager/ComponentManager.h"


namespace Button_Script
{

	/******************************************************************************/
	/*!
	  \fn MenuEnterPlay()

	  \brief Texture update script for the Menu to Play State button
	*/
	/******************************************************************************/
	static void MenuEnterPlay(const EntityID& button_id, ButtonStates& state) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
		GraphicsSystem* graphics_sys = &*CORE->GetSystem<GraphicsSystem>();
		Clickable* clickable = component_mgr->GetComponent<Clickable>(button_id);
		TextureRenderer* texture_renderer = component_mgr->GetComponent<TextureRenderer>(button_id);

		if (!texture_renderer)
			return;

		// Assumption is that the Texture Renderer component is GUARANTEED to exist for each button
		switch (state)
		{
			case ButtonStates::DEFAULT:
			{
				graphics_sys->ChangeTexture(texture_renderer, "Menu_Start_0");
				break;
			}
			case ButtonStates::HOVERED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "Menu_Start_Hover_0");
				break;
			}
			case ButtonStates::CLICKED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "Menu_Start_Clicked_0");

				// Temporary measure until the ToggleClickables are sorted out
				Message_Button msg{ clickable->GetIndex() };
				CORE->BroadcastMessage(&msg);

				break;
			}
		}
	}


	/******************************************************************************/
	/*!
	  \fn MenuHowToPlay()

	  \brief Texture update script for the Menu to open HowToPlay guide
	*/
	/******************************************************************************/
	static void MenuHowToPlay(const EntityID& button_id, ButtonStates& state) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
		GraphicsSystem* graphics_sys = &*CORE->GetSystem<GraphicsSystem>();
		Clickable* clickable = component_mgr->GetComponent<Clickable>(button_id);
		TextureRenderer* texture_renderer = component_mgr->GetComponent<TextureRenderer>(button_id);

		if (!texture_renderer)
			return;

		// Assumption is that the Texture Renderer component is GUARANTEED to exist for each button
		switch (state)
		{
			case ButtonStates::DEFAULT:
			{
				graphics_sys->ChangeTexture(texture_renderer, "Menu_HowToPlay_0");

				break;
			}
			case ButtonStates::HOVERED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "Menu_HowToPlay_Hover_0");
				break;
			}
			case ButtonStates::CLICKED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "Menu_HowToPlay_Clicked_0");

				// Parent stuff
				ParentChild* pc = component_mgr->GetComponent<ParentChild>(button_id);
				std::vector<Entity*> children = pc->GetChildren();
				EntityID howtoplay_id = children[0]->GetID();

				// Child stuff
				TextureRenderer* child_renderer = component_mgr->GetComponent<TextureRenderer>(howtoplay_id);
				if (!child_renderer->IsAlive())
					child_renderer->SetAlive(true);


				// Temporary measure until the ToggleClickables are sorted out
				Message_Button msg{ clickable->GetIndex() };
				CORE->BroadcastMessage(&msg);

				break;
			}
		}
	}


	/******************************************************************************/
	/*!
	  \fn MenuEnterEditor()

	  \brief Texture update script for the Menu to Editor State button
	*/
	/******************************************************************************/
	static void MenuEnterEditor(const EntityID& button_id, ButtonStates& state) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
		GraphicsSystem* graphics_sys = &*CORE->GetSystem<GraphicsSystem>();
		Clickable* clickable = component_mgr->GetComponent<Clickable>(button_id);
		TextureRenderer* texture_renderer = component_mgr->GetComponent<TextureRenderer>(button_id);

		if (!texture_renderer)
			return;

		// Assumption is that the Texture Renderer component is GUARANTEED to exist for each button
		switch (state)
		{
			case ButtonStates::DEFAULT:
			{
				graphics_sys->ChangeTexture(texture_renderer, "Menu_Editor_0");
				break;
			}
			case ButtonStates::HOVERED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "Menu_Editor_Hover_0");
				break;
			}
			case ButtonStates::CLICKED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "Menu_Editor_Clicked_0");

				// Temporary measure until the ToggleClickables are sorted out
				Message_Button msg{ clickable->GetIndex() };
				CORE->BroadcastMessage(&msg);

				break;
			}
		}
	}


	/******************************************************************************/
	/*!
	  \fn MenuQuitGame()

	  \brief Texture update script for the Menu to Quit
	*/
	/******************************************************************************/
	static void MenuQuitGame(const EntityID& button_id, ButtonStates& state) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
		GraphicsSystem* graphics_sys = &*CORE->GetSystem<GraphicsSystem>();
		Clickable* clickable = component_mgr->GetComponent<Clickable>(button_id);
		TextureRenderer* texture_renderer = component_mgr->GetComponent<TextureRenderer>(button_id);

		if (!texture_renderer)
			return;

		// Assumption is that the Texture Renderer component is GUARANTEED to exist for each button
		switch (state)
		{
			case ButtonStates::DEFAULT:
			{
				graphics_sys->ChangeTexture(texture_renderer, "Menu_Quit_0");
				break;
			}
			case ButtonStates::HOVERED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "Menu_Quit_Hover_0");
				break;
			}
			case ButtonStates::CLICKED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "Menu_Quit_Clicked_0");

				// Temporary measure until the ToggleClickables are sorted out
				Message_Button msg{ clickable->GetIndex() };
				CORE->BroadcastMessage(&msg);

				break;
			}
		}
	}




	/******************************************************************************/
	/*!
	  \fn PlayPauseGame()

	  \brief Texture update script for the Play State to enter Pause State
	*/
	/******************************************************************************/
	static void PlayPauseGame(const EntityID& button_id, ButtonStates& state) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
		GraphicsSystem* graphics_sys = &*CORE->GetSystem<GraphicsSystem>();
		Clickable* clickable = component_mgr->GetComponent<Clickable>(button_id);
		TextureRenderer* texture_renderer = component_mgr->GetComponent<TextureRenderer>(button_id);

		if (!texture_renderer)
			return;

		// Assumption is that the Texture Renderer component is GUARANTEED to exist for each button
		switch (state)
		{
			case ButtonStates::DEFAULT:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_PauseButton_0");
				break;
			}
			case ButtonStates::HOVERED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_PauseButton_Hover_0");
				break;
			}
			case ButtonStates::CLICKED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_PauseButton_Clicked_0");

				// Temporary measure until the ToggleClickables are sorted out
				Message_Button msg{ clickable->GetIndex() };
				CORE->BroadcastMessage(&msg);

				break;
			}
		}
	}


	/******************************************************************************/
	/*!
	  \fn PlayFullScreen()

	  \brief Texture update script for the Play State to enter Fullscreen
	*/
	/******************************************************************************/
	static void PlayFullScreen(const EntityID& button_id, ButtonStates& state) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
		GraphicsSystem* graphics_sys = &*CORE->GetSystem<GraphicsSystem>();
		Clickable* clickable = component_mgr->GetComponent<Clickable>(button_id);
		TextureRenderer* texture_renderer = component_mgr->GetComponent<TextureRenderer>(button_id);

		if (!texture_renderer)
			return;

		// Assumption is that the Texture Renderer component is GUARANTEED to exist for each button
		switch (state)
		{
			case ButtonStates::DEFAULT:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_FullScreenButton_0");
				break;
			}
			case ButtonStates::HOVERED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_FullScreenButton_Hover_0");
				break;
			}
			case ButtonStates::CLICKED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_FullScreenButton_Clicked_0");

				// Temporary measure until the ToggleClickables are sorted out
				Message_Button msg{ clickable->GetIndex() };
				CORE->BroadcastMessage(&msg);

				break;
			}
		}
	}


	/******************************************************************************/
	/*!
	  \fn PauseResumeGame()

	  \brief Texture update script for the Pause State to resume game
	*/
	/******************************************************************************/
	static void PauseResumeGame(const EntityID& button_id, ButtonStates& state) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
		GraphicsSystem* graphics_sys = &*CORE->GetSystem<GraphicsSystem>();
		Clickable* clickable = component_mgr->GetComponent<Clickable>(button_id);
		TextureRenderer* texture_renderer = component_mgr->GetComponent<TextureRenderer>(button_id);

		if (!texture_renderer)
			return;

		// Assumption is that the Texture Renderer component is GUARANTEED to exist for each button
		switch (state)
		{
			case ButtonStates::DEFAULT:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_Paused_Resume_0");
				break;
			}
			case ButtonStates::HOVERED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_Paused_Resume_Hover_0");
				break;
			}
			case ButtonStates::CLICKED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_Paused_Resume_Clicked_0");

				// Temporary measure until the ToggleClickables are sorted out
				Message_Button msg{ clickable->GetIndex() };
				CORE->BroadcastMessage(&msg);

				break;
			}
		}
	}


	/******************************************************************************/
	/*!
	  \fn PauseHowToPlay()

	  \brief Texture update script for the Pause State to view HowToPlay
	*/
	/******************************************************************************/
	static void PauseHowToPlay(const EntityID& button_id, ButtonStates& state) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
		GraphicsSystem* graphics_sys = &*CORE->GetSystem<GraphicsSystem>();
		Clickable* clickable = component_mgr->GetComponent<Clickable>(button_id);
		TextureRenderer* texture_renderer = component_mgr->GetComponent<TextureRenderer>(button_id);

		if (!texture_renderer)
			return;

		// Assumption is that the Texture Renderer component is GUARANTEED to exist for each button
		switch (state)
		{
			case ButtonStates::DEFAULT:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_Paused_HowTo_0");
				break;
			}
			case ButtonStates::HOVERED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_Paused_HowTo_Hover_0");
				break;
			}
			case ButtonStates::CLICKED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_Paused_HowTo_Clicked_0");

				// Parent stuff
				ParentChild* pc = component_mgr->GetComponent<ParentChild>(button_id);
				std::vector<Entity*> children = pc->GetChildren();
				EntityID howtoplay_id = children[0]->GetID();

				// Child stuff
				TextureRenderer* child_renderer = component_mgr->GetComponent<TextureRenderer>(howtoplay_id);
				if (!child_renderer->IsAlive())
					child_renderer->SetAlive(true);

				// Temporary measure until the ToggleClickables are sorted out
				Message_Button msg{ clickable->GetIndex() };
				CORE->BroadcastMessage(&msg);

				break;
			}
		}
	}


	/******************************************************************************/
	/*!
	  \fn PauseReturnToMenu()

	  \brief Texture update script for the Pause State to return to Menu
	*/
	/******************************************************************************/
	static void PauseReturnToMenu(const EntityID& button_id, ButtonStates& state) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
		GraphicsSystem* graphics_sys = &*CORE->GetSystem<GraphicsSystem>();
		Clickable* clickable = component_mgr->GetComponent<Clickable>(button_id);
		TextureRenderer* texture_renderer = component_mgr->GetComponent<TextureRenderer>(button_id);

		if (!texture_renderer)
			return;

		// Assumption is that the Texture Renderer component is GUARANTEED to exist for each button
		switch (state)
		{
			case ButtonStates::DEFAULT:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_Paused_Return_0");
				break;
			}
			case ButtonStates::HOVERED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_Paused_Return_Hover_0");
				break;
			}
			case ButtonStates::CLICKED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_Paused_Return_Clicked_0");

				// Temporary measure until the ToggleClickables are sorted out
				Message_Button msg{ clickable->GetIndex() };
				CORE->BroadcastMessage(&msg);

				break;
			}
		}
	}


	/******************************************************************************/
	/*!
	  \fn PauseQuitGame()

	  \brief Texture update script for the Pause State to Quit
	*/
	/******************************************************************************/
	static void PauseQuitGame(const EntityID& button_id, ButtonStates& state) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
		GraphicsSystem* graphics_sys = &*CORE->GetSystem<GraphicsSystem>();
		Clickable* clickable = component_mgr->GetComponent<Clickable>(button_id);
		TextureRenderer* texture_renderer = component_mgr->GetComponent<TextureRenderer>(button_id);

		if (!texture_renderer)
			return;

		// Assumption is that the Texture Renderer component is GUARANTEED to exist for each button
		switch (state)
		{
			case ButtonStates::DEFAULT:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_Paused_QuitGame_0");
				break;
			}
			case ButtonStates::HOVERED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_Paused_QuitGame_Hover_0");
				break;
			}
			case ButtonStates::CLICKED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_Paused_QuitGame_Clicked_0");

				// Temporary measure until the ToggleClickables are sorted out
				Message_Button msg{ clickable->GetIndex() };
				CORE->BroadcastMessage(&msg);

				break;
			}
		}
	}


	/******************************************************************************/
	/*!
	  \fn WinLoseReturnToMenu()

	  \brief Texture update script for the WinLose State to Menu
	*/
	/******************************************************************************/
	static void WinLoseReturnToMenu(const EntityID& button_id, ButtonStates& state) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
		GraphicsSystem* graphics_sys = &*CORE->GetSystem<GraphicsSystem>();
		Clickable* clickable = component_mgr->GetComponent<Clickable>(button_id);
		TextureRenderer* texture_renderer = component_mgr->GetComponent<TextureRenderer>(button_id);

		if (!texture_renderer)
			return;

		// Assumption is that the Texture Renderer component is GUARANTEED to exist for each button
		switch (state)
		{
			case ButtonStates::DEFAULT:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_QuitYes_0");
				break;
			}
			case ButtonStates::HOVERED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_QuitYes_Hovered_0");
				break;
			}
			case ButtonStates::CLICKED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_QuitYes_Clicked_0");

				// Temporary measure until the ToggleClickables are sorted out
				Message_Button msg{ clickable->GetIndex() };
				CORE->BroadcastMessage(&msg);

				break;
			}
		}
	}


	/******************************************************************************/
	/*!
	  \fn WinLoseReturnToQuit()

	  \brief Texture update script for the WinLose State to Quit
	*/
	/******************************************************************************/
	static void WinLoseReturnToQuit(const EntityID& button_id, ButtonStates& state) {

		ComponentManager* component_mgr = &*CORE->GetManager<ComponentManager>();
		GraphicsSystem* graphics_sys = &*CORE->GetSystem<GraphicsSystem>();
		Clickable* clickable = component_mgr->GetComponent<Clickable>(button_id);
		TextureRenderer* texture_renderer = component_mgr->GetComponent<TextureRenderer>(button_id);

		if (!texture_renderer)
			return;

		// Assumption is that the Texture Renderer component is GUARANTEED to exist for each button
		switch (state)
		{
			case ButtonStates::DEFAULT:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_QuitNo_0");
				break;
			}
			case ButtonStates::HOVERED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_QuitNo_Hovered_0");
				break;
			}
			case ButtonStates::CLICKED:
			{
				graphics_sys->ChangeTexture(texture_renderer, "UI_QuitNo_Clicked_0");

				// Temporary measure until the ToggleClickables are sorted out
				Message_Button msg{ clickable->GetIndex() };
				CORE->BroadcastMessage(&msg);

				break;
			}
		}
	}
}

#endif