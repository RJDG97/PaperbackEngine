#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "Entity/Entity.h"
#include "MathLib/Vector2D.h"
#include <sstream>

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