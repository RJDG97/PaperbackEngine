#pragma once

#ifndef _TEXTRENDERER_H_
#define _TEXTRENDERER_H_

#include <windows.h>
#include "Entity/Entity.h"
#include "Manager/ModelManager.h"
#include "Manager/FontManager.h"
#include "Manager/ShaderManager.h"
#include <glm/glm.hpp>

class TextRenderer : public Component {

	Model* model_;
	Shader* shdr_pgm_;

	Font* font_;
	std::string text_;
	glm::vec3 color_;
	float scale_;

	int layer_;

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
	  \fn Serialize()

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
	  \fn Clone()

	  \brief Clones the existing component
	*/
	/******************************************************************************/
	std::shared_ptr<Component> Clone() override;
};

#endif