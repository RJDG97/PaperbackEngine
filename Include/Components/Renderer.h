#pragma once

#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <windows.h>
#include "Entity/Entity.h"
#include "Manager/ModelManager.h"
#include "Manager/TextureManager.h"
#include "Manager/ShaderManager.h"
#include <glm/glm.hpp>

class Renderer : public Component {

protected:

	bool x_mirror_ = false;
	bool y_mirror_ = false;

	Model* model_;
	Shader* shdr_pgm_;
	Texture texture_;
	int layer_;

	glm::mat3 mdl_to_ndc_xform_;		//model-to-NDC transform
	glm::mat3 mdl_xform_;				//model (model-to-world)

public:

	friend class GraphicsSystem;

	Renderer();
	~Renderer();

	void Init();

	void Serialize(std::stringstream& data) override;
};

#endif