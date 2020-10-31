#include "Script/GeneralScripts.h"
#include <iostream>

void Patrol(AIIt ai)
{
	std::cout << "Patrol" << std::endl;
	ai->second->SetState(AI::AIState::Attack);
}