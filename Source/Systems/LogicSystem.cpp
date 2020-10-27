#include "Systems/LogicSystem.h"
#include "Components/AI.h"
#include "Systems/Debug.h"

void LogicSystem::AddAIComponent(EntityID id, AI* ai){

	M_DEBUG->WriteDebugMessage("Adding AI Component to entity: " + std::to_string(id) + "\n");

	ai_arr_[id] = ai;
}

void LogicSystem::RemoveAIComponent(EntityID id) {

	AIIt it = ai_arr_.find(id);

	if (it != ai_arr_.end()) {

		M_DEBUG->WriteDebugMessage("Removing AI Component from entity: " + std::to_string(id) + "\n");
		ai_arr_.erase(it);
	}
}

void LogicSystem::Init()
{
	//std::cout << "LogicSystem::Init" << std::endl;
}

void LogicSystem::Update(float frametime)
{
	(void)frametime;
	//std::cout << "LogicSystem::Update" << std::endl;

	for (AIIt ai1 = ai_arr_.begin(); ai1 != ai_arr_.end(); ++ai1) {
		
		// AI type handler
		/*switch (ai1->second->aitype)
		{
		case Enemy_Types::Stag_Beetle:
			switch (ai1->second->state)
			{
			case Logic_States::Patrol:
				break;
			case Logic_States::Detection:
				break;
			case Logic_States::Chase:
				break;
			case Logic_States::Attack:
				break;
			case Logic_States::Find:
				break;
			case Logic_States::Return:
				break;
			}
			break;
		}*/
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