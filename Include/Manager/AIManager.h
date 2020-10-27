#ifndef _AI_MANAGER_H_
#define _AI_MANAGER_H_

#include "Manager/IManager.h"
#include "Systems/LogicSystem.h"
#include "Components/AI.h"

class AIManager : public IManager {

public:

	void Init() override;

	void AIHandler(AIIt ai);

	AI::AIType GetType(std::string type);

	void StagBeetleHandler(AIIt ai);

	void MiteHandler(AIIt ai);

	void HornetHandler(AIIt ai);

};

#endif
