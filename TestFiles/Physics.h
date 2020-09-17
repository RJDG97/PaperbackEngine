#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "ISystem.h"
#include "Composition.h"
#include "ComponentTypes.h"
#include "ComponentCreator.h"
#include "Message.h"
#include "Transform.h"
#include "Health.h"
#include "Factory.h"
#include <list>

class MessageRotation : public Entity_Message
{
public:
	MessageRotation(size_t entityID) : Entity_Message{ MessageIDTypes::Rotate, entityID } {}
	//GOC * CollidedWith;
	~MessageRotation() = default;
};

class MessageHPDecre : public Entity_Message
{
public:
	MessageHPDecre(size_t entityID) : Entity_Message{ MessageIDTypes::HP, entityID } {}
	//GOC * CollidedWith;
	~MessageHPDecre() = default;
};

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

	using PosIt = std::list<Transform>::iterator;
	std::list<Transform> Transforms;

	using HPIt = std::list<Health>::iterator;
	std::list<Health> HPs;
};

extern Physics* PHYSICS;

#endif