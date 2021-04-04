#include "Script/Common_Nodes.h"

int Alert::level;

void Alert::increment() {
	if (level < 5)
		level++;
}

Common::CheckAlive::CheckAlive(EntityID id) : id_(id) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
	respawn_timer_.TimerStop();
	respawn_timer_.TimerReset();
}

bool Common::CheckAlive::run()
{
	if (ai_->GetLife()) {
		return true;
	}
	else
	{
		respawn_timer_.TimerUpdate();
		// If Timer has not started, start
		if (respawn_timer_.TimeElapsed(s) == 0) {
			respawn_timer_.TimerStop();
			respawn_timer_.TimerReset();
			respawn_timer_.TimerStart();
			component_mgr->GetComponent<AnimationRenderer>(id_)->SetAlive(false);
			ParentChild* pc = component_mgr->GetComponent<ParentChild>(id_);
			auto& children = pc->GetChildren();

			for (auto& child : children) {

				ConeLight* cone = component_mgr->GetComponent<ConeLight>(child->GetID());
				PointLight* pt = component_mgr->GetComponent<PointLight>(child->GetID());

				if (cone)
					cone->SetAlive(false);
				if (pt)
					pt->SetAlive(false);
			}
		}
		if (respawn_timer_.TimeElapsed(s) > 10.0f)
		{
			respawn_timer_.TimerStop();
			respawn_timer_.TimerReset();
			component_mgr->GetComponent<AnimationRenderer>(id_)->SetAlive(true);
			ParentChild* pc = component_mgr->GetComponent<ParentChild>(id_);
			auto& children = pc->GetChildren();

			for (auto& child : children) {

				ConeLight* cone = component_mgr->GetComponent<ConeLight>(child->GetID());
				PointLight* pt = component_mgr->GetComponent<PointLight>(child->GetID());

				if (cone)
					cone->SetAlive(true);
				if (pt)
					pt->SetAlive(true);
			}
			obj_rigidbody_->SetPosition({
				ai_->GetDestinations().begin()->x,
				ai_->GetDestinations().begin()->y });
			ai_->SetState(AI::AIState::Patrol);
			ai_->GetPath().clear();
			ai_->SetLife(true);
		}
		// Update Timer if timer has not reached respawn time
		return false;
	}
}

Common::PlayerWithinDistance::PlayerWithinDistance(EntityID id, float dist)
	: id_(id),
	detectdistance_(dist) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
}

void Common::PlayerWithinDistance::PlayerInit()
{
	player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities()->GetID();
	player_status_ = component_mgr->GetComponent<Status>(player_id_);
	player_rigidbody_ = component_mgr->GetComponent<Transform>(player_id_);
}

bool Common::PlayerWithinDistance::run()
{
	if (!player_id_)
		PlayerInit();

	if (ai_->GetType() == AI::AIType::Mite && ai_->GetState() == AI::AIState::Attack)
		return true;

	if (player_status_->GetStatus() != StatusType::INVISIBLE)
	{
		if (!ai_->GetLevel() && player_status_->GetStatus() == StatusType::BURROW)
			return false;

		ai_->SetPlayerLastPos(player_rigidbody_->GetOffsetAABBPos());

		// Find current distance of player from obj
		float distance = Vector2DDistance(player_rigidbody_->GetOffsetAABBPos(), obj_rigidbody_->GetOffsetAABBPos());
		// If Player is very close, is detected

		if (ai_->GetLevel()) {
			if (distance < (detectdistance_ + (Alert::level * 0.2f))) {
				Alert::increment();
				return true;
			}
		}
		else {
			if (distance < detectdistance_) {
				Alert::increment();
				return true;
			}
		}
	}
	return false;
}

Common::PlayerWithinVision::PlayerWithinVision(EntityID id, float dist)
	: id_(id),
	detectdistance_(dist){
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
}

void Common::PlayerWithinVision::PlayerInit()
{
	player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities()->GetID();
	player_status_ = component_mgr->GetComponent<Status>(player_id_);
	player_rigidbody_ = component_mgr->GetComponent<Transform>(player_id_);
}

