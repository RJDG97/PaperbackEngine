#ifndef _ATTACK_H_
#define _ATTACK_H_

#include "Entity.h" 

class Attack : public Component {
	float attack_power_;
public:

	Attack();

	void init();
	void PublishResults();
	//void serialize(ISerializer& str);
};

#endif