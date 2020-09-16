#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Composition.h" // prolly icomponent.h
#include "../Vector2D.h"

class Transform : public Component {

public:
	Vector2D _position;
	float _rotation;

	Transform();
	void init();
	void PublishResults();
	//void serialize(ISerializer& str);
};

#endif