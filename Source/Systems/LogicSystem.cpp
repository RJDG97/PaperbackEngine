/**********************************************************************************
*\file         LogicSystem.cpp
*\brief        Contains definition of functions and variables used for the LogicSystem
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Systems/LogicSystem.h"
//#include "Script/GeneralScripts.h"
#include "Manager/ForcesManager.h"
//#include "Manager/AMap.h"
#include "Systems/Debug.h"
#include "Engine/Core.h"

#include "Script/Stag_Tree.h"

void LogicSystem::Init()
{
	comp_mgr = &*CORE->GetManager<ComponentManager>();
	//GeneralScripts::comp_mgr = &*CORE->GetManager<ComponentManager>();

	ai_arr_ = comp_mgr->GetComponentArray<AI>();

	//GeneralScripts::map_ = &*CORE->GetManager<AMap>();
	//GeneralScripts::forces_ = &*CORE->GetManager<ForcesManager>();
}

void LogicSystem::Update(float frametime)
{
	(void)frametime;
	for (AIIt ai = ai_arr_->begin(); ai != ai_arr_->end(); ++ai) {

		// AI type handler
		//GeneralScripts::AIHandler(ai);

		// Run AI Behaviour Tree
		ai->second->root_.run();
	}
}

void LogicSystem::Draw()
{
	//std::cout << "LogicSystem::Draw" << std::endl;
}

void LogicSystem::SendMessageD(Message *m)
{
	(void)m;
	//std::cout << "LogicSystem::SendMessageD" << std::endl;
}