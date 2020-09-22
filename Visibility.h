#ifndef _VISIBILITY_H_
#define _VISIBILITY_H_

#include "Entity.h" 

class Visibility : public Component {
	bool b_detected_;
	// Stays in the same position for some time after player is out of range
    float counter_;
public:

	Visibility();

	void Init();
	void PublishResults();
	//void Serialize(ISerializer& str);
};

#endif