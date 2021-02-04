#ifndef _STAG_TREE_H_
#define _STAG_TREE_H_

#include "Manager/BehaviourTree.h"
#include "Systems/Physics.h"
#include "Engine/Core.h"

class Stag_Tree: public Behaviour
{
public:

	class StagRoot :public Root
	{
		EntityID id_;
	public:
		StagRoot(EntityID id) :id_(id) {
			setChild(new StagSequence(id));
		}

		void CollisionResponse(EntityID obj) override;
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
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;
		Time_Channel respawn_timer_;
	public:
		CheckAlive(EntityID id);

		bool run() override;
	};

	class ActionSelector : public Selector
	{
		EntityID id_;
	public:
		ActionSelector(EntityID id) : id_(id) {
			addChild(new DetectSequence(id_));
			addChild(new IdleSequence(id_));
			addChild(new PatrolSequence(id_));
		}
	};

	class IdleSequence : public Sequence
	{
		EntityID id_;
	public:
		IdleSequence(EntityID id) : id_(id) {
			addChild(new CheckSentry(id_));
			addChild(new SentrySequence(id_));
			addChild(new SentryAnim(id_));
		}
	};

	class CheckSentry :public Node
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;
	public:
		CheckSentry(EntityID id);

		bool run() override;
	};

	class SentrySequence : public Sequence
	{
		EntityID id_;
	public:
		SentrySequence(EntityID id) : id_(id) {
			addChild(new SentryReturn(id_));
			addChild(new CheckPath(id_));
			addChild(new Move(id_, 300.0f));
			addChild(new WalkAnim(id_));
		}
	};

	class SentryReturn :public Node
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;
	public:
		SentryReturn(EntityID id);

		bool run() override;
	};

	class SentryAnim :public Node
	{
		EntityID id_;
		std::shared_ptr<GraphicsSystem> graphics;
		ComponentManager* component_mgr;

		AnimationRenderer* renderer;
		Motion* motion;
		Name* name;
		AI* ai;
	public:
		SentryAnim(EntityID id);

		bool run() override;
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
		AtWaypoint(EntityID id);

		bool run() override;
	};

	class ChangeWaypoint :public Node
	{
		EntityID id_;
		AI* ai_;
		ComponentManager* component_mgr;
	public:
		ChangeWaypoint(EntityID id);

		bool run() override;
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
		WalkAnim(EntityID id);

		bool run() override;
	};

	class CheckPath :public Node
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;
		AMap* map_;
		Vector2D SetDes;
	public:
		CheckPath(EntityID id);
		bool run() override;
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
		Move(EntityID id, float spd);

		bool run() override;
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
			addChild(new PlayerWithinDistance(id_, 3.0f));
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
		PlayerWithinDistance(EntityID id, float dist);

		void PlayerInit();

		bool run() override;
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

		float detectdistance_ = 5.0f;
	public:
		PlayerWithinVision(EntityID id);

		void PlayerInit();

		bool run() override;
	};

	class AttackSelector :public Selector
	{
		EntityID id_;
	public:
		AttackSelector(EntityID id) : id_(id) {
			addChild(new DetectAnim(id_));
			addChild(new ChaseSequence(id_));
			addChild(new AttackSequence(id_));
			addChild(new IdleAnim(id_));
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
		DetectAnim(EntityID id);

		bool run() override;
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
			setChild(new PlayerWithinDistance(id_, 1.0f));
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
		Vector2D SetDes;
	public:
		ChasePath(EntityID id);

		void PlayerInit();

		bool run() override;
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
		ChaseAnim(EntityID id);

		bool run() override;
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
		Charge(EntityID id);
		
		void PlayerInit();

		bool run() override;
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
		AttackAnim(EntityID id);

		bool run() override;
	};

	class IdleAnim :public Node
	{
		EntityID id_;
		std::shared_ptr<GraphicsSystem> graphics;
		ComponentManager* component_mgr;

		AnimationRenderer* renderer;
		Motion* motion;
		Name* name;
		AI* ai_;
	public:
		IdleAnim(EntityID id);

		bool run() override;
	};
};

#endif