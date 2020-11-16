#include "MathLib/MathHelper.h"
#include <cmath>

void RoundDown(float& var) {

	int tmp = static_cast<int>(var);
	var = static_cast<float>((var < tmp) ? static_cast<int>(var) - 1 : static_cast<int>(var));
};


void RoundUp(float& var) {

	int tmp = static_cast<int>(var);
	var = static_cast<float>((var < tmp) ? static_cast<int>(var) : static_cast<int>(var) + 1);
};