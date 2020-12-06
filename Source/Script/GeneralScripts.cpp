/**********************************************************************************
*\file         GeneralScripts.cpp
*\brief        Contains definition of functions and variables used for
*			   generic AI in the game
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#include "Script/GeneralScripts.h"
#include "Manager/EntityManager.h"
#include "Engine/Core.h"
#include <cmath>
#include <iostream>

namespace GeneralScripts
{
	ComponentManager* comp_mgr;

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
		float distance = Vector2DLength(player_rigidbody->GetOffsetAABBPos() - obj_rigidbody->GetOffsetAABBPos());

		if (distance < 2.0f)
			return true;

		// If path is empty, set path
		if (obj->second->GetPath().empty())
		{
			if (obj_rigidbody->GetOffsetAABBPos().x == obj->second->GetCurrentDes()->x &&
				obj_rigidbody->GetOffsetAABBPos().y == obj->second->GetCurrentDes()->y)
				obj->second->SetCurrentDes(++obj->second->GetCurrentDes());
			// Set new path
			GeneralScripts::map_->Pathing(obj->second->GetPath(), obj_rigidbody->GetOffsetAABBPos(), player_rigidbody->GetOffsetAABBPos());
		}

		// Calculate distance between ai and destination
		distance = Vector2DLength(obj->second->GetPath().back() - obj_rigidbody->GetOffsetAABBPos());

		// If object is at next path node
		if (distance < 1.0f) {
			// Remove node destination
			obj->second->GetPath().pop_back();
			// If path is empty, destination is reached
			if (obj->second->GetPath().empty())
			{	
					// Set new path
					GeneralScripts::map_->Pathing(obj->second->GetPath(), obj_rigidbody->GetOffsetAABBPos(), player_rigidbody->GetOffsetAABBPos());
			}
		}

		//get directional unit vector
		Vector2D directional = obj->second->GetPath().back() - obj_rigidbody->GetOffsetAABBPos();
		directional /= Vector2DLength(directional);

		//multiply by speed
		directional *= (obj->second->GetSpeed() * 5.0f);

		// Move AI
		forces_->AddForce(obj->second->GetOwner()->GetID(), "movement", PE_FrameRate.GetFixedDelta(), directional);
		return false;
	}

	bool DetectPlayer(AIIt obj)
	{
		if (player_status->GetStatus() != StatusType::BURROW &&
			player_status->GetStatus() != StatusType::INVISIBLE)
		{
			obj->second->SetPlayerLastPos(player_rigidbody->GetOffsetAABBPos());

			// Find current distance of player from obj
			float distance = Vector2DDistance(player_rigidbody->GetOffsetAABBPos(), obj_rigidbody->GetOffsetAABBPos());
			// If Player is very close, is detected
			if (distance < 3.0f)
				return true;
			// Else check if player is in line of sight (May replace check after shadows)
			else if (distance < obj->second->GetRange())
			{
				// Get current direction of object
				Vector2D vector1 = *obj->second->GetCurrentDes() - obj_rigidbody->GetOffsetAABBPos();
				// Get direction of player from object
				Vector2D vector2 = player_rigidbody->GetOffsetAABBPos() - obj_rigidbody->GetOffsetAABBPos();
				// Find the angle of player from current destination
				float angle = std::atan2f(vector2.y, vector2.x) - std::atan2f(vector1.y, vector1.x);
				// Change angle from rad to degrees
				angle *= 180 / 3.14159f;
				// If within view, return detected
				if (angle > -45.0f && angle < 45.0f)
					// Note: will have to check for object obstruction in the line of sight
					obj->second->GetPath().clear();
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
		player_rigidbody = comp_mgr->GetComponent<Transform>(player_id);
		DEBUG_ASSERT((player_rigidbody), "Player does not have Transform component");
		player_status = comp_mgr->GetComponent<Status>(player_id);
		player_health = comp_mgr->GetComponent<Health>(player_id);
		// Update obj rigid body
		obj_rigidbody = comp_mgr->GetComponent<Transform>(obj->first);
		DEBUG_ASSERT((obj_rigidbody), "AI does not have Transform component");
		obj_anim_renderer = comp_mgr->GetComponent<AnimationRenderer>(obj->first);

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

	std::string ReturnStringType(const AI::AIType& type) {
	
		switch (type)
		{
			case AI::AIType::StagBeetle:
			{
				return "Stag_Beetle";
				break;
			}
			case AI::AIType::Mite:
			{
				return "Mite";
				break;
			}
			case AI::AIType::Hornet:
			{
				return "Hornet";
				break;
			}
			default:
			{
				return "Stag_Beetle";
				break;
			}
		}
	}

	void Patrol(AIIt obj)
	{
		// If path is empty, set path
		if (obj->second->GetPath().empty())
		{
			if (obj_rigidbody->GetOffsetAABBPos().x == obj->second->GetCurrentDes()->x &&
				obj_rigidbody->GetOffsetAABBPos().y == obj->second->GetCurrentDes()->y)
				obj->second->SetCurrentDes(++obj->second->GetCurrentDes());
			// Set new path
			GeneralScripts::map_->Pathing(obj->second->GetPath(), obj_rigidbody->GetOffsetAABBPos(),*obj->second->GetCurrentDes());
		}

		if (obj->second->GetPath().size() < 1)
			return;

	// Calculate distance between ai and destination
		float distance = Vector2DLength(obj->second->GetPath().back() - obj_rigidbody->GetOffsetAABBPos());

		// If object is at next path node
		if (distance < 1.0f) {
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
				GeneralScripts::map_->Pathing(obj->second->GetPath(), obj_rigidbody->GetOffsetAABBPos(),*obj->second->GetCurrentDes());
			}
		}

		if (obj->second->GetPath().size() < 1)
			return;

		//get directional unit vector
		Vector2D directional = obj->second->GetPath().back() - obj_rigidbody->GetOffsetAABBPos();
		directional /= Vector2DLength(directional);

		//multiply by speed
		directional *= obj->second->GetSpeed();

		// Move AI
		forces_->AddForce(obj->second->GetOwner()->GetID(), "movement", PE_FrameRate.GetFixedDelta(), directional);
	}

}