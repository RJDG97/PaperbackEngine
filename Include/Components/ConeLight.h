/**********************************************************************************
*\file         ConeLight.h
*\brief        Contains declaration of functions and variables used for
*			   the ConeLight Component
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once

#ifndef _CONELIGHT_H_
#define _CONELIGHT_H_

#include <windows.h>
#include "Entity/Entity.h"
#include "Manager/ModelManager.h"
#include "Manager/ShaderManager.h"
#include "MathLib/Vector2D.h"

class ConeLight : public Component {

	Model model_;
	Shader shdr_pgm_;

	glm::vec3 color_;
	float radius_;
	float intensity_;
	float angle_;

	glm::vec2 pos_;
	glm::vec2 direction_;

	bool alive_;

	bool pulse_;
	float max_radius_;
	float min_radius_;
	float max_intensity_;
	float min_intensity_;
	float cycle_duration_;
	float elapsed_time_{0.0f};

public:

	friend class LightingSystem;


	/******************************************************************************/
	/*!
	  \fn ConeLight()

	  \brief Constructor for PointLight that defaults the data members of the
			 component
	*/
	/******************************************************************************/
	ConeLight();

	/******************************************************************************/
	/*!
		\fn ~ConeLight()

		\brief Destructor for PointLight that removes the component from the
			 Lighting system point light map
	*/
	/******************************************************************************/
	~ConeLight();

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
    \fn SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)

    \brief Serialises a component as a clone into JSON format
    */
    /******************************************************************************/
	void SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;


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
		\fn GetAngle()

		\brief Get the Angle of the component
	*/
	/******************************************************************************/
	float GetAngle();

	/******************************************************************************/
	/*!
		\fn GetAlive()

		\brief Get the alive of the component
	*/
	/******************************************************************************/
	bool GetAlive();

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

	/******************************************************************************/
	/*!
		\fn SetAngle()

		\brief Set the Angle of the component
	*/
	/******************************************************************************/
	void SetAngle(float new_angle);

	/******************************************************************************/
	/*!
		\fn SetAlive()

		\brief Set the alive of the component
	*/
	/******************************************************************************/
	void SetAlive(bool alive);

};

#endif