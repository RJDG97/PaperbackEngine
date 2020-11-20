#include "Script/GeneralScripts.h"
#include "Manager/EntityManager.h"
#include "Engine/Core.h"
#include <cmath>
#include <iostream>

namespace GeneralScripts
{
	EntityID player_id;
	Transform* player_rigidbody;
	Transform* obj_rigidbody;
	Health* player_health;
	Status* player_status;
	AnimationRenderer* obj_anim_renderer;
	AMap* map_;
	ForcesManager* forces_;

	bool Chase(AIIt obj)
	{
		obj->second->SetPlayerLastPos(player_rigidbody->GetPosition());

		// Find current distance of player from obj
		float distance = Vector2DDistance(player_rigidbody->GetPosition(), obj_rigidbody->GetPosition());
		// If obj is close enough, return true
		if (distance < obj->second->GetRange()/5)
			return true;

		//get directional unit vector
		Vector2D directional = player_rigidbody->GetPosition() - obj_rigidbody->GetPosition();
		directional /= Vector2DLength(directional);

		//multiply by speed
		directional *= (obj->second->GetSpeed());

		// Move AI
		forces_->AddForce(obj->second->GetOwner()->GetID(), "movement", PE_FrameRate.GetFixedDelta(), directional);

		return false;
	}

	bool DetectPlayer(AIIt obj)
	{
		if (player_status->GetStatus() != StatusType::BURROW &&
			player_status->GetStatus() != StatusType::INVISIBLE)
		{
			obj->second->SetPlayerLastPos(player_rigidbody->GetPosition());

			// Find current distance of player from obj
			float distance = Vector2DDistance(player_rigidbody->GetPosition(), obj_rigidbody->GetPosition());
			// If Player is very close, is detected
			if (distance < 3.0f)
				return true;
			// Else check if player is in line of sight (May replace check after shadows)
			else if (distance < obj->second->GetRange())
			{
				// Get current direction of object
				Vector2D vector1 = *obj->second->GetCurrentDes() - obj_rigidbody->GetPosition();
				// Get direction of player from object
				Vector2D vector2 = player_rigidbody->GetPosition() - obj_rigidbody->GetPosition();
				// Find the angle of player from current destination
				float angle = std::atan2f(vector2.y, vector2.x) - std::atan2f(vector1.y, vector1.x);
				// Change angle from rad to degrees
				angle *= 180 / 3.14159f;
				// If within view, return detected
				if (angle > -45.0f && angle < 45.0f)
					// Note: will have to check for object obstruction in the line of sight
					return true;
			}
			return false;
		}
		return false;
	}

	void AIHandler(AIIt obj)
	{
		if (!(CORE->GetManager<EntityManager>()->GetPlayerEntities().size() >= 1))
			return;

		// Update player id
		player_id = CORE->GetManager<EntityManager>()->GetPlayerEntities().back()->GetID();
		// Update player rigid body
		player_rigidbody = CORE->GetManager<ComponentManager>()->GetComponent<Transform>(player_id);
		DEBUG_ASSERT((player_rigidbody), "Player does not have Transform component");
		player_status = CORE->GetManager<ComponentManager>()->GetComponent<Status>(player_id);
		player_health = CORE->GetManager<ComponentManager>()->GetComponent<Health>(player_id);
		// Update obj rigid body
		obj_rigidbody = CORE->GetManager<ComponentManager>()->GetComponent<Transform>(obj->first);
		DEBUG_ASSERT((obj_rigidbody), "AI does not have Transform component");
		obj_anim_renderer = CORE->GetManager<ComponentManager>()->GetComponent<AnimationRenderer>(obj->first);

		obj->second->GetTimer().TimerUpdate();
		// Assign type handler
		switch (obj->second->GetType())
		{
		case AI::AIType::StagBeetle:
			StagBeetle::Handler(obj);
			break;
		case AI::AIType::Mite:
			Mite::Handler(obj);
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

	void Patrol(AIIt obj)
	{
		// If path is empty, set path
		if (obj->second->GetPath().empty())
		{
			if (obj_rigidbody->GetPosition().x == obj->second->GetCurrentDes()->x &&
				obj_rigidbody->GetPosition().y == obj->second->GetCurrentDes()->y)
				obj->second->SetCurrentDes(++obj->second->GetCurrentDes());
			// Set new path
			GeneralScripts::map_->Pathing
			(obj->second->GetPath(), obj_rigidbody->GetPosition(), 
				*obj->second->GetCurrentDes());

		}

		if (obj->second->GetPath().size() < 1)
			return;

	// Calculate distance between ai and destination
		float distance = Vector2DLength(obj->second->GetPath().back() - obj_rigidbody->GetPosition());

		// If object is at next path node
		if (distance <= 0.1f) {
			// Remove node destination
			obj->second->GetPath().pop_back();
			// If path is empty, destination is reached
			if (obj->second->GetPath().empty())
			{	// Update next destination
				DestinationIt next_it = obj->second->GetCurrentDes();
				//map->DrawMap();
				// check if next destination is out of range, and loop to beginning if so
				if (++next_it == obj->second->GetDestinations().end())
					//if next destination does not exist, then wrap back to beginning
					next_it = obj->second->GetDestinations().begin();

				// continue to next destination
				obj->second->SetCurrentDes(next_it);
				// Set new path
				GeneralScripts::map_->Pathing
				(obj->second->GetPath(), obj_rigidbody->GetPosition(),
					*obj->second->GetCurrentDes());
			}
		}

		if (obj->second->GetPath().size() < 1)
			return;

		//get directional unit vector
		Vector2D directional = obj->second->GetPath().back() - obj_rigidbody->GetPosition();
		directional /= Vector2DLength(directional);

		//multiply by speed
		directional *= obj->second->GetSpeed();

		// Move AI
		forces_->AddForce(obj->second->GetOwner()->GetID(), "movement", PE_FrameRate.GetFixedDelta(), directional);
	}

}