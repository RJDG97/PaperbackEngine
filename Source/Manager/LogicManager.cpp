/**********************************************************************************
*\file         LogicManager.cpp
*\brief        Contains definition of functions and variables used for
*			   the Logic Manager
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Manager/LogicManager.h"
#include "Script/Player_Script.h"
#include "Script/UI_Script.h"
#include "Script/Collectible_Script.h"
#include "Script/Button_Script.h"
#include "Script/Movable_Script.h"
#include "Script/DialogueTrigger_Script.h"
#include "Script/TitleCard_Script.h"
#include "Systems/Message.h"


LogicManager::LogicManager() {
	
	// Constructor
}


void LogicManager::Init() {
	
	// Register Player helper functions
	RegisterLogic("Player_UpdateTexture", &Player_Scripts::TextureUpdateScript);
	RegisterLogic("Player_UpdateChildOffset", &Player_Scripts::UpdateChildOffset);
	RegisterLogic("Player_UpdateInput", &Player_Scripts::PlayerControllerScript);
	RegisterLogic("Player_Collectible", &Player_Scripts::CollectedCollectible);

	// Register Burrow UI helper function
	RegisterLogic("BurrowIcon_UpdateTexture", &UI_Script::BurrowUI_TextureUpdateScript);

	// Register Hide UI helper function
	RegisterLogic("HideIcon_UpdateTexture", &UI_Script::HideUI_TextureUpdateScript);

	// Register Environment Collectible helper function
	RegisterLogic("Environment_Collectible", &Collectible_Script::CollectedCollectible);
	RegisterLogic("Interactable_Response", &Collectible_Script::InteractableResponse);

	// Register MenuState Button helper functions
	RegisterLogic("MenuState_EnterPlayState", &Button_Script::MenuEnterPlay);
	RegisterLogic("MenuState_HowToPlay", &Button_Script::MenuHowToPlay);
	RegisterLogic("MenuState_EnterEditorState", &Button_Script::MenuEnterEditor);
	RegisterLogic("MenuState_QuitGame", &Button_Script::MenuQuitGame);
	RegisterLogic("MenuState_Credits", &Button_Script::MenuCredits);
	RegisterLogic("MenuState_TitleCard", &TitleCard_Script::TitleCardAnim);
	RegisterLogic("MenuState_TitleText", &TitleCard_Script::TitleTextAnim);

	// Register PauseState Button helper functions
	RegisterLogic("PlayState_PauseGame", &Button_Script::PlayPauseGame);
	RegisterLogic("PlayState_EnterFullScreen", &Button_Script::PlayFullScreen);
	RegisterLogic("PauseState_ResumeGame", &Button_Script::PauseResumeGame);
	RegisterLogic("PauseState_HowToPlay", &Button_Script::PauseHowToPlay);
	RegisterLogic("PauseState_ReturnToMenu", &Button_Script::PauseReturnToMenu);
	RegisterLogic("PauseState_QuitGame", &Button_Script::PauseQuitGame);
	RegisterLogic("PauseState_QuitGameYes", &Button_Script::PauseQuitGameYes);
	RegisterLogic("PauseState_QuitGameNo", &Button_Script::PauseQuitGameNo);
	RegisterLogic("Button_WinAnimation", &Button_Script::PauseWinAnim);
	RegisterLogic("Button_LoseAnimation", &Button_Script::PauseLoseAnim);
	RegisterLogic("WinLoseState_EnterMenuState", &Button_Script::WinLoseReturnToMenu);
	RegisterLogic("WinLoseState_QuitGame", &Button_Script::WinLoseReturnToQuit);

	// Environmental entity helper functions
	RegisterLogic("Environmental_AnimUpdate", &Movable_Script::UpdateMovable);

	// For Dialogue triggers
	RegisterLogic("TriggerDialogue", &DialogueTrigger_Script::TriggerDialogue);
}