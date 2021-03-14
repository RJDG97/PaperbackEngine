/**********************************************************************************
*\file         Stag_Tree.cpp
*\brief        Contains definition of functions and variables used for
*			   the Stag Beetle AI
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Script/Stag_Tree.h"

void Stag_Tree::StagRoot::CollisionResponse(EntityID obj)
{
	UNREFERENCED_PARAMETER(obj);
}

Stag_Tree::IdleAnim::IdleAnim(EntityID id) :id_(id) {
	graphics = CORE->GetSystem<GraphicsSystem>();
	component_mgr = &*CORE->GetManager<ComponentManager>();
	renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
	motion = component_mgr->GetComponent<Motion>(id_);
	name = component_mgr->GetComponent<Name>(id_);
	ai = component_mgr->GetComponent<AI>(id_);
}

bool Stag_Tree::IdleAnim::run() {
	// If any pointers are invalid, return
	if (!renderer || !ai || !motion || !name)
		return false;
	ai->SetState(AI::AIState::Patrol);
	// If velocity is essentially 0, set player to idle
	graphics->ChangeAnimation(renderer, "Stagbeetle_Idle");

	if (motion->GetVelocity().x > 0 && motion->IsLeft()) {
		graphics->FlipTextureY(renderer);
		motion->SetIsLeft(false);
	}
	else if (motion->GetVelocity().x < 0 && !motion->IsLeft()) {
		graphics->FlipTextureY(renderer);
		motion->SetIsLeft(true);
	}
	return true;
}

Stag_Tree::WalkAnim::WalkAnim(EntityID id) :id_(id) {
	graphics = CORE->GetSystem<GraphicsSystem>();
	component_mgr = &*CORE->GetManager<ComponentManager>();
	renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
	motion = component_mgr->GetComponent<Motion>(id_);
	name = component_mgr->GetComponent<Name>(id_);
	ai = component_mgr->GetComponent<AI>(id_);
}

bool Stag_Tree::WalkAnim::run(){
	// If any pointers are invalid, return
	if (!renderer || !ai || !motion || !name)
		return false;
	ai->SetState(AI::AIState::Patrol);
	// If velocity is essentially 0, set player to idle
	if (VerifyZeroFloat(motion->GetVelocity().x) && VerifyZeroFloat(motion->GetVelocity().y))
		graphics->ChangeAnimation(renderer, "Stagbeetle_Idle");
	else
		graphics->ChangeAnimation(renderer, "Stagbeetle_Walk");

	if (motion->GetVelocity().x > 0 && motion->IsLeft()) {
		graphics->FlipTextureY(renderer);
		motion->SetIsLeft(false);
	}
	else if (motion->GetVelocity().x < 0 && !motion->IsLeft()) {
		graphics->FlipTextureY(renderer);
		motion->SetIsLeft(true);
	}
	return true;
}

Stag_Tree::DetectAnim::DetectAnim(EntityID id) :id_(id) {
	graphics = CORE->GetSystem<GraphicsSystem>();
	component_mgr = &*CORE->GetManager<ComponentManager>();
	renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
	motion = component_mgr->GetComponent<Motion>(id_);
	name = component_mgr->GetComponent<Name>(id_);
	ai_ = component_mgr->GetComponent<AI>(id_);
}

bool Stag_Tree::DetectAnim::run() {

	// If any pointers are invalid, return
	if (!renderer || !ai_ || !motion || !name)
		return false;

	if (ai_->GetState() == AI::AIState::Patrol) {
		ai_->SetState(AI::AIState::Detected);
		MessageBGM_Play msg{ "EnemyDetect" };
		CORE->BroadcastMessage(&msg);
		graphics->ChangeAnimation(renderer, "Stagbeetle_Alert");
	}

	if (ai_->GetState() == AI::AIState::Detected && !renderer->FinishedAnimating()) {
		// If velocity is essentially 0, set player to idle
		if (VerifyZeroFloat(motion->GetVelocity().x) && VerifyZeroFloat(motion->GetVelocity().y))
			graphics->ChangeAnimation(renderer, "Stagbeetlee_Idle");

		if (motion->GetVelocity().x > 0 && motion->IsLeft()) {
			graphics->FlipTextureY(renderer);
			motion->SetIsLeft(false);
		}
		else if (motion->GetVelocity().x < 0 && !motion->IsLeft()) {
			graphics->FlipTextureY(renderer);
			motion->SetIsLeft(true);
		}
		return true;
	}
	return false;
}

Stag_Tree::ChaseAnim::ChaseAnim(EntityID id) :id_(id) {
	graphics = CORE->GetSystem<GraphicsSystem>();
	component_mgr = &*CORE->GetManager<ComponentManager>();
	renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
	motion = component_mgr->GetComponent<Motion>(id_);
	name = component_mgr->GetComponent<Name>(id_);
	ai_ = component_mgr->GetComponent<AI>(id_);
}

bool Stag_Tree::ChaseAnim::run() {

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

		if (motion->GetVelocity().x > 0 && motion->IsLeft()) {
			graphics->FlipTextureY(renderer);
			motion->SetIsLeft(false);
		}
		else if (motion->GetVelocity().x < 0 && !motion->IsLeft()) {
			graphics->FlipTextureY(renderer);
			motion->SetIsLeft(true);
		}
	}
	return true;
}

Stag_Tree::Charge::Charge(EntityID id) :id_(id) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
	forces_ = &*CORE->GetManager<ForcesManager>();
}

void Stag_Tree::Charge::PlayerInit() {
	player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities()->GetID();
	player_status_ = component_mgr->GetComponent<Status>(player_id_);
	player_rigidbody_ = component_mgr->GetComponent<Transform>(player_id_);
}

bool Stag_Tree::Charge::run(){
	if (!player_id_)
		PlayerInit();

	if (ai_->GetState() == AI::AIState::Attack || ai_->GetState() == AI::AIState::Chase) {
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
	return false;
}

Stag_Tree::AttackAnim::AttackAnim(EntityID id) :id_(id) {
	graphics = CORE->GetSystem<GraphicsSystem>();
	component_mgr = &*CORE->GetManager<ComponentManager>();
	renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
	motion = component_mgr->GetComponent<Motion>(id_);
	name = component_mgr->GetComponent<Name>(id_);
	ai_ = component_mgr->GetComponent<AI>(id_);
}

bool Stag_Tree::AttackAnim::run() {

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

		if (motion->GetVelocity().x > 0 && motion->IsLeft()) {
			graphics->FlipTextureY(renderer);
			motion->SetIsLeft(false);
		}
		else if (motion->GetVelocity().x < 0 && !motion->IsLeft()) {
			graphics->FlipTextureY(renderer);
			motion->SetIsLeft(true);
		}
	}
	return true;
}

Stag_Tree::ConfusedAnim::ConfusedAnim(EntityID id) :id_(id) {
	graphics = CORE->GetSystem<GraphicsSystem>();
	component_mgr = &*CORE->GetManager<ComponentManager>();
	renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
	motion = component_mgr->GetComponent<Motion>(id_);
	name = component_mgr->GetComponent<Name>(id_);
	ai_ = component_mgr->GetComponent<AI>(id_);
}

bool Stag_Tree::ConfusedAnim::run() {
	// If any pointers are invalid, return
	if (!renderer || !ai_ || !motion || !name)
		return false;

	if (ai_->GetState() == AI::AIState::Attack || ai_->GetState() == AI::AIState::Chase) {
		ai_->SetState(AI::AIState::Confused);
		MessageBGM_Play msg{ "EnemyLostSight" };
		CORE->BroadcastMessage(&msg);
		graphics->ChangeAnimation(renderer, "Stagbeetle_Confused");
	}
	if (ai_->GetState() == AI::AIState::Confused && !renderer->FinishedAnimating()) {
		// If velocity is essentially 0, set player to idle
		if (VerifyZeroFloat(motion->GetVelocity().x) && VerifyZeroFloat(motion->GetVelocity().y))
			graphics->ChangeAnimation(renderer, "Stagbeetlee_Idle");

		if (motion->GetVelocity().x > 0 && motion->IsLeft()) {
			graphics->FlipTextureY(renderer);
			motion->SetIsLeft(false);
		}
		else if (motion->GetVelocity().x < 0 && !motion->IsLeft()) {
			graphics->FlipTextureY(renderer);
			motion->SetIsLeft(true);
		}
		return true;
	}
	return false;
}