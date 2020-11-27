#ifndef MATH_HELPER_H_
#define MATH_HELPER_H_

#include "MathLib/Vector2D.h"
#include "Components/SoundEmitter.h"
#include <glm/gtc/type_ptr.hpp>
#include <vector>

/******************************************************************************/
/*!
  \fn RoundDown()

  \brief Rounds down the value of a float
*/
/******************************************************************************/
void RoundDown(float& var);

/******************************************************************************/
/*!
  \fn RoundUp()

  \brief Rounds up the value of a float
*/
/******************************************************************************/
void RoundUp(float& var);

/******************************************************************************/
/*!
  \fn SoundLineToGLM()

  \brief Converts a sound line in vector2d to glm
*/
/******************************************************************************/
void SoundLineToGLM(SoundLine& line, std::vector<glm::vec2>& line2);


#endif 