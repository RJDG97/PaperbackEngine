#include "Systems/LogicSystem.h"
#include "Script/GeneralScripts.h"
#include "Manager/ForcesManager.h"
#include "Manager/AMap.h"
#include "Systems/Debug.h"
#include "Engine/Core.h"

void LogicSystem::Init()
{
	comp_mgr = &*CORE->GetManager<ComponentManager>();

	ai_arr_ = comp_mgr->GetComponentArray<AI>();
	parentchild_arr_ = comp_mgr->GetComponentArray<ParentChild>();
	
	GeneralScripts::map_ = &*CORE->GetManager<AMap>();
	GeneralScripts::forces_ = &*CORE->GetManager<ForcesManager>();
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

	// Update loop for parent child component currently, update "MyUpdate" to include
	// more functions in the future to handle all game logic updates
	for (auto& [parent_id, parent] : *parentchild_arr_) {

		for (size_t i = 0; i < parent->children_.size(); ++i) {
			
			EntityID child_id = parent->children_[i]->GetID();
			LogicComponent* logic = comp_mgr->GetComponent<LogicComponent>(child_id);

			if (logic)
				logic->MyUpdate(child_id, parent_id);
		}
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