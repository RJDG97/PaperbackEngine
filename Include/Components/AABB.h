#ifndef _AABB_H_
#define _AABB_H_

#include "Entity/Entity.h" 
#include "MathLib/Vector2D.h"
#include "IComponent.h"
#include <sstream>

class AABB : public Component {
	Vector2D top_right_;
    Vector2D bottom_left_;
public:
	friend class Collision;

	AABB();
	~AABB();

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

	/*
	template <typename ComponentType>
	ComponentType* GetEntityComponent() {
		ComponentType* return_val = GetOwner()->GetComponent<ComponentType>();
		assert(return_val && "Component does not exist");
		return return_val;
	}
	*/

	void Init();
	void Serialize(std::stringstream& data) override;

	AABB* Clone() override;
};

#endif