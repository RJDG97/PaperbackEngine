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

	Transform();

	void init();
	void PublishResults();
	//void serialize(ISerializer& str);
};

#endif