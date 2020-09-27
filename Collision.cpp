#include "Collision.h"

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
		if (Vb.x < 0.0f)
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
		if (Vb.x > 0.0f)
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
		if (Vb.y < 0.0f)
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
		if (Vb.y > 0.0f)
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

}

//contains logic executed during the update loop of a game
void Collision::Update(float frametime) {
	(void)frametime;
	//CheckCollision()
}

//function more akin to "What to do when message is received" for internal logic
void Collision::SendMessageD(Message* m) {

	(void)m;
}

void Collision::AddAABBComponent(EntityID id, AABB* aabb) {

	aabb_arr_[id] = aabb;
}
