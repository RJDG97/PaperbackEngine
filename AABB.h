#ifndef _AABB_H_
#define _AABB_H_

#include "Entity.h" 
#include "Vector2D.h"

class AABB : public Component {
	Vector2D top_right_;
    Vector2D bottom_left_;
public:
	friend class Collision;

	AABB();

	Vector2D GetBottomLeft() const { 
		return bottom_left_;
	}

	Vector2D GetTopRight() const {
		return top_right_;
	}

	void SetBottomLeft(const Vector2D bottom_left) { 
		bottom_left_ = bottom_left;
	}

	void SetTopRight(const Vector2D& top_right) {
		top_right_ = top_right;
	}

	void Init();
	void PublishResults();
	//void Serialize(ISerializer& str);

	AABB* Clone() override;
};

#endif