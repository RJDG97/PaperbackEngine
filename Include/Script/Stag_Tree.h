#ifndef _STAG_TREE_H_
#define _STAG_TREE_H_

#include "Manager/BehaviourTree.h"

class Stag_Tree: public Behaviour
{
public:
	class StagRoot : public Sequence
	{
		AI* ai_;
	public:
		StagRoot(AI* ai): ai_(ai){
			addChild(new CheckAlive(ai_));
			addChild(new ActionSelector(ai_));
		}

		bool run() const {
			return run();
		}
	};

	class CheckAlive :public Node
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

	class ActionSelector : public Selector
	{
		AI* ai_;
	public:
		ActionSelector(AI* ai) :ai_(ai) {
			addChild(new AttackSequence(ai_));
			addChild(new PatrolSequence(ai_));
		}

		bool run() const {
			return run();
		}
	};

	class PatrolSequence : public Sequence
	{
		AI* ai_;
	public:
		PatrolSequence(AI* ai) : ai_(ai) {
			addChild(new CheckWaypoint(ai_));
			addChild(new CheckPath(ai_));
			addChild(new WaypointMove(ai_));
		}

		bool run() const {
			return run();
		}
	};

	class CheckWaypoint :public Selector
	{
		AI* ai_;
	public:
		CheckWaypoint(AI* ai) : ai_(ai) {
			addChild(new NotAtWaypoint(ai_));
			addChild(new ChangeWaypoint(ai_));
		}

		bool run() const {
			return run();
		}
	};

	class NotAtWaypoint :public Node
	{
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* comp_mgr_;
	public:
		NotAtWaypoint(AI* ai) :ai_(ai) {
			comp_mgr_ = &*CORE->GetManager<ComponentManager>();
			obj_rigidbody_ = comp_mgr_->GetComponent<Transform>(ai_->GetOwner()->GetID());
		}

		bool run() override{
			float distance = Vector2DLength(*ai_->GetCurrentDes() - obj_rigidbody_->GetOffsetAABBPos());
			// If object is at next path node
			if (distance < 1.0f || ai_->GetPath().empty())
				return false;
			return true;
		}
	};

	class ChangeWaypoint :public Node
	{
		AI* ai_;
	public:
		ChangeWaypoint(AI* ai) :ai_(ai) {}

		bool run() override {
			DestinationIt next_it = ai_->GetCurrentDes();
			if (++next_it == std::end(ai_->GetDestinations()))
				next_it = std::begin(ai_->GetDestinations());
			ai_->SetCurrentDes(next_it);
			return true;
		}
	};

	class CheckPath :public Node
	{
		AI* ai_;
		AMap* map_;
		Transform* obj_rigidbody_;
		ComponentManager* comp_mgr_;
	public:
		CheckPath(AI* ai) : ai_(ai) {
			map_ = &*CORE->GetManager<AMap>();
			comp_mgr_ = &*CORE->GetManager<ComponentManager>();
			obj_rigidbody_ = comp_mgr_->GetComponent<Transform>(ai_->GetOwner()->GetID());
		}

		bool run() override{
			ai_->GetPath().clear();
			return map_->Pathing(ai_->GetPath(), obj_rigidbody_->GetOffsetAABBPos(), *ai_->GetCurrentDes());
		}
	};

	class WaypointMove :public Node
	{
		AI* ai_;
		ForcesManager* forces_;
		Transform* obj_rigidbody_;
		ComponentManager* comp_mgr_;
	public:
		WaypointMove(AI* ai): ai_(ai){
			forces_ = &*CORE->GetManager<ForcesManager>();
			comp_mgr_ = &*CORE->GetManager<ComponentManager>();
			obj_rigidbody_ = comp_mgr_->GetComponent<Transform>(ai_->GetOwner()->GetID());
		}

		bool run() override{
			// Calculate distance between ai and destination
			float distance = Vector2DLength(ai_->GetPath().back() - obj_rigidbody_->GetOffsetAABBPos());

			// If object is at next path node
			if (distance < 1.0f) 
				// Remove node destination
				ai_->GetPath().pop_back();
			if (!ai_->GetPath().empty())
			{
				//get directional unit vector
				Vector2D directional = ai_->GetPath().back() - obj_rigidbody_->GetOffsetAABBPos();
				directional /= Vector2DLength(directional);

				//multiply by speed
				directional *= ai_->GetSpeed();

				// Move AI
				forces_->AddForce(ai_->GetOwner()->GetID(), "movement", PE_FrameRate.GetFixedDelta(), directional);
				return true;
			}
			return false;
		}
	};

	class AttackSequence :public Sequence
	{
		AI* ai_;
	public:
		AttackSequence(AI* ai) : ai_(ai) {
			addChild(new DetectPlayer(ai_));
			addChild(new AttackPlayer(ai_));
		}

		bool run() const {
			return run();
		}
	};

	class DetectPlayer : public Selector
	{
		AI* ai_;

	public:
		DetectPlayer(AI* ai) : ai_(ai) {
			addChild(new PlayerWithinDistance(ai_));
			addChild(new PlayerWithinVision(ai_));
		}

		bool run() const {
			return run();
		}
	};

	class PlayerWithinDistance :public Node 
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
			obj_rigidbody_ = comp_mgr_->GetComponent<Transform>(ai_->GetOwner()->GetID());
		}

		void PlayerInit() {
			player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities().back()->GetID();
			player_status_ = comp_mgr_->GetComponent<Status>(player_id_);
			player_rigidbody_ = comp_mgr_->GetComponent<Transform>(player_id_);
		}

		bool run() override {
			if (!player_id_)
				PlayerInit();

			if (player_status_->GetStatus() != StatusType::BURROW &&
				player_status_->GetStatus() != StatusType::INVISIBLE)
			{
				ai_->SetPlayerLastPos(player_rigidbody_->GetOffsetAABBPos());

				// Find current distance of player from obj
				float distance = Vector2DDistance(player_rigidbody_->GetOffsetAABBPos(), obj_rigidbody_->GetOffsetAABBPos());
				// If Player is very close, is detected
				if (distance < detectdistance_) {
					return true;
				}
			}
			return false;
		}
	};

	class PlayerWithinVision : public Node
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
			obj_rigidbody_ = comp_mgr_->GetComponent<Transform>(ai_->GetOwner()->GetID());
		}

		void PlayerInit() {
			player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities().back()->GetID();
			player_status_ = comp_mgr_->GetComponent<Status>(player_id_);
			player_rigidbody_ = comp_mgr_->GetComponent<Transform>(player_id_);
		}

		bool run() override {
			if (!player_id_)
				PlayerInit();

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

	class AttackPlayer :public Sequence
	{
		AI* ai_;
	public:
		AttackPlayer(AI* ai) :ai_(ai) {
		
		}

		bool run() const {
			return run();
		}
	};
};

#endif