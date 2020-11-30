#include "Manager/LogicManager.h"
#include "Script/Mite_Script.h"
#include "Script/Stag_Script.h"
#include "Script/Player_Script.h"
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
}