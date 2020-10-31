#include "Manager/AIManager.h"
#include "Script/ScriptList.h"

void AIManager::Init()
{
	//std::cout << "AIManager" << std::endl;
}

void AIManager::AIHandler(AIIt ai)
{
	switch (ai->second->type_)
	{
	case AI::AIType::StagBeetle:
		StagBeetle.StagBeetleHandler(ai);
		break;
	case AI::AIType::Mite:
		mite.MiteHandler(ai);
		break;
	case AI::AIType::Hornet:
		hornet.HornetHandler(ai);
		break;
	}
}

AI::AIType AIManager::GetType(std::string type)
{
	if (type == "Stag_Beetle")
		return AI::AIType::StagBeetle;
	else if (type == "Mite")
		return AI::AIType::Mite;
	else if (type == "Hornet")
		return AI::AIType::Hornet;
	// replace with exception
	return AI::AIType::StagBeetle;
}
