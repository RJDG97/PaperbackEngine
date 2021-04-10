/**********************************************************************************
*\file         TextRenderer.h
*\brief        Contains declaration of functions and variables used for
*			   the TextRenderer Component
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once

#ifndef _TEXTRENDERER_H_
#define _TEXTRENDERER_H_

#include <windows.h>
#include "Entity/Entity.h"
#include "Manager/ModelManager.h"
#include "Manager/FontManager.h"
#include "Manager/ShaderManager.h"
#include "Components/IRenderer.h"
#include <glm/glm.hpp>

class TextRenderer : public IRenderer {

	std::string font_name_;
	Font* font_;
	std::string text_;
	glm::vec3 color_;
	float scale_;
	bool center_align_;

public:

	friend class GraphicsSystem;

/******************************************************************************/
/*!
	\fn TextRenderer()

	\brief Constructor for TextRenderer that defaults the data members of
			the component
*/
/******************************************************************************/
	TextRenderer();

/******************************************************************************/
/*!
	\fn ~TextRenderer()

	\brief Destructor for TextRenderer that removes the component from the
			Graphics System's text renderer map
*/
/******************************************************************************/
	~TextRenderer();

/******************************************************************************/
/*!
	\fn Init()

	\brief Adds the component itself to the Graphics System's animation
			renderer map
*/
/******************************************************************************/
	void Init();

/******************************************************************************/
/*!
	\fn Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)

	\brief Serialises a component into JSON format
*/
/******************************************************************************/
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;

/******************************************************************************/
/*!
	\fn DeSerialize(std::stringstream& data)

	\brief Reads data from a stringstream and stores them into the data members
*/
/******************************************************************************/
	void DeSerialize(std::stringstream& data) override;

/******************************************************************************/
/*!
	\fn DeSerializeClone(std::stringstream& data)

	\brief Serializes data members within the Component
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
  \fn SetText()

  \brief Set text
*/
/******************************************************************************/
	void SetText(std::string text);

/******************************************************************************/
/*!
  \fn SetColor()

  \brief Sets color
*/
/******************************************************************************/
	void SetColor(glm::vec3 color) { color_ = color; }

/******************************************************************************/
/*!
	\fn SetColor()

	\brief Sets scale
*/
/******************************************************************************/
	void SetScale(float scale) { scale_ = scale; }

/******************************************************************************/
/*!
	\fn SetCenterAlign()

	\brief Sets center align
*/
/******************************************************************************/
	void SetCenterAlign(float value) { center_align_ = value; }

};

#endif