#ifndef _SCALE_H_
#define _SCALE_H_

#include "Entity/Entity.h"
#include "MathLib/Vector2D.h"
#include <sstream>
//#include "Graphics.h"

class Scale : public Component { // double check friend class afterwards
	Vector2D scale_;
public:
	friend class Graphics;
	friend class Collision;

	Scale();

	void Init();
	void PublishResults();
	void Serialize(std::stringstream& data) override;
};

#endif