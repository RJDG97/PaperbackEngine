#ifndef _AI_MANAGER_H_
#define _AI_MANAGER_H_

#include "Engine/Core.h"
#include "Components/AI.h"
#include "Manager/IManager.h"
#include "Systems/LogicSystem.h"
#include "Components/Transform.h"
#include "Manager/ForcesManager.h"
#include "Systems/FrameRateController.h"

class AIManager : public IManager {

public:

	void Init() override;

	void AIHandler(AIIt ai);

	AI::AIType GetType(std::string type);

};

#endif
