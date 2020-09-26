#pragma once

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Entity.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include <glm/glm.hpp>

class Renderer : public Component {

	Model model;
	Shader shdr_pgm;

	Texture texture;

	glm::mat3 mdl_to_ndc_xform;		//model-to-NDC transform
	glm::mat3 mdl_xform;			//model (model-to-world)

	glm::vec2 orientation;			// x - angle of rotation in degrees, y - speed of rotation
	glm::vec2 scaling;				//scaling parameters
	glm::vec2 position;				//translation vector coordinates

public:

	Renderer();
	~Renderer();

	void Init();
	void PublishResults();
	//void Serialize(ISerializer& str);

	void Update(float frametime, glm::mat3 world_to_ndc_xform);
	void Draw();
};

#endif