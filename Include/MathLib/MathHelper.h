/**********************************************************************************
*\file         MathHelper.h
*\brief        Contains declaration of functions used for generic math conversions
*
*\author	   Jun Pu, Lee, 50% Code Contribution
*\author	   Low Shun Qiang, Bryan, 50% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


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