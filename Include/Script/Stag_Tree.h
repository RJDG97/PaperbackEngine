#ifndef _STAG_TREE_H_
#define _STAG_TREE_H_

#include "Manager/BehaviourTree.h"
#include "Systems/Physics.h"
#include "Engine/Core.h"

class Stag_Tree: public Behaviour
{
public:
	void CollisionResponse()
	{

	}
	class StagRoot :public Root
	{
		EntityID id_;
	public:
		StagRoot(EntityID id) :id_(id) {
			setChild(new StagSequence(id));
		}

		void CollisionResponse(EntityID obj) override {
			
		}
	};

	class StagSequence : public Sequence
	{
		EntityID id_;
	public:
		StagSequence(EntityID id) : id_(id) {
			addChild(new CheckAlive(id));
			addChild(new ActionSelector(id));
		}
	};

	class CheckAlive :public Node
	{
		EntityID id_;
		AI* ai_;
		ComponentManager* component_mgr;
		Time_Channel respawn_timer_;
	public:
		CheckAlive(EntityID id) : id_(id){
			component_mgr = &*CORE->GetManager<ComponentManager>();
			ai_ = component_mgr->GetComponent<AI>(id_);
			respawn_timer_.TimerReset(); 
		}

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
		EntityID id_;
	public:
		ActionSelector(EntityID id) : id_(id) {
			addChild(new DetectSequence(id_));
			addChild(new PatrolSequence(id_));
		}
	};

	class PatrolSequence : public Sequence
	{
		EntityID id_;
	public:
		PatrolSequence(EntityID id) : id_(id) {
			addChild(new CheckWaypoint(id_));
			addChild(new CheckPath(id_));
			addChild(new Move(id_, 300.0f));
			addChild(new WalkAnim(id_));
		}
	};

	class CheckWaypoint :public Selector
	{
		EntityID id_;
	public:
		CheckWaypoint(EntityID id) : id_(id) {
			addChild(new AtWaypoint(id_));
			addChild(new ChangeWaypoint(id_));
		}
	};

	class AtWaypoint :public Node
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;
	public:
		AtWaypoint(EntityID id) : id_(id) {
			component_mgr = &*CORE->GetManager<ComponentManager>();
			ai_ = component_mgr->GetComponent<AI>(id_);
			obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
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
		EntityID id_;
		AI* ai_;
		ComponentManager* component_mgr;
	public:
		ChangeWaypoint(EntityID id) : id_(id) {
			component_mgr = &*CORE->GetManager<ComponentManager>();
			ai_ = component_mgr->GetComponent<AI>(id_);
		}

		bool run() override {
			DestinationIt next_it = ai_->GetCurrentDes();
			if (++next_it == std::end(ai_->GetDestinations()))
				next_it = std::begin(ai_->GetDestinations());
			ai_->SetCurrentDes(next_it);
			return true;
		}
	};

	class WalkAnim :public Node
	{
		EntityID id_;
		std::shared_ptr<GraphicsSystem> graphics;
		ComponentManager* component_mgr;

		AnimationRenderer* renderer;
		Motion* motion;
		Name* name;
		AI* ai;
	public:
		WalkAnim(EntityID id) :id_(id) {
			graphics = CORE->GetSystem<GraphicsSystem>();
			component_mgr = &*CORE->GetManager<ComponentManager>();
			renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
			motion = component_mgr->GetComponent<Motion>(id_);
			name = component_mgr->GetComponent<Name>(id_);
			ai = component_mgr->GetComponent<AI>(id_);
		}

		bool run() override {
			// If any pointers are invalid, return
			if (!renderer || !ai || !motion || !name)
				return false;
			ai->SetState(AI::AIState::Patrol);
			// If velocity is essentially 0, set player to idle
			if (VerifyZeroFloat(motion->GetVelocity().x) && VerifyZeroFloat(motion->GetVelocity().y))
				graphics->ChangeAnimation(renderer, "Stagbeetle_Idle");
			graphics->ChangeAnimation(renderer, "Stagbeetle_Walk");
			
			if (motion->GetVelocity().x > 0 && motion->GetIsLeft()) {
				graphics->FlipTextureY(renderer);
				motion->SetIsLeft(false);
			}
			else if (motion->GetVelocity().x < 0 && !motion->GetIsLeft()) {
				graphics->FlipTextureY(renderer);
				motion->SetIsLeft(true);
			}
			return true;
		}
	};

