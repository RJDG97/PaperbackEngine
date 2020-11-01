#include "Systems/LogicSystem.h"
#include "Components/AI.h"
#include "Manager/AIManager.h"
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

	for (AIIt ai1 = ai_arr_->begin(); ai1 != ai_arr_->end(); ++ai1) {
		
		// AI type handler
		CORE->GetManager<AIManager>()->AIHandler(ai1);

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