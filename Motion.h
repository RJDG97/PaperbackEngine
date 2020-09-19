#ifndef _MOTION_H_
#define _MOTION_H_

#include "Entity.h" 
#include "Vector2D.h"
//#include "Physics.h"

class Motion : public Component {
	Vector2D velocity_;
    Vector2D acceleration_;
public:
	friend class Physics;

	Motion();

	void init();
	void PublishResults();
	//void serialize(ISerializer& str);
};

#endif