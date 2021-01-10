#ifndef _STAG_TREE_H_
#define _STAG_TREE_H_

#include "Manager/BehaviourTree.h"

class Stag_Tree
{
public:
	class StagRoot : public Behaviour::Sequence
	{
		AI* ai_;
	public:
		StagRoot(AI* ai): ai_(ai){
			addChild(new CheckAlive(ai_));
			addChild(new DetectPlayer(ai_));
		}

		bool run() const {
			return run();
		}
	};

	class CheckAlive :public Behaviour::Node
	{
		AI* ai_;
		Time_Channel respawn_timer_;
	public:
		CheckAlive(AI* ai) : ai_(ai)
		{ respawn_timer_.TimerReset(); }

		bool run() override
		{
			if (ai_->GetLife())
				return true;
			else
			{
				// If Timer has not started, start
				if (respawn_timer_.TimeElapsed(s) == 0)
					respawn_timer_.TimerStart();
				// Update Timer if timer has not reached respawn time
				if (respawn_timer_.TimeElapsed(s) < 5.0f)
				{
					respawn_timer_.TimerUpdate();
				}
				else // Stop timer and reset to 0
				{
					respawn_timer_.TimerStop();
					respawn_timer_.TimerReset();
					ai_->SetLife(true);
				}
				return false;
			}
		}
	};

	class DetectPlayer : public Behaviour::Selector
	{
		AI* ai_;

	public:
		DetectPlayer(AI* ai) : ai_(ai) {
			//addChild(new PlayerWithinDistance(ai_));
			//addChild(new PlayerWithinVision(ai_));
		}

		bool run() const {
			return run();
		}
	};

	class PlayerWithinDistance :public Behaviour::Node 
	{
		ComponentManager* comp_mgr_;
		AI* ai_;

		EntityID player_id_;
		Status* player_status_;
		Transform* player_rigidbody_;
		Transform* obj_rigidbody_;

		float detectdistance_ = 3.0f;
	public:
		PlayerWithinDistance(AI* ai) :ai_(ai) {
			comp_mgr_ = &*CORE->GetManager<ComponentManager>();
			player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities().back()->GetID();
			player_status_ = comp_mgr_->GetComponent<Status>(player_id_);
			player_rigidbody_ = comp_mgr_->GetComponent<Transform>(player_id_);
			obj_rigidbody_ = comp_mgr_->GetComponent<Transform>(ai_->GetOwner()->GetID());
		}

		bool run() override {
			if (player_status_->GetStatus() != StatusType::BURROW &&
				player_status_->GetStatus() != StatusType::INVISIBLE)
			{
				ai_->SetPlayerLastPos(player_rigidbody_->GetOffsetAABBPos());

				// Find current distance of player from obj
				float distance = Vector2DDistance(player_rigidbody_->GetOffsetAABBPos(), obj_rigidbody_->GetOffsetAABBPos());
				// If Player is very close, is detected
				if (distance < detectdistance_)
					return true;
			}
			return false;
		}
	};

	class PlayerWithinVision : public Behaviour::Node
	{
		ComponentManager* comp_mgr_;
		AI* ai_;

		EntityID player_id_;
		Status* player_status_;
		Transform* player_rigidbody_;
		Transform* obj_rigidbody_;

		float detectdistance_ = 5.0f;
	public:
		PlayerWithinVision(AI* ai) :ai_(ai) {
			comp_mgr_ = &*CORE->GetManager<ComponentManager>();
			player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities().back()->GetID();
			player_status_ = comp_mgr_->GetComponent<Status>(player_id_);
			player_rigidbody_ = comp_mgr_->GetComponent<Transform>(player_id_);
			obj_rigidbody_ = comp_mgr_->GetComponent<Transform>(ai_->GetOwner()->GetID());
		}

		bool run() override {
			if (player_status_->GetStatus() != StatusType::BURROW &&
				player_status_->GetStatus() != StatusType::INVISIBLE)
			{
				ai_->SetPlayerLastPos(player_rigidbody_->GetOffsetAABBPos());

				// Find current distance of player from obj
				float distance = Vector2DDistance(player_rigidbody_->GetOffsetAABBPos(), obj_rigidbody_->GetOffsetAABBPos());

				if (distance < detectdistance_)
				{
					// Get current direction of object
					Vector2D vector1 = *ai_->GetCurrentDes() - obj_rigidbody_->GetOffsetAABBPos();
					// Get direction of player from object
					Vector2D vector2 = player_rigidbody_->GetOffsetAABBPos() - obj_rigidbody_->GetOffsetAABBPos();
					// Find the angle of player from current destination
					float angle = std::atan2f(vector2.y, vector2.x) - std::atan2f(vector1.y, vector1.x);
					// Change angle from rad to degrees
					angle *= 180 / 3.14159f;
					// If within view, return detected
					if (angle > -45.0f && angle < 45.0f)
						// Note: will have to check for object obstruction in the line of sight
						ai_->GetPath().clear();
					return true;
				}
			}
			return false;
		}
	};
};

#endif