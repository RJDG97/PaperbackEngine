/**********************************************************************************
*\file         TextureRenderer.h
*\brief        Contains declaration of functions and variables used for
*			   the TextureRenderer Component
*
*\author	   Mok Wen Qing, 100% Code Contribution
*
*\copyright    Copyright (c) 2021 DigiPen Institute of Technology. Reproduction
			   or disclosure of this file or its contents without the prior
			   written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once

#ifndef _TEXTURERENDERER_H_
#define _TEXTURERENDERER_H_

#include <windows.h>
#include "Components/SpriteRenderer.h"
#include "Entity/Entity.h"
#include "Manager/TextureManager.h"
#include <glm/glm.hpp>

class TextureRenderer : public SpriteRenderer {

	std::string texture_name_;
	Texture* texture_;

public:

	friend class GraphicsSystem;

/******************************************************************************/
/*!
	\fn TextureRenderer()

	\brief Constructor for TextureRenderer that defaults the data members of
			the component
*/
/******************************************************************************/
	TextureRenderer();

/******************************************************************************/
/*!
	\fn ~TextureRenderer()

	\brief Destructor for TextureRenderer that removes the component from the
			Graphics System's texture renderer map
*/
/******************************************************************************/
	~TextureRenderer();

/******************************************************************************/
/*!
	\fn Init()

	\brief Adds the component itself to the TextureRenderer's
		   texture renderer map
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
  \fn SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)

  \brief Serialises a component as a clone into JSON format
*/
/******************************************************************************/
	void SerializeClone(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;

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
  \fn Clone()

  \brief Clones the existing component
*/
/******************************************************************************/
	std::shared_ptr<Component> Clone() override;

/******************************************************************************/
/*!
  \fn InitTextures()

  \brief Called to initialise textures
*/
/******************************************************************************/
	void InitTextures();

/******************************************************************************/
/*!
  \fn GetCurrentTextureName()

  \brief Get the name of the texture
*/
/******************************************************************************/
	std::string GetCurrentTextureName();

};

#endif