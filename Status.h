#ifndef _STATUS_H_
#define _STATUS_H_

#include "Entity.h" 

class Status : public Component {
	bool b_player_hit_;
	// Invulnerability period after getting hit
    float counter_;
public:

	Status();

	void init();
	void PublishResults();
	//void serialize(ISerializer& str);
};

#endif