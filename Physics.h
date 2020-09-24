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
#include "Motion.h"
#include <unordered_map>

class Physics : public ISystem
{
public:
	// Broadcast physics collision messages after physics has updated
	// the entities position
	void PublishResults();

	// Draw debugging boxes
	void DebugDraw();

	void ChangeAcceleration(Message* m);

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
	using TransformIt = std::unordered_map<EntityID, Transform>::iterator;
	std::unordered_map<EntityID,Transform> transform_arr_;

	/*using HPIt = std::unordered_map<EntityID, Health>::iterator;
	std::unordered_map<EntityID, Health> hp_arr_;*/

	using MotionIt = std::unordered_map<EntityID, Motion>::iterator;
	std::unordered_map<EntityID, Motion> motion_arr_;
};

extern Physics* PHYSICS;

#endif