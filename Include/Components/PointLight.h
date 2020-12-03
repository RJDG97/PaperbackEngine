/**********************************************************************************
*\file         PointLight.cpp
*\brief        Contains declaration of functions and variables used for
*			   the PointLight Component
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once

#ifndef _POINTLIGHT_H_
#define _POINTLIGHT_H_

#include <windows.h>
#include "Entity/Entity.h"
#include "Manager/ModelManager.h"
#include "Manager/ShaderManager.h"
#include "MathLib/Vector2D.h"

class PointLight : public Component {

	Model model_;
	Shader shdr_pgm_;

	glm::vec3 color_;
	float radius_;
	float intensity_;

	glm::vec2 pos_;

public:

	friend class LightingSystem;


/******************************************************************************/
/*!
  \fn PointLight()

  \brief Constructor for PointLight that defaults the data members of the
		 component
*/
/******************************************************************************/
	PointLight();

/******************************************************************************/
/*!
	\fn ~PointLight()

	\brief Destructor for PointLight that removes the component from the
		 Lighting system point light map
*/
/******************************************************************************/
	~PointLight();

/******************************************************************************/
/*!
	\fn Init()

	\brief Adds the component itself to the Lighting System's point light map
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
  \fn Serialize()

  \brief Serialises a component into JSON format
*/
/******************************************************************************/
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;

/******************************************************************************/
/*!
	\fn Serialize(std::stringstream& data)

	\brief Reads data from a stringstream and stores them into the data members
*/
/******************************************************************************/
	void DeSerialize(std::stringstream& data) override;

/******************************************************************************/
/*!
  \fn DeSerializeClone()

  \brief Retrieves data that will initialise data that can be unique from other
		 components
*/
/******************************************************************************/
	void DeSerializeClone(std::stringstream& data) override;

/******************************************************************************/
/*!
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone() override;

/******************************************************************************/
/*!
	\fn GetRadius()

	\brief Gets the radius of the component
*/
/******************************************************************************/
	float GetRadius();

/******************************************************************************/
/*!
	\fn GetIntensity()
	
	\brief Gets the intensity of the component
*/
/******************************************************************************/
	float GetIntensity();

/******************************************************************************/
/*!
	\fn GetColor()

	\brief Get the color of the component
*/
/******************************************************************************/
	glm::vec3 GetColor();

/******************************************************************************/
/*!
	\fn SetRadius()

	\brief Sets the radius of the component
*/
/******************************************************************************/
	void SetRadius(float new_radius);

/******************************************************************************/
/*!
	\fn SetIntensity()

	\brief Sets the intensity of the component
*/
/******************************************************************************/
	void SetIntensity(float new_intensity);

/******************************************************************************/
/*!
	\fn SetColor()

	\brief Sets the color of the component
*/
/******************************************************************************/
	void SetColor(glm::vec3 new_color);
	
};

#endif