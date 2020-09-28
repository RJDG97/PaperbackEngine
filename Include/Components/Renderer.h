#pragma once

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Entity/Entity.h"
#include "Manager/ModelManager.h"
#include "Manager/TextureManager.h"
#include "Manager/ShaderManager.h"
#include <glm/glm.hpp>

class Renderer : public Component {

	Model model_;
	Shader shdr_pgm_;

	Texture texture_;

	glm::mat3 mdl_to_ndc_xform_;		//model-to-NDC transform
	glm::mat3 mdl_xform_;				//model (model-to-world)

	glm::vec2 orientation_;				// x - angle of rotation in degrees, y - speed of rotation
	glm::vec2 scaling_;					//scaling parameters
	glm::vec2 position_;				//translation vector coordinates

public:

	Renderer();
	~Renderer();

	void Init();
	void PublishResults();
	//void Serialize(ISerializer& str);

	void ChangeTexture(GLint tex_id);
	void ChangeModel(GLint model_id);
	void ChangeShdrpgm(GLint shdrpgm_id);

	void Update(float frametime, glm::mat3 world_to_ndc_xform);
	void Draw();
};

#endif