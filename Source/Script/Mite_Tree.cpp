/**********************************************************************************
*\file         Mite_Tree.cpp
*\brief        Contains definition of functions and variables used for
*			   the Mite AI
*
*\author	   Renzo Garcia, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Script/Mite_Tree.h"

void Mite_Tree::MiteRoot::CollisionResponse(EntityID obj){
	UNREFERENCED_PARAMETER(obj);
}

Mite_Tree::WalkAnim::WalkAnim(EntityID id) :id_(id) {
	graphics = CORE->GetSystem<GraphicsSystem>();
	component_mgr = &*CORE->GetManager<ComponentManager>();
	renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
	motion = component_mgr->GetComponent<Motion>(id_);
	name = component_mgr->GetComponent<Name>(id_);
	ai = component_mgr->GetComponent<AI>(id_);
}

bool Mite_Tree::WalkAnim::run() {
	// If any pointers are invalid, return
	if (!renderer || !ai || !motion || !name)
		return false;
	ai->SetState(AI::AIState::Patrol);
	// If velocity is essentially 0, set player to idle
	if (VerifyZeroFloat(motion->GetVelocity().x) && VerifyZeroFloat(motion->GetVelocity().y))
		graphics->ChangeAnimation(renderer, "Mite_Idle");
	graphics->ChangeAnimation(renderer, "Mite_Walk");

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

Mite_Tree::DetectAnim::DetectAnim(EntityID id) :id_(id) {
	graphics = CORE->GetSystem<GraphicsSystem>();
	component_mgr = &*CORE->GetManager<ComponentManager>();
	renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
	motion = component_mgr->GetComponent<Motion>(id_);
	name = component_mgr->GetComponent<Name>(id_);
	ai_ = component_mgr->GetComponent<AI>(id_);
}

bool Mite_Tree::DetectAnim::run() {

	// If any pointers are invalid, return
	if (!renderer || !ai_ || !motion || !name)
		return false;

	if (ai_->GetState() == AI::AIState::Patrol) {
		ai_->SetState(AI::AIState::Detected);
		MessageBGM_Play msg{ "EnemyDetect" };
		CORE->BroadcastMessage(&msg);
		graphics->ChangeAnimation(renderer, "Mite_Alert");
	}

	if (ai_->GetState() == AI::AIState::Detected && !renderer->FinishedAnimating()) {

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

Mite_Tree::ChaseAnim::ChaseAnim(EntityID id) :id_(id) {
	graphics = CORE->GetSystem<GraphicsSystem>();
	component_mgr = &*CORE->GetManager<ComponentManager>();
	renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
	motion = component_mgr->GetComponent<Motion>(id_);
	name = component_mgr->GetComponent<Name>(id_);
	ai_ = component_mgr->GetComponent<AI>(id_);
}

bool Mite_Tree::ChaseAnim::run() {

	// If any pointers are invalid, return
	if (!renderer || !ai_ || !motion || !name)
		return false;

	if (ai_->GetState() == AI::AIState::Detected) {
		ai_->SetState(AI::AIState::Chase);
		MessageBGM_Play msg{ "EnemyAttack" };
		CORE->BroadcastMessage(&msg);
		graphics->ChangeAnimation(renderer, "Mite_Walk");
	}

	if (ai_->GetState() == AI::AIState::Chase) {
		// If velocity is essentially 0, set player to idle
		if (VerifyZeroFloat(motion->GetVelocity().x) && VerifyZeroFloat(motion->GetVelocity().y))
			graphics->ChangeAnimation(renderer, "Mite_Idle");
		else {
			graphics->ChangeAnimation(renderer, "Mite_Walk");
		}

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

Mite_Tree::AttackAnim::AttackAnim(EntityID id) :id_(id) {
	graphics = CORE->GetSystem<GraphicsSystem>();
	component_mgr = &*CORE->GetManager<ComponentManager>();
	renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
	motion = component_mgr->GetComponent<Motion>(id_);
	name = component_mgr->GetComponent<Name>(id_);
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
}


void Mite_Tree::AttackAnim::PlayerInit()
{
	player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities()->GetID();
	player_status_ = component_mgr->GetComponent<Status>(player_id_);
	player_renderer_ = component_mgr->GetComponent<AnimationRenderer>(player_id_);
	player_health_ = component_mgr->GetComponent<Health>(player_id_);
	player_rigidbody_ = component_mgr->GetComponent<Transform>(player_id_);
}

bool Mite_Tree::AttackAnim::run() {

	if (!player_id_)
		PlayerInit();

	// If any pointers are invalid, return
	if (!renderer || !ai_ || !motion || !name)
		return false;

	if (ai_->GetState() != AI::AIState::Attack) {
		ai_->SetState(AI::AIState::Attack);
		graphics->ChangeAnimation(renderer, "Mite_Explode");
		return true;
	}
	if (ai_->GetState() == AI::AIState::Attack && !renderer->FinishedAnimating()) {
		motion->SetForce(0);
		motion->SetVelocity({0.0f, 0.0f});

		if (motion->GetVelocity().x > 0 && motion->IsLeft()) {
			graphics->FlipTextureY(renderer);
			motion->SetIsLeft(false);
		}
		else if (motion->GetVelocity().x < 0 && !motion->IsLeft()) {
			graphics->FlipTextureY(renderer);
			motion->SetIsLeft(true);
		}
	}
	else if (renderer->FinishedAnimating()) {
		float distance = Vector2DDistance(player_rigidbody_->GetOffsetAABBPos(), obj_rigidbody_->GetOffsetAABBPos());

		if (distance < 2.0f && player_status_->GetStatus() != StatusType::BURROW); {
			player_status_->SetStatus(StatusType::HIT);
			player_health_->SetCurrentHealth(player_health_->GetCurrentHealth() - 1);

			graphics->ChangeAnimation(player_renderer_, "Player_Hit");

			MessageBGM_Play player_msg{ "PlayerHurt" };
			CORE->BroadcastMessage(&player_msg);
		}

		MessageBGM_Play msg{ "EnemyExplode" };
		CORE->BroadcastMessage(&msg);
		renderer->SetAnimationStatus(false);
		ai_->SetLife(false);
	}
	return true;
}

Mite_Tree::ConfusedAnim::ConfusedAnim(EntityID id) :id_(id) {
	graphics = CORE->GetSystem<GraphicsSystem>();
	component_mgr = &*CORE->GetManager<ComponentManager>();
	renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
	motion = component_mgr->GetComponent<Motion>(id_);
	name = component_mgr->GetComponent<Name>(id_);
	ai_ = component_mgr->GetComponent<AI>(id_);
}

bool Mite_Tree::ConfusedAnim::run() {
	// If any pointers are invalid, return
	if (!renderer || !ai_ || !motion || !name || ai_->GetState() == AI::AIState::Patrol)
		return false;
	if (ai_->GetState() == AI::AIState::Search)
		return true;

	if (ai_->GetState() == AI::AIState::Chase || ai_->GetState() == AI::AIState::Detected) {
		ai_->SetState(AI::AIState::Confused);
		MessageBGM_Play msg{ "EnemyLostSight" };
		CORE->BroadcastMessage(&msg);
		graphics->ChangeAnimation(renderer, "Mite_Confused");
		return true;
	}
	if (ai_->GetState() == AI::AIState::Confused && !renderer->FinishedAnimating()) {

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
	if (renderer->FinishedAnimating()) {
		ai_->SetState(AI::AIState::Search);
		return true;
	}
}

Mite_Tree::IdleAnim::IdleAnim(EntityID id) :id_(id) {
	graphics = CORE->GetSystem<GraphicsSystem>();
	component_mgr = &*CORE->GetManager<ComponentManager>();
	renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
	motion = component_mgr->GetComponent<Motion>(id_);
	name = component_mgr->GetComponent<Name>(id_);
	ai_ = component_mgr->GetComponent<AI>(id_);
	running = false;
}

bool Mite_Tree::IdleAnim::run() {
	if (!renderer || !ai_ || !motion || !name)
		return false;
	if (ai_->GetNumDes() > 1 && ai_->GetLevel()) {
		if (!running) {
			graphics->ChangeAnimation(renderer, "Mite_Idle");
			running = true;
			return false;
		}
		else {
			if (renderer->FinishedAnimating()) {
				running = false;
				return true;
			}

			if (motion->GetVelocity().x > 0 && motion->IsLeft()) {
				graphics->FlipTextureY(renderer);
				motion->SetIsLeft(false);
			}
			else if (motion->GetVelocity().x < 0 && !motion->IsLeft()) {
				graphics->FlipTextureY(renderer);
				motion->SetIsLeft(true);
			}
			return false;
		}
	}
}

Mite_Tree::SearchCheck::SearchCheck(EntityID id) :id_(id) {
	graphics = CORE->GetSystem<GraphicsSystem>();
	component_mgr = &*CORE->GetManager<ComponentManager>();
	renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
	motion = component_mgr->GetComponent<Motion>(id_);
	name = component_mgr->GetComponent<Name>(id_);
	ai_ = component_mgr->GetComponent<AI>(id_);
	pass = false;
}

bool Mite_Tree::SearchCheck::run()
{
	if (ai_->GetState() == AI::AIState::Patrol)
		return false;
	if (ai_->GetState() == AI::AIState::Confused)
		return true;
	if (!pass && ai_->GetState() == AI::AIState::Search) {
		pass = true;
		ai_->SetState(AI::AIState::Confused);
		return true;
	}
	if (pass)
	{
		ai_->SetState(AI::AIState::Patrol);
		pass = false;
		return false;
	}
}