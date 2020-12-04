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
#include "Script/Mite_Script.h"
#include "Script/Stag_Script.h"
#include "Script/Player_Script.h"
#include "Script/Button_Script.h"
#include "Systems/Message.h"


LogicManager::LogicManager() {
	
	// Constructor
}


void LogicManager::Init() {
	
	// Register player helper functions
	RegisterLogic("Player_UpdateTexture", &Player_Scripts::TextureUpdateScript);
	RegisterLogic("Player_UpdateChildOffset", &Player_Scripts::UpdateChildOffset);
	RegisterLogic("Player_UpdateInput", &Player_Scripts::PlayerControllerScript);

	// Register stag helper functions
	RegisterLogic("Stag_UpdateTexture", &Stag_Script::TextureUpdateScript);

	// Register mite helper functions
	RegisterLogic("Mite_UpdateTexture", &Mite_Script::TextureUpdateScript);

	// Register Burrow UI button helper function
	RegisterLogic("BurrowIcon_UpdateTexture", &Button_Script::BurrowUI_TextureUpdateScript);

	// Register Burrow UI button helper function
	RegisterLogic("HideIcon_UpdateTexture", &Button_Script::HideUI_TextureUpdateScript);
}