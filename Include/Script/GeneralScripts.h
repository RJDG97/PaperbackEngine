#ifndef _GENERALSCRIPT_H_
#define _GENERALSCRIPT_H_

#include "Manager/AIManager.h"

class GeneralScripts : protected AI
{
public:

	bool Chase(AIIt ai)
	{
		/*
			Move towards player location
		*/
		return false;
	}

	bool DetectPlayer(AIIt ai)
	{
		/* 
			Check against player location 
			if(Player is detected)
				state = detected;
		*/
		return false;
	}

	void Patrol(AIIt ai)
	{
		std::shared_ptr<Transform> xform =
			std::dynamic_pointer_cast<Transform>(ai->second->GetOwner()->GetComponent(ComponentTypes::TRANSFORM));
		DEBUG_ASSERT((xform), "AI does not have Transform component");

		// Check if entity close to destination aka point box collision

		// Calculate distance between ai and destination
		float distance = Vector2DLength(*(ai->second->current_destination_) - xform->GetPosition());
		if (distance <= 0.5f) {

			// if ai is near then calculate new vector and set
			// check if next destination is out of range, and loop to beginning if so
			AI::DestinationIt next_it = ai->second->current_destination_;

			if (++next_it == std::end(ai->second->destinations_)) {

				//if next destination does not exist, then wrap back to beginning
				next_it = ai->second->destinations_.begin();
			}

			ai->second->current_destination_ = next_it;
		}

		//get directional unit vector
		Vector2D directional = *ai->second->current_destination_ - xform->GetPosition();
		directional /= Vector2DLength(directional);

		//multiply by speed
		directional *= ai->second->speed_;

		// Move AI
		CORE->GetManager<ForcesManager>()->AddForce(ai->second->GetOwner()->GetID(), "movement", PE_FrameRate.GetFixedDelta(), directional);
	}

}GS;


#endif