#pragma once
#ifndef _IRENDERER_H_
#define _IRENDERER_H_

#include <windows.h>
#include "Entity/Entity.h"
#include "Manager/ModelManager.h"
#include "Manager/ShaderManager.h"

class IRenderer : public Component {

protected:

	Model* model_;
	Shader* shdr_pgm_;

	glm::mat3 mdl_to_ndc_xform_;		//model-to-NDC transform
	glm::mat3 mdl_xform_;				//model (model-to-world)

	GLuint* texture_handle_;
	std::vector<glm::vec2>* tex_vtx_;
	int layer_;

public:

	friend class GraphicsSystem;

};

#endif