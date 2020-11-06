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
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone() override;

	float GetRadius();
	float GetIntensity();
	void SetRadius(float newRadius);
	void SetIntensity(float newIntensity);
	
};

#endif