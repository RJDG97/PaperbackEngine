#ifndef _MOTION_H_
#define _MOTION_H_

#include "Entity.h" 
#include "Vector2D.h"

class Motion : public Component {
	Vector2D velocity_;
    Vector2D acceleration_;
public:
	friend class Physics;

	Motion();
	~Motion();

	void Init();
	void PublishResults();
	//void serialize(ISerializer& str);
};

#endif