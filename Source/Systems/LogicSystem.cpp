/**********************************************************************************
*\file         LogicSystem.cpp
*\brief        Contains definition of functions and variables used for the LogicSystem
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Systems/LogicSystem.h"
#include "Manager/ForcesManager.h"
#include "Systems/Debug.h"
#include "Engine/Core.h"

void LogicSystem::Init()
{
	comp_mgr = &*CORE->GetManager<ComponentManager>();

	ai_arr_ = comp_mgr->GetComponentArray<AI>();
}

void LogicSystem::Update(float frametime)
{
	(void)frametime;
	for (AIIt ai = ai_arr_->begin(); ai != ai_arr_->end(); ++ai) {
		// Run AI Behaviour Tree
		if(ai->second->root_)
			ai->second->root_->run();
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