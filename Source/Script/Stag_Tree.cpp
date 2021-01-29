#include "Script/Stag_Tree.h"

void Stag_Tree::StagRoot::CollisionResponse(EntityID obj)
{

}

Stag_Tree::CheckAlive::CheckAlive(EntityID id) : id_(id) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	respawn_timer_.TimerReset();
}

bool Stag_Tree::CheckAlive::run()
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
	Detect_timer_.TimerReset();
	Detect_timer_.TimerStop();
}

bool Stag_Tree::DetectAnim::run() {

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