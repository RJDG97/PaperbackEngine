#ifndef _COLLISION_H_
#define _COLLISION_H_

#include <string>
#include "Message.h"
#include "ISystem.h"
#include "AABB.h"
#include <unordered_map>

class Collision : public ISystem {

	using AABBIt = std::unordered_map<EntityID, AABB>::iterator;
	std::unordered_map<EntityID, AABB> aabb_arr_;
public:
	Collision();

	bool CheckCollision(const AABB &aabb1, const Vec2 &vel1, 
						const AABB &aabb2, const Vec2 &vel2,
						const float dt);

	void AddAABBComponent(EntityID id, AABB* aabb);

	//init function called to initialise a system
	virtual void Init() override;

	//contains logic executed during the update loop of a game
	virtual void Update(float frametime) override;

	//returns the name of the system for debug use
	virtual std::string GetName() override { return "Collision System"; }

	//function more akin to "What to do when message is received" for internal logic
	virtual void SendMessageD(Message* m) override;

	virtual ~Collision() = default;
};

extern Collision* COLLISION;

#endif