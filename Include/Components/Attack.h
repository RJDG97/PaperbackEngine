#ifndef _ATTACK_H_
#define _ATTACK_H_

#include "Entity/Entity.h"

class Attack : public Component {
	float attack_power_;
public:

	Attack();

	void Init();
	void PublishResults();
	//void Serialize(ISerializer& str);
};

#endif