#include "Script/GeneralScripts.h"
#include "Manager/ForcesManager.h"
#include "Engine/Core.h"
#include <iostream>

namespace GeneralScripts
{

	void Patrol(AIIt ai)
	{

		Transform* xform = CORE->GetManager<ComponentManager>()->GetComponent<Transform>(ai->first);
		DEBUG_ASSERT((xform), "AI does not have Transform component");

		// Check if entity close to destination aka point box collision

		// Calculate distance between ai and destination
		float distance = Vector2DLength(*(ai->second->GetCurrentDes()) - xform->GetPosition());

		// if ai is near then calculate new vector and set
		if (distance <= 0.5f) {

			DestinationIt next_it = ++(ai->second->GetCurrentDes());
			
			// check if next destination is out of range, and loop to beginning if so
			if (next_it == ai->second->GetDestinations().end())
				//if next destination does not exist, then wrap back to beginning
				next_it = ai->second->GetDestinations().begin();

			// continue to next destination
			ai->second->SetCurrentDes(next_it);
			
		}

		//get directional unit vector
		Vector2D directional = *ai->second->GetCurrentDes() - xform->GetPosition();
		directional /= Vector2DLength(directional);

		//multiply by speed
		directional *= ai->second->GetSpeed();

		// Move AI
		CORE->GetManager<ForcesManager>()->AddForce(ai->second->GetOwner()->GetID(), "movement", PE_FrameRate.GetFixedDelta(), directional);
	}

	void AIHandler(AIIt obj)
	{
		//obj_ = CORE->GetManager<ComponentManager>()->GetComponent<AI>(obj->first);
		switch (obj->second->GetState())
		{
		case AI::AIType::StagBeetle:
			GeneralScripts::Patrol(obj);
			break;
		case AI::AIType::Mite:
			break;
		case AI::AIType::Hornet:
			break;
		}
	}

	AI::AIType GetType(std::string type)
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

}