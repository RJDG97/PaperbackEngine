#include "MathLib/MathHelper.h"
#include "Engine/Core.h"
#include <cmath>

void RoundDown(float& var) {

	int tmp = static_cast<int>(var);
	var = static_cast<float>((var < tmp) ? static_cast<int>(var) - 1 : static_cast<int>(var));
};


void RoundUp(float& var) {

	int tmp = static_cast<int>(var);
	var = static_cast<float>((var < tmp) ? static_cast<int>(var) : static_cast<int>(var) + 1);
};

void SoundLineToGLM(SoundLine& line, std::vector<glm::vec2>& line2) {

	const float scale = CORE->GetGlobalScale();

	line2.push_back( {scale * line.start_.x, scale * line.start_.y} );
	line2.push_back( { scale * line.end_.x, scale * line.end_.y} );
}