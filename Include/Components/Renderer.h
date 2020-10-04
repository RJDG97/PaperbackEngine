#pragma once

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "Entity/Entity.h"
#include "Manager/ModelManager.h"
#include "Manager/TextureManager.h"
#include "Manager/ShaderManager.h"
#include <glm/glm.hpp>

class Renderer : public Component {

protected:

	bool x_flipped_ = true;
	bool y_flipped_ = false;

	Model model_;
	Shader shdr_pgm_;

	Texture texture_;
	int layer_;

	glm::mat3 mdl_to_ndc_xform_;		//model-to-NDC transform
	glm::mat3 mdl_xform_;				//model (model-to-world)

public:

	Renderer();
	~Renderer();

	void Init();

	void ChangeTexture(std::string texture_name);
	void ChangeModel(std::string model_name);
	void ChangeShdrpgm(std::string shdr_pgm_name);

	void FlipTextureX();
	void FlipTextureY();


	virtual void Update(float frametime, glm::mat3 world_to_ndc_xform);
	void Draw();
	int GetLayer();

	void Serialize(std::stringstream& data) override;
};

#endif