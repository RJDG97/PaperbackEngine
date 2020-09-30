#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "Systems/ISystem.h"
#include "Entity/Entity.h"
#include "Entity/ComponentTypes.h"
#include "Entity/ComponentCreator.h"
#include "Systems/Message.h"
#include "Components/Transform.h"
#include "Components/Health.h"
#include "Systems/Factory.h"
#include "Components/Motion.h"
#include <unordered_map>

class Physics : public ISystem
{
public:
	// Broadcast physics collision messages after physics has updated
	// the entities position
	void PublishResults();

	void ChangeVelocity(Message* m);

	void Rotate(Transform* pos);

	//void DecreaseHP(Health* hp);

	void AddTransformComponent(EntityID id, Transform* transform);
	void RemoveTransformComponent(EntityID id);

	void AddMotionComponent(EntityID id, Motion* motion);
	void RemoveMotionComponent(EntityID id);

	Physics();

	void Init() override;
	virtual void Update(float frametime) override;
	virtual std::string GetName() override { return "Physics"; }
	void SendMessageD(Message* msg) override;

private:

	bool debug;

	using TransformIt = std::unordered_map<EntityID, Transform*>::iterator;
	std::unordered_map<EntityID,Transform*> transform_arr_;

	/*using HPIt = std::unordered_map<EntityID, Health>::iterator;
	std::unordered_map<EntityID, Health> hp_arr_;*/

	using MotionIt = std::unordered_map<EntityID, Motion*>::iterator;
	std::unordered_map<EntityID, Motion*> motion_arr_;
};

extern Physics* PHYSICS;

#endif