#include "Systems/Collision.h"
#include "Systems/Factory.h"
#include "Components/Scale.h"
#include "Components/Transform.h"
#include "Entity/ComponentCreator.h"
#include "Entity/ComponentTypes.h"
#include "Components/AABB.h"
#include "Components/Motion.h"
#include <iostream>
#include <assert.h>

#define EPSILON 0.0001f

Collision* COLLISION;

Collision::Collision() {
	
	COLLISION = this;
}

// Comparison function
auto max = [](float a, float b) {
	return (a < b) ? a : b;
};

auto min = [](float a, float b) {
	return (a > b) ? a : b;
};

bool Collision::CheckCollision(const AABB& aabb1, const Vec2& vel1,
							   const AABB& aabb2, const Vec2& vel2,
							   const float dt) {

	// AABB_1
	Vector2D aab1_bot_left = aabb1.GetBottomLeft();
	Vector2D aab1_top_right = aabb1.GetTopRight();

	// AABB_2
	Vector2D aab2_bot_left = aabb2.GetBottomLeft();
	Vector2D aab2_top_right = aabb2.GetTopRight();

	if ((aab1_bot_left.x > aab2_top_right.x || aab1_top_right.x < aab2_bot_left.x) ||
		(aab1_bot_left.y > aab2_top_right.y || aab1_top_right.y < aab2_bot_left.y))
	{
		Vector2D Vb;
		float tFirst = 0, tLast = dt; // g_dt does not exist yet
		Vb.x = vel2.x - vel1.x;
		Vb.y = vel2.y - vel1.y;

		if (Vb.x == 0 || Vb.y == 0)
			return 0;

		// X-Axis check
		if (Vb.x < EPSILON)
		{
			//case 1
			if (aab1_bot_left.x > aab2_top_right.x)
				return 0;
			//case 4
			if (aab1_top_right.x < aab2_bot_left.x)
				tFirst = max((aab1_top_right.x - aab2_bot_left.x) / Vb.x, tFirst);
			if (aab1_bot_left.x < aab2_top_right.x)
				tLast = min((aab1_bot_left.x - aab2_top_right.x) / Vb.x, tLast);
		}
		if (Vb.x > EPSILON)
		{
			//case 2
			if (aab1_bot_left.x > aab2_top_right.x)
				tFirst = max((aab1_bot_left.x - aab2_top_right.x) / Vb.x, tFirst);
			if (aab1_top_right.x > aab2_bot_left.x)
				tLast = min((aab1_top_right.x - aab2_bot_left.x) / Vb.x, tLast);
			//case3
			if (aab1_top_right.x < aab2_bot_left.x)
				return 0;
		}
		if (tFirst > tLast)
			return 0;

		tFirst = 0, tLast = dt;

		// Y-Axis check
		if (Vb.y < EPSILON)
		{
			//case 1
			if (aab1_bot_left.y > aab2_top_right.y)
				return 0;
			//case 4
			if (aab1_top_right.y < aab2_bot_left.y)
				tFirst = max((aab1_top_right.y - aab2_bot_left.y) / Vb.y, tFirst);
			if (aab1_bot_left.y < aab2_top_right.y)
				tLast = min((aab1_bot_left.y - aab2_top_right.y) / Vb.y, tLast);
		}
		if (Vb.y > EPSILON)
		{
			//case 2
			if (aab1_bot_left.y > aab2_top_right.y)
				tFirst = max((aab1_bot_left.y - aab2_top_right.y) / Vb.y, tFirst);
			if (aab1_top_right.y > aab2_bot_left.y)
				tLast = min((aab1_top_right.y - aab2_bot_left.y) / Vb.y, tLast);
			//case3
			if (aab1_top_right.y < aab2_bot_left.y)
				return 0;
		}
		if (tFirst > tLast)
			return 0;
	}
	return 1;
}

//init function called to initialise a system
void Collision::Init() {

	FACTORY->AddComponentCreator("AABB", new ComponentCreatorType<AABB>(ComponentTypes::AABB));
}

//contains logic executed during the update loop of a game
void Collision::Update(float frametime) {
	(void)frametime;
	size_t counter = 0;
	UpdateBoundingBox();

	/*for (AABBIt box = aabb_arr_.begin(); box != aabb_arr_.end(); ++ box) {
		
		std::cout << "AABB " << counter << ": "
				  << "Top left: " << box->second->bottom_left_.x
				  << ", " << box->second->bottom_left_.y
				  << "\nBottom right: " << box->second->top_right_.x
				  << ", " << box->second->top_right_.y
				  << std::endl;
	}*/

	for (AABBIt aabb1 = aabb_arr_.begin(); aabb1 != aabb_arr_.end(); ++aabb1) {

		AABBIt aabb2 = aabb1;
		Motion* motion1 = dynamic_cast<Motion*>(aabb1->second->GetOwner()->GetComponent(ComponentTypes::MOTION));
		//assert(motion1 != nullptr && "aabb1 does not have a motion component");

		Vec2* vel1{};

		vel1 = (motion1 != nullptr) ? &motion1->velocity_ : &Vec2{};

		++aabb2;
		
		for (; aabb2 != aabb_arr_.end(); ++aabb2) {

			Motion* motion2 = dynamic_cast<Motion*>(aabb2->second->GetOwner()->GetComponent(ComponentTypes::MOTION));
			//assert(motion2 != nullptr && "aabb2 does not have a motion component");

			Vec2* vel2{};

			vel2 = (motion2 != nullptr) ? &motion2->velocity_ : &Vec2{};

			if (CheckCollision(*aabb1->second, *vel1, *aabb2->second, *vel2, frametime)) {

				std::cout << "Collision detected between " << aabb1->second->GetOwner()->GetID() 
					<< " and " << aabb2->second->GetOwner()->GetID() << std::endl;
			}
		}
	}
}

//void Collision::Draw() {
//	
//}

//function more akin to "What to do when message is received" for internal logic
void Collision::SendMessageD(Message* m) {

	(void)m;
}

void Collision::AddAABBComponent(EntityID id, AABB* aabb) {

	aabb_arr_[id] = aabb;
}

void Collision::RemoveAABBComponent(EntityID id) {

	AABBIt it = aabb_arr_.find(id);

	if (it != aabb_arr_.end()) {

		aabb_arr_.erase(it);
	}
}

void Collision::UpdateBoundingBox() {

	for (AABBIt aabb = aabb_arr_.begin(); aabb != aabb_arr_.end(); ++aabb) {
		
		Entity* entity = aabb->second->GetOwner();
		assert(entity && "Entity does not exist");

		Scale* entity_scale = dynamic_cast<Scale*>(aabb->second->GetOwner()->GetComponent(ComponentTypes::SCALE));
		Transform* entity_position = dynamic_cast<Transform*>(aabb->second->GetOwner()->GetComponent(ComponentTypes::TRANSFORM));

		aabb->second->bottom_left_ = entity_position->position_ - (0.5 * entity_scale->scale_);
		aabb->second->top_right_ = entity_position->position_ + (0.5 * entity_scale->scale_);
	}
}