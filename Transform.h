#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Entity.h" // prolly icomponent.h
#include "Vector2D.h"
//#include "Physics.h" // for testing purposes (friend class)

class Transform : public Component {
	Vector2D position_;
	float rotation_;
public:
	friend class Physics;
	friend class AnimationRenderer;

	Transform();
	~Transform();

	void Init();
	void PublishResults();
	void Serialize(std::stringstream& data) override;
};

#endif