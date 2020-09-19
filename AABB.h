#ifndef _AABB_H_
#define _AABB_H_

#include "Entity.h" 
#include "Vector2D.h"
//#include "Collision.h"

class AABB : public Component {
	Vector2D top_left_;
    Vector2D bottom_right_;
public:
	//friend class Collision;

	AABB();

	void init();
	void PublishResults();
	//void serialize(ISerializer& str);
};

#endif