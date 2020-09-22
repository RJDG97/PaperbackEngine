#ifndef _SCALE_H_
#define _SCALE_H_

#include "Entity.h" 
//#include "Graphics.h"

class Scale : public Component { // double check friend class afterwards
	float width_scale_;
    float height_scale_;
public:
	// friend class Graphics;

	Scale();

	void Init();
	void PublishResults();
	//void Serialize(ISerializer& str);
};

#endif