#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "ISystem.h"
#include "Entity.h"
#include "ComponentTypes.h"
#include "ComponentCreator.h"
#include "Message.h"
#include "Transform.h"
#include "Health.h"
#include "Factory.h"
#include <unordered_map>

class Physics : public ISystem
{
	// Check whether entities are colliding
	void DetectCollision();

	// Broadcast physics collision messages after physics has updated
	// the entities position
	void PublishResults();

	// Draw debugging boxes
	void DebugDraw();

	void Rotate(Transform* pos);

	void DecreaseHP(Health* hp);

	void SendMessageD(Message* msg);

public:
	Physics();
	void init();
	virtual void update(float frametime);
	virtual std::string GetName() { return "Physics"; }

	using TransformIt = std::unordered_map<EntityID, Transform>::iterator;
	std::unordered_map<EntityID,Transform> Transforms;

	using HPIt = std::unordered_map<EntityID, Health>::iterator;
	std::unordered_map<EntityID, Health> HPs;
};

extern Physics* PHYSICS;

#endif