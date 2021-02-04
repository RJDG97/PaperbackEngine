#include "Script/Stag_Tree.h"

void Stag_Tree::StagRoot::CollisionResponse(EntityID obj)
{
	UNREFERENCED_PARAMETER(obj);
}

Stag_Tree::CheckAlive::CheckAlive(EntityID id) : id_(id) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
	respawn_timer_.TimerStop();
	respawn_timer_.TimerReset();
}

bool Stag_Tree::CheckAlive::run()
{
	if (ai_->GetLife()) {
		return true;
	}
	else
	{
		// If Timer has not started, start
		if (respawn_timer_.TimeElapsed(s) == 0)
			respawn_timer_.TimerStart();
		// Update Timer if timer has not reached respawn time
		if (respawn_timer_.TimeElapsed(s) < 10.0f)
		{
			component_mgr->GetComponent<AnimationRenderer>(id_)->SetAlive(false);
			component_mgr->GetComponent<PointLight>(id_)->SetAlive(false);
			component_mgr->GetComponent<ConeLight>(id_)->SetAlive(false);
			respawn_timer_.TimerUpdate();
		}
		else // Stop timer and reset to 0
		{
			respawn_timer_.TimerStop();
			respawn_timer_.TimerReset();
			component_mgr->GetComponent<AnimationRenderer>(id_)->SetAlive(true);
			component_mgr->GetComponent<PointLight>(id_)->SetAlive(true);
			component_mgr->GetComponent<ConeLight>(id_)->SetAlive(true);
			obj_rigidbody_->SetPosition({
				ai_->GetDestinations().begin()->x,
				ai_->GetDestinations().begin()->y });
			ai_->SetState(AI::AIState::Patrol);
			ai_->SetLife(true);
		}
		return false;
	}
}

Stag_Tree::CheckSentry::CheckSentry(EntityID id) : id_(id) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
}

bool Stag_Tree::CheckSentry::run()
{
	if(ai_->GetNumDes() == 1)
		return true;
	return false;
}

Stag_Tree::SentryReturn::SentryReturn(EntityID id) : id_(id) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
}

bool Stag_Tree::SentryReturn::run()
{
	if (ai_->GetDestinations().begin()->x == obj_rigidbody_->GetOffsetAABBPos().x && 
		ai_->GetDestinations().begin()->y == obj_rigidbody_->GetOffsetAABBPos().y)
		return false;
	return true;
}

Stag_Tree::SentryAnim::SentryAnim(EntityID id) :id_(id) {
	graphics = CORE->GetSystem<GraphicsSystem>();
	component_mgr = &*CORE->GetManager<ComponentManager>();
	renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
	motion = component_mgr->GetComponent<Motion>(id_);
	name = component_mgr->GetComponent<Name>(id_);
	ai = component_mgr->GetComponent<AI>(id_);
}

bool Stag_Tree::SentryAnim::run() {
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

Stag_Tree::AtWaypoint::AtWaypoint(EntityID id) : id_(id) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
}

bool Stag_Tree::AtWaypoint::run()
{
	float distance = Vector2DLength(*ai_->GetCurrentDes() - obj_rigidbody_->GetOffsetAABBPos());
	// If object is at next path node
	if (distance < 1.0f || ai_->GetPath().empty())
		return false;
	return true;
}

Stag_Tree::ChangeWaypoint::ChangeWaypoint(EntityID id) : id_(id) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
}

bool Stag_Tree::ChangeWaypoint::run()
{
	DestinationIt next_it = ai_->GetCurrentDes();
	if (++next_it == std::end(ai_->GetDestinations()))
		next_it = std::begin(ai_->GetDestinations());
	ai_->SetCurrentDes(next_it);
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

Stag_Tree::CheckPath::CheckPath(EntityID id) : id_(id) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
	map_ = &*CORE->GetManager<AMap>();
}

bool Stag_Tree::CheckPath::run(){
	Vector2D CurrentDes = *ai_->GetCurrentDes();
	if (CurrentDes.x != SetDes.x || CurrentDes.y != SetDes.y) {
		SetDes = *ai_->GetCurrentDes();
		ai_->GetPath().clear();
		return map_->Pathing(ai_->GetPath(), obj_rigidbody_->GetOffsetAABBPos(), *ai_->GetCurrentDes());
	}
	return true;
}

Stag_Tree::Move::Move(EntityID id, float spd) : id_(id), Speed_(spd) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
	forces_ = &*CORE->GetManager<ForcesManager>();
}

bool Stag_Tree::Move::run() {
	if (ai_->GetPath().empty())
		return false;

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

Stag_Tree::PlayerWithinDistance::PlayerWithinDistance(EntityID id, float dist)
	: id_(id),
	detectdistance_(dist) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
}

void Stag_Tree::PlayerWithinDistance::PlayerInit() 
{
	player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities().back()->GetID();
	player_status_ = component_mgr->GetComponent<Status>(player_id_);
	player_rigidbody_ = component_mgr->GetComponent<Transform>(player_id_);
}

bool Stag_Tree::PlayerWithinDistance::run() 
{
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

Stag_Tree::PlayerWithinVision::PlayerWithinVision(EntityID id) : id_(id) 
{
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
}

void Stag_Tree::PlayerWithinVision::PlayerInit() 
{
	player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities().back()->GetID();
	player_status_ = component_mgr->GetComponent<Status>(player_id_);
	player_rigidbody_ = component_mgr->GetComponent<Transform>(player_id_);
}

bool Stag_Tree::PlayerWithinVision::run() {
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

Stag_Tree::ChasePath::ChasePath(EntityID id) : id_(id) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
	map_ = &*CORE->GetManager<AMap>();
}

void Stag_Tree::ChasePath::PlayerInit() {
	player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities().back()->GetID();
	player_rigidbody_ = component_mgr->GetComponent<Transform>(player_id_);
}

bool Stag_Tree::ChasePath::run() {
	if (!player_id_)
		PlayerInit();
	Vector2D CurrentDes = *ai_->GetCurrentDes();
	if (CurrentDes.x != SetDes.x || CurrentDes.y != SetDes.y) {
		SetDes = *ai_->GetCurrentDes();
		ai_->GetPath().clear();
		return map_->Pathing(ai_->GetPath(), obj_rigidbody_->GetOffsetAABBPos(), player_rigidbody_->GetOffsetAABBPos());
	}
	return true;
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
	player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities().back()->GetID();
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

Stag_Tree::IdleAnim::IdleAnim(EntityID id) :id_(id) {
	graphics = CORE->GetSystem<GraphicsSystem>();
	component_mgr = &*CORE->GetManager<ComponentManager>();
	renderer = component_mgr->GetComponent<AnimationRenderer>(id_);
	motion = component_mgr->GetComponent<Motion>(id_);
	name = component_mgr->GetComponent<Name>(id_);
	ai_ = component_mgr->GetComponent<AI>(id_);
}

bool Stag_Tree::IdleAnim::run() {

	// If any pointers are invalid, return
	if (!renderer || !ai_ || !motion || !name)
		return false;

	if (ai_->GetState() == AI::AIState::Attack) {
		ai_->SetState(AI::AIState::Return);
		MessageBGM_Play msg{ "EnemyLostSight" };
		CORE->BroadcastMessage(&msg);
		graphics->ChangeAnimation(renderer, "Stagbeetle_Confused");
	}

	if (ai_->GetState() == AI::AIState::Return && !renderer->FinishedAnimating()) {
		motion->SetForce(0);
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