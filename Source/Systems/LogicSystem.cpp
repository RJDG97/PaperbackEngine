#include "Systems/LogicSystem.h"
#include "Script/GeneralScripts.h"
#include "Systems/Debug.h"
#include "Engine/Core.h"

void LogicSystem::Init()
{

	ComponentManager* comp_mgr = &*CORE->GetManager<ComponentManager>();

	ai_arr_ = comp_mgr->GetComponentArray<AI>();
}

void LogicSystem::Update(float frametime)
{
	(void)frametime;
	//std::cout << "LogicSystem::Update" << std::endl;
	for (AIIt ai = ai_arr_->begin(); ai != ai_arr_->end(); ++ai) {

		// AI type handler
		GeneralScripts::AIHandler(ai);

		// any code relevant for all AI

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