bool Common::PlayerWithinVision::run() {
	if (!player_id_)
		PlayerInit();

	if (player_status_->GetStatus() != StatusType::INVISIBLE)
	{
		if (!ai_->GetLevel() && player_status_->GetStatus() == StatusType::BURROW)
			return false;

		ai_->SetPlayerLastPos(player_rigidbody_->GetOffsetAABBPos());

		// Find current distance of player from obj
		float distance = Vector2DDistance(player_rigidbody_->GetOffsetAABBPos(), obj_rigidbody_->GetOffsetAABBPos());

		if (ai_->GetLevel()) {
			if (distance < ( detectdistance_ + (Alert::level * 0.2f) ))
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
				if (angle > (-45.0f - (Alert::level * 5)) && angle < (45.0f + (Alert::level * 5))) {
					// Note: will have to check for object obstruction in the line of sight
					ai_->GetPath().clear();
					Alert::increment();
					return true;
				}	
			}
		}
		else {
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
				if (angle > -45.0f && angle < 45.0f) {
					// Note: will have to check for object obstruction in the line of sight
					ai_->GetPath().clear();
					Alert::increment();
					return true;
				}
			}
		}
	}
	return false;
}

Common::SentryReturn::SentryReturn(EntityID id) : id_(id) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
}

bool Common::SentryReturn::run()
{
	if (ai_->GetDestinations().begin()->x == obj_rigidbody_->GetOffsetAABBPos().x &&
		ai_->GetDestinations().begin()->y == obj_rigidbody_->GetOffsetAABBPos().y)
		return false;
	return true;
}

Common::AtWaypoint::AtWaypoint(EntityID id) : id_(id) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
}

bool Common::AtWaypoint::run()
{
	ai_->SetState(AI::AIState::Patrol);
	float distance = Vector2DLength(*ai_->GetCurrentDes() - obj_rigidbody_->GetOffsetAABBPos());
	// If object is at next path node
	if (distance < 1.0f || ai_->GetPath().empty())
		return false;
	return true;
}

Common::ChangeWaypoint::ChangeWaypoint(EntityID id) : id_(id) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
}

bool Common::ChangeWaypoint::run()
{
	DestinationIt next_it = ai_->GetCurrentDes();
	if (++next_it == std::end(ai_->GetDestinations()))
		next_it = std::begin(ai_->GetDestinations());
	ai_->SetCurrentDes(next_it);
	return true;
}

Common::CheckPath::CheckPath(EntityID id) : id_(id) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
	map_ = &*CORE->GetManager<AMap>();
}

bool Common::CheckPath::run() {
	Vector2D CurrentDes = *ai_->GetCurrentDes();
	if (CurrentDes.x != SetDes.x || CurrentDes.y != SetDes.y) {
		SetDes = *ai_->GetCurrentDes();
		ai_->GetPath().clear();
		return map_->Pathing(ai_->GetPath(), obj_rigidbody_->GetOffsetAABBPos(), *ai_->GetCurrentDes());
	}
	return true;
}

Common::Move::Move(EntityID id, float spd) : id_(id), Speed_(spd) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
	forces_ = &*CORE->GetManager<ForcesManager>();
}

bool Common::Move::run() {
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

Common::ChasePath::ChasePath(EntityID id) : id_(id) {
	component_mgr = &*CORE->GetManager<ComponentManager>();
	ai_ = component_mgr->GetComponent<AI>(id_);
	obj_rigidbody_ = component_mgr->GetComponent<Transform>(id_);
	map_ = &*CORE->GetManager<AMap>();
}

void Common::ChasePath::PlayerInit() {
	player_id_ = CORE->GetManager<EntityManager>()->GetPlayerEntities()->GetID();
	player_rigidbody_ = component_mgr->GetComponent<Transform>(player_id_);
}

bool Common::ChasePath::run() {
	if (!player_id_)
		PlayerInit();
	Vector2D CurrentDes = *ai_->GetCurrentDes();
	if (CurrentDes.x != SetDes.x || CurrentDes.y != SetDes.y || ai_->GetPath().empty()) {
		SetDes = *ai_->GetCurrentDes();
		ai_->GetPath().clear();
		return map_->Pathing(ai_->GetPath(), obj_rigidbody_->GetOffsetAABBPos(), player_rigidbody_->GetOffsetAABBPos());
	}
	return true;
}