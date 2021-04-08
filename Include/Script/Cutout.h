#ifndef _CUTOUT_H_
#define _CUTOUT_H_

#include "Script/Common_Nodes.h"
#include "Entity/ComponentTypes.h"

class Cutout : public Behaviour
{
public:

	class CutoutRoot : public Root
	{
		EntityID id_;
	public:
		CutoutRoot(EntityID id) : id_(id) {
			setChild(new ActionSequence(id));
		}

		void CollisionResponse(EntityID obj) override
		{}
	};

	class ActionSequence : public Sequence
	{
		EntityID id_;
	public:
		ActionSequence(EntityID id) : id_(id) {
			addChild(new DetectPlayer(id_));
			addChild(new Detected(id_));
		}
	};

	class DetectPlayer :public Selector
	{
		EntityID id_;
	public:
		DetectPlayer(EntityID id) : id_(id) {
			addChild(new Common::PlayerWithinDistance(id_, 3.0f));
			addChild(new PlayerWithinVision(id_, 4.5f));
			addChild(new Flip(id_));
		}
	};

	class PlayerWithinVision : public Node
	{
		EntityID id_;
		AI* ai_;
		Transform* obj_rigidbody_;
		ComponentManager* component_mgr;
		Motion* motion;
		EntityID player_id_;
		Status* player_status_;
		Transform* player_rigidbody_;

		float detectdistance_;
	public:
		PlayerWithinVision(EntityID id, float dist)
			: id_(id),
			detectdistance_(dist) 
		{
			component_mgr = &*CORE->GetManager<ComponentManager>();
			ai_ = component_mgr->GetComponent<AI>(id_);
			obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
			motion = component_mgr->GetComponent<Motion>(id_);
		}

		void PlayerInit()
		{
			player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities()->GetID();
			player_status_ = component_mgr->GetComponent<Status>(player_id_);
			player_rigidbody_ = component_mgr->GetComponent<Transform>(player_id_);
		}

		bool run() override {
			if (!player_id_)
				PlayerInit();

			if (player_status_->GetStatus() != StatusType::INVISIBLE)
			{
				if (player_status_->GetStatus() == StatusType::BURROW)
					return false;

				float distance = Vector2DDistance(player_rigidbody_->GetOffsetAABBPos(), obj_rigidbody_->GetOffsetAABBPos());

				if (distance < detectdistance_)
				{
					Vector2D vector1;
					if (motion->IsLeft()) {
						vector1 = (Vector2D{ obj_rigidbody_->GetOffsetAABBPos().x - 4.0f, obj_rigidbody_->GetOffsetAABBPos().y}) - obj_rigidbody_->GetOffsetAABBPos();
					}
					else
						vector1 = (Vector2D{ obj_rigidbody_->GetOffsetAABBPos().x + 4.0f, obj_rigidbody_->GetOffsetAABBPos().y }) - obj_rigidbody_->GetOffsetAABBPos();
					
					// Get direction of player from object
					Vector2D vector2 = player_rigidbody_->GetOffsetAABBPos() - obj_rigidbody_->GetOffsetAABBPos();
					// Find the angle of player from current destination
					float angle = std::atan2f(vector2.y, vector2.x) - std::atan2f(vector1.y, vector1.x);
					// Change angle from rad to degrees
					angle *= 180 / 3.14159f;
					// If within view, return detected
					if (angle > -45.0f && angle < 45.0f)
						return true;
				}
			}
			return false;
		}
	};

	class Detected :public Node
	{
		EntityID id_;
		ComponentManager* component_mgr;
		AI* ai_;
		ParentChild* pc;
		PointLight* pt;
		ConeLight* cone;

		std::shared_ptr<GraphicsSystem> graphics;
		AnimationRenderer* renderer;
	public:
		Detected(EntityID id) : id_(id) {
			component_mgr = &*CORE->GetManager<ComponentManager>();
			ai_ = component_mgr->GetComponent<AI>(id_);
			graphics = CORE->GetSystem<GraphicsSystem>();
			renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
		}

		bool run() override
		{
			if (ai_->GetState() != AI::AIState::Detected) {
				ai_->SetState(AI::AIState::Detected);
				graphics->ChangeAnimation(renderer, "CutOut_Detect");
				renderer->SetReversed(false);
				renderer->SetAnimationStatus(true);
				if (!pc) {
					pc = component_mgr->GetComponent<ParentChild>(id_);
					auto& children = pc->GetChildren();

					for (auto& child : children) {
						cone = component_mgr->GetComponent<ConeLight>(child->GetID());
						pt = component_mgr->GetComponent<PointLight>(child->GetID());
					}
				}

				if (cone)
					cone->SetColor({ 1, 0, 0 });
				if (pt)
					pt->SetColor({ 1, 0, 0 });
			}
			else if(renderer->FinishedAnimating()){
				renderer->SetAnimationStatus(false);
			}
			return true;
		}
	};

	class Flip :public Node
	{
		EntityID id_;
		EntityID child_id_;
		ComponentManager* component_mgr;
		AI* ai_;

		ParentChild* pc;
		PointLight* pt;
		ConeLight* cone;

		std::shared_ptr<GraphicsSystem> graphics;
		Motion* motion;
		AnimationRenderer* renderer;
		Time_Channel timer;
	public:
		Flip(EntityID id) : id_(id) {
			component_mgr = &*CORE->GetManager<ComponentManager>();
			motion = component_mgr->GetComponent<Motion>(id_);
			ai_ = component_mgr->GetComponent<AI>(id_);
			graphics = CORE->GetSystem<GraphicsSystem>();
			renderer = component_mgr->GetComponent<AnimationRenderer>(id_);

			timer.TimerStart();
		}

		bool run() override
		{
			if (ai_->GetState() == AI::AIState::Detected) {
				ai_->SetState(AI::AIState::Search);
				renderer->SetReversed(true);
				renderer->SetAnimationStatus(true);
			}
			else if(renderer->FinishedAnimating()){
				renderer->SetAnimationStatus(false);
			}
			if (!pc) {
				pc = component_mgr->GetComponent<ParentChild>(id_);
				auto& children = pc->GetChildren();

				for (auto& child : children) {
					cone = component_mgr->GetComponent<ConeLight>(child->GetID());
					pt = component_mgr->GetComponent<PointLight>(child->GetID());
				}
			}

			timer.TimerUpdate();
			if (cone)
				cone->SetColor({ 1, 1, 1 });
			if (pt)
				pt->SetColor({ 1, 1, 1 });

			if (timer.TimeElapsed(s) > 3.0f)
			{
				motion->SetIsLeft(!motion->IsLeft());
				if (motion->IsLeft()) {
					motion->SetVelocity({ -0.1f, 0.0f });
				}
				else
					motion->SetVelocity({ 0.1f, 0.0f });
				graphics->FlipTextureY(renderer);
				timer.TimerReset();
			}

			return false;
		}
	};

};

#endif