	class CheckPath :public Node
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;
		AMap* map_;
	public:
		CheckPath(EntityID id) : id_(id) {
			component_mgr = &*CORE->GetManager<ComponentManager>();
			ai_ = component_mgr->GetComponent<AI>(id_);
			obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
			map_ = &*CORE->GetManager<AMap>();
		}

		bool run() override{
			ai_->GetPath().clear();
			return map_->Pathing(ai_->GetPath(), obj_rigidbody_->GetOffsetAABBPos(), *ai_->GetCurrentDes());
		}
	};

	class Move :public Node
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;
		ForcesManager* forces_;
		float Speed_;
	public:
		Move(EntityID id, float spd) : id_(id), Speed_(spd) {
			component_mgr = &*CORE->GetManager<ComponentManager>();
			ai_ = component_mgr->GetComponent<AI>(id_);
			obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
			forces_ = &*CORE->GetManager<ForcesManager>();
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
				directional *= Speed_;

				// Move AI
				forces_->AddForce(id_, "movement", PE_FrameRate.GetFixedDelta(), directional);
				return true;
			}
			return false;
		}
	};

	class DetectSequence :public Sequence
	{
		EntityID id_;
	public:
		DetectSequence(EntityID id) : id_(id) {
			addChild(new DetectPlayer(id_));
			addChild(new AttackSelector(id_));
		}
	};

	class DetectPlayer : public Selector
	{
		EntityID id_;
	public:
		DetectPlayer(EntityID id) : id_(id) {
			addChild(new PlayerWithinDistance(id_, 5.0f));
			addChild(new PlayerWithinVision(id_));
		}
	};

	class PlayerWithinDistance :public Node 
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;

		EntityID player_id_;
		Status* player_status_;
		Transform* player_rigidbody_;

		float detectdistance_;
	public:
		PlayerWithinDistance(EntityID id, float dist)
			: id_(id),
			  detectdistance_(dist){
			component_mgr = &*CORE->GetManager<ComponentManager>();
			ai_ = component_mgr->GetComponent<AI>(id_);
			obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
		}

		void PlayerInit() {
			player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities().back()->GetID();
			player_status_ = component_mgr->GetComponent<Status>(player_id_);
			player_rigidbody_ = component_mgr->GetComponent<Transform>(player_id_);
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
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;

		EntityID player_id_;
		Status* player_status_;
		Transform* player_rigidbody_;

		float detectdistance_ = 10.0f;
	public:
		PlayerWithinVision(EntityID id) : id_(id) {
			component_mgr = &*CORE->GetManager<ComponentManager>();
			ai_ = component_mgr->GetComponent<AI>(id_);
			obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
		}

		void PlayerInit() {
			player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities().back()->GetID();
			player_status_ = component_mgr->GetComponent<Status>(player_id_);
			player_rigidbody_ = component_mgr->GetComponent<Transform>(player_id_);
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

	class AttackSelector :public Selector
	{
		EntityID id_;
	public:
		AttackSelector(EntityID id) : id_(id) {
			addChild(new DetectAnim(id_));
			addChild(new ChaseSequence(id_));
			addChild(new AttackSequence(id_));
		}
	};

	class DetectAnim :public Node
	{
		EntityID id_;
		std::shared_ptr<GraphicsSystem> graphics;
		ComponentManager* component_mgr;

		AnimationRenderer* renderer;
		Motion* motion;
		Name* name;
		AI* ai_;
		Time_Channel Detect_timer_;
	public:
		DetectAnim(EntityID id) :id_(id) {
			graphics = CORE->GetSystem<GraphicsSystem>();
			component_mgr = &*CORE->GetManager<ComponentManager>();
			renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
			motion = component_mgr->GetComponent<Motion>(id_);
			name = component_mgr->GetComponent<Name>(id_);
			ai_ = component_mgr->GetComponent<AI>(id_);
			Detect_timer_.TimerReset();
			Detect_timer_.TimerStop();
		}

		bool run() override {

			// If any pointers are invalid, return
			if (!renderer || !ai_ || !motion || !name)
				return false;

			if (ai_->GetState() == AI::AIState::Patrol) {
				ai_->SetState(AI::AIState::Detected);
				Detect_timer_.TimerStop();
				Detect_timer_.TimerReset();
				Detect_timer_.TimerStart();
				MessageBGM_Play msg{ "EnemyDetect" };
				CORE->BroadcastMessage(&msg);
				graphics->ChangeAnimation(renderer, "Stagbeetle_Alert");
			}

			if (ai_->GetState() == AI::AIState::Detected) {
				// If velocity is essentially 0, set player to idle
				if (VerifyZeroFloat(motion->GetVelocity().x) && VerifyZeroFloat(motion->GetVelocity().y))
					graphics->ChangeAnimation(renderer, "Stagbeetlee_Idle");

				if (motion->GetVelocity().x > 0 && motion->GetIsLeft()) {
					graphics->FlipTextureY(renderer);
					motion->SetIsLeft(false);
				}
				else if (motion->GetVelocity().x < 0 && !motion->GetIsLeft()) {
					graphics->FlipTextureY(renderer);
					motion->SetIsLeft(true);
				}

				if (Detect_timer_.TimeElapsed(s) < 1.0f) {
					Detect_timer_.TimerUpdate();
					return true;
				}
			}
			Detect_timer_.TimerStop();
			Detect_timer_.TimerReset();
			return false;
		}
	};

	class ChaseSequence :public Sequence
	{
		EntityID id_;
	public:
		ChaseSequence(EntityID id) : id_(id) {
			addChild(new NotAtkRange(id_));
			addChild(new ChasePath(id_));
			addChild(new Move(id_, 500.0f));
			addChild(new ChaseAnim(id_));
		}
	};

	class NotAtkRange :public Inverter
	{
		EntityID id_;
	public:
		NotAtkRange(EntityID id) :id_(id) {
			setChild(new PlayerWithinDistance(id_, 2.0f));
		}
	};

	class ChasePath :public Node
	{
		EntityID id_;
		AMap* map_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;

		EntityID player_id_;
		Transform* player_rigidbody_;
	public:
		ChasePath(EntityID id): id_(id) {
			component_mgr = &*CORE->GetManager<ComponentManager>();
			ai_ = component_mgr->GetComponent<AI>(id_);
			obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
			map_ = &*CORE->GetManager<AMap>();
		}

		void PlayerInit() {
			player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities().back()->GetID();
			player_rigidbody_ = component_mgr->GetComponent<Transform>(player_id_);
		}

		bool run() override {
			if (!player_id_)
				PlayerInit();

			ai_->GetPath().clear();
			return map_->Pathing(ai_->GetPath(), obj_rigidbody_->GetOffsetAABBPos(), player_rigidbody_->GetOffsetAABBPos());
		}
	};

	class ChaseAnim :public Node
	{
		EntityID id_;
		std::shared_ptr<GraphicsSystem> graphics;
		ComponentManager* component_mgr;

		AnimationRenderer* renderer;
		Motion* motion;
		Name* name;
		AI* ai_;
	public:
		ChaseAnim(EntityID id) :id_(id) {
			graphics = CORE->GetSystem<GraphicsSystem>();
			component_mgr = &*CORE->GetManager<ComponentManager>();
			renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
			motion = component_mgr->GetComponent<Motion>(id_);
			name = component_mgr->GetComponent<Name>(id_);
			ai_ = component_mgr->GetComponent<AI>(id_);
		}

		bool run() override {

			// If any pointers are invalid, return
			if (!renderer || !ai_ || !motion || !name)
				return false;

			if (ai_->GetState() == AI::AIState::Detected) {
				ai_->SetState(AI::AIState::Chase);
				MessageBGM_Play msg{ "EnemyAttack" };
				CORE->BroadcastMessage(&msg); 
				graphics->ChangeAnimation(renderer, "Stagbeetle_Run");
			}

			if (ai_->GetState() == AI::AIState::Chase) {
				// If velocity is essentially 0, set player to idle
				if (VerifyZeroFloat(motion->GetVelocity().x) && VerifyZeroFloat(motion->GetVelocity().y))
					graphics->ChangeAnimation(renderer, "Stagbeetlee_Idle");

				if (motion->GetVelocity().x > 0 && motion->GetIsLeft()) {
					graphics->FlipTextureY(renderer);
					motion->SetIsLeft(false);
				}
				else if (motion->GetVelocity().x < 0 && !motion->GetIsLeft()) {
					graphics->FlipTextureY(renderer);
					motion->SetIsLeft(true);
				}
			}
			return true;
		}
	};

	class AttackSequence :public Sequence
	{
		EntityID id_;
	public:
		AttackSequence(EntityID id) :id_(id) {
			addChild(new Charge(id_));
			addChild(new AttackAnim(id_));
		}
	};

	class Charge :public Node
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;

		EntityID player_id_;
		Status* player_status_;
		Transform* player_rigidbody_;

		ForcesManager* forces_;
		float Speed_ = 1000.0f;
	public:
		Charge(EntityID id) :id_(id) {
			component_mgr = &*CORE->GetManager<ComponentManager>();
			ai_ = component_mgr->GetComponent<AI>(id_);
			obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
			forces_ = &*CORE->GetManager<ForcesManager>();
		}
		
		void PlayerInit() {
			player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities().back()->GetID();
			player_status_ = component_mgr->GetComponent<Status>(player_id_);
			player_rigidbody_ = component_mgr->GetComponent<Transform>(player_id_);
		}

		bool run() override {
			if (!player_id_)
				PlayerInit();
		
			// Calculate distance between ai and destination
			float distance = Vector2DLength(player_rigidbody_->GetOffsetAABBPos() - obj_rigidbody_->GetOffsetAABBPos());

			// If object is at dest node
			if (distance < 1.0f)
			{
				return false;
			}
			//get directional unit vector
			Vector2D directional = player_rigidbody_->GetOffsetAABBPos() - obj_rigidbody_->GetOffsetAABBPos();
			directional /= Vector2DLength(directional);

			//multiply by speed
			directional *= Speed_;

			// Move AI
			forces_->AddForce(id_, "movement", PE_FrameRate.GetFixedDelta(), directional);
			return true;
		}
	};

	class AttackAnim :public Node
	{
		EntityID id_;
		std::shared_ptr<GraphicsSystem> graphics;
		ComponentManager* component_mgr;

		AnimationRenderer* renderer;
		Motion* motion;
		Name* name;
		AI* ai_;
	public:
		AttackAnim(EntityID id) :id_(id) {
			graphics = CORE->GetSystem<GraphicsSystem>();
			component_mgr = &*CORE->GetManager<ComponentManager>();
			renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
			motion = component_mgr->GetComponent<Motion>(id_);
			name = component_mgr->GetComponent<Name>(id_);
			ai_ = component_mgr->GetComponent<AI>(id_);
		}

		bool run() override {

			// If any pointers are invalid, return
			if (!renderer || !ai_ || !motion || !name)
				return false;

			if (ai_->GetState() == AI::AIState::Chase) {
				ai_->SetState(AI::AIState::Attack);
				MessageBGM_Play msg{ "EnemyAttack" };
				CORE->BroadcastMessage(&msg);
				graphics->ChangeAnimation(renderer, "Stagbeetle_Attack");
			}

			if (ai_->GetState() == AI::AIState::Attack) {
				// If velocity is essentially 0, set player to idle
				if (VerifyZeroFloat(motion->GetVelocity().x) && VerifyZeroFloat(motion->GetVelocity().y))
					graphics->ChangeAnimation(renderer, "Stagbeetlee_Idle");

				if (motion->GetVelocity().x > 0 && motion->GetIsLeft()) {
					graphics->FlipTextureY(renderer);
					motion->SetIsLeft(false);
				}
				else if (motion->GetVelocity().x < 0 && !motion->GetIsLeft()) {
					graphics->FlipTextureY(renderer);
					motion->SetIsLeft(true);
				}
			}
			return true;
		}
	};
};

#endif