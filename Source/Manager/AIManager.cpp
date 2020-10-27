#include "Manager/AIManager.h"
#include <iostream>

void Patrol(AIIt)
{

}

void AIManager::StagBeetleHandler(AIIt ai)
{
	switch (ai->second->state_)
	{
	case AI::AIState::Patrol:
		Patrol(ai);
		break;
	}
}

void AIManager::MiteHandler(AIIt ai)
{
	switch (ai->second->state_)
	{
	case AI::AIState::Patrol:
		Patrol(ai);
		break;
	}
}

void AIManager::HornetHandler(AIIt ai)
{
	switch (ai->second->state_)
	{
	case AI::AIState::Patrol:
		Patrol(ai);
		break;
	}
}

void AIManager::Init()
{
	std::cout << "AIManager" << std::endl;
}

void AIManager::AIHandler(AIIt ai)
{
	switch (ai->second->type_)
	{
	case AI::AIType::StagBeetle:
		StagBeetleHandler(ai);
		break;
	case AI::AIType::Mite:
		MiteHandler(ai);
		break;
	case AI::AIType::Hornet:
		HornetHandler(ai);
